/*
 * Copyright 2019 Kasper Biegun
 */

#include <stdio.h>
#include <assert.h>

void max_value_and_index(int votes_array[], size_t size, int *max_votes_index) {
    /* enforce the contract */
    assert(votes_array && size);
    size_t i;
    int max_value = votes_array[0];
    int max_index = 0;

    for (i = 1; i < size; ++i) {
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
	int mandates_to_give = 20;   //mandatow do rozdania
	//int comittees = 10;			//komitetow wyborczych
	//int constituencies = 10;	//okregow wyborczych
	//int entitled_to_vote = 11;	//uprawnionych do głosowania
	char parties[5][13] = {"PiS", "KO", "SLD", "PSL", "Konfederacja"};

    int main_results[5] = {1228, 1012, 850, 543, 352};
    int mandates[5] = {0};
    int dHondt_results[5] = {0};
    int index;

    for (int i=0; i<mandates_to_give; i++) {
        for (int j=0; j<5; j++){
            dHondt_results[j]=main_results[j]/(mandates[j]+1);
        }
        max_value_and_index(dHondt_results, 5, &index);
        mandates[index] += 1;
        printf("Max Index (%d) = %d \n", i, index);
    }


    printf("\n\nWyniki wyborów:\n");
    for (int i=0; i<5; i++) {
        printf("Partia %s: %d mandatów \n", parties[i], mandates[i]);
    }

	return 0;
}

