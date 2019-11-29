/*
 * Copyright 2019 Kasper Biegun
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

char parties[6][13] = {"PiS", "KO", "Lewica", "PSL", "Konfederacja", "Niewazne"};


void generate_random_votes(int array_to_fill[], float sounding_results[], int comittees/*, int sum_of_votes*/ ) {
    //now not random
    float percentages[6];
    float random_sounding;
    float random;
    float sum;
    srand(time(0));
    for (int i = 0; i < comittees; i++) {
        //printf("Sounding result: %f\n", sounding_results[i]);
        random = (float)((rand() % 400) - 200)/10000; //do poprawy (+/- 2%)
        //float random = ((float)rand()/(float)(2))/100;// * sounding_results[i]+0.02;
        //printf("Random: %f\n", random);
        random_sounding = sounding_results[i] + random;
        //printf("Sounding result: %f\n", random_sounding);
        //((sounding_results[i]+2) + 1 - (sounding_results[i]-2)) - sounding_results[i]-2;
        //tmp = sounding_results[i] * (30253556*0.6174);
        //tmp = (random_sounding * (30253556*0.6174));
        //printf("Votes: %d\n\n", tmp);
        percentages[i] = random_sounding;
        //array_to_fill[i] = rand() % 5000;
        sum += random_sounding;
        //array_to_fill[i] = tmp;
    }
    //printf("Sum: %f\n", sum);
    //float sum1;
    for (int i = 0; i < comittees; i++) {
        percentages[i] = percentages[i]/sum;
        //sum1 += percentages[i];
        //printf("Sum: %f\n", sum);
        array_to_fill[i] = (int)(percentages[i] * (30253556*0.6174));
        //printf("Votes %s: %d\n\n", parties[i], array_to_fill[i]);
    }
    //printf("Sum: %f\n", sum1);
}

void max_value_and_index(int votes_array[], size_t size, int *max_votes_index) {
    //size_t i;
    int max_value = votes_array[0];
    int max_index = 0;

    for (int i = 0; i < size; i++) {
        if ( votes_array[i] > max_value ) {
            max_value = votes_array[i];
            max_index = i;
        }
    }
    *max_votes_index = max_index;
}

int main(int argc, char **argv)
{
	//int votes = 10;				//glosow
	int mandates_to_give = 460;   //mandatow do rozdania
	int comittees = 6;			//komitetow wyborczych
	//int constituencies = 41;	//okregow wyborczych
	int entitled_to_vote = 30253556;	//uprawnionych do głosowania
	float attendance = 0.6174;

    float sounding_results[6] = {0.44, 0.25, 0.14, 0.08, 0.07, 0.02};
    //int mandates_on_constituencies = {12,8,14,12,13,15,12,12,10,9,12,8,14,10,9,19,9,12,20,12,12,11,15,14,12,14,9,7,9,9,12,9,16,8,10,12,9,9,10,8,12};

    //int main_results[5] = {1228, 1012, 850, 543, 352, 0};
    int main_results[6];
    generate_random_votes(main_results, sounding_results, comittees);
    int mandates[5] = {0};
    int dHondt_results[5] = {0};
    int index;

    for (int i = 0; i < comittees; i++) {
        printf("Votes %s: %d\n", parties[i], main_results[i]);
    }

    for (int i = 0; i < mandates_to_give; i++) {
        for (int j=0; j<comittees; j++){
            dHondt_results[j]=main_results[j]/(mandates[j]+1);
        }
        max_value_and_index(dHondt_results, sizeof(dHondt_results)/sizeof(int), &index);
        mandates[index] += 1;
        //printf("Size: %d", sizeof(dHondt_results)/sizeof(int));
        //printf("Max Index (%d) = %d \n", i, index);
    }


    printf("\n\nElection results:\n");
    for (int i=0; i<comittees-1; i++) {
        printf("%s: %d Mandates \n", parties[i], mandates[i]);
    }
    printf("\nInvalid votes : %d, %0.2f%%", main_results[comittees-1], (main_results[comittees-1]/(30253556*0.6174)*100) );

	return 0;
}

