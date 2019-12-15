/*
 * Copyright 2019 Kasper Biegun
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#define MAXLINE 100


float * read_data_from_file(const char *filename) {
    FILE *fp;
    char *lp, line[MAXLINE];
    float n;
    static float data[4];
    int i = 0;

    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Error: Couldn't open \"%s\"!\n", filename);
    }

    while (fgets(line, MAXLINE, fp) != NULL) {
        for (lp = line; *lp != '\n' && (lp - line) < MAXLINE; lp++) {
            if (*lp == ':' && sscanf(lp + 1, "%f", &n) == 1) {
                data[i] = n;
                i++;
                break;
            }
        }
    }

    fclose(fp);
    return data;
}

float * generate_sounding_results(int commitees) {
    float *sounding_results = malloc(commitees * sizeof(float));
    float *sounding_array = malloc(commitees * sizeof(float));
    float sum = 0;
    float random = 0;
    int i = 0;
    for (i = 0; i < commitees; i++) {
        random = (float)rand() / (RAND_MAX);
        if (i == commitees - 1) {
            random = (float)rand() / (RAND_MAX) * 0.03 + 0.02; // rand() / (RAND_MAX) * (max-min) + (min);
        }
        sounding_results[i] = random;
        sum += sounding_results[i];
    }
    for (i = 0; i < commitees; i++) {
        sounding_array[i] = sounding_results[i]/sum;
    }
    return sounding_array;
}

int * generate_random_votes(float sounding_results[], int commitees, int entitled_to_vote, float attendance) {
    float *percentages = malloc(commitees * sizeof(float));
    int *array_to_fill = malloc(commitees * sizeof(int));
    float random_sounding, random, sum = 0;
    int i;

    for (i = 0; i < commitees; i++) {
        random = (float)((rand() % 400) - 200)/4000; // (+/- 10%)
        random_sounding = sounding_results[i] + random;
        percentages[i] = fabs(random_sounding);
        sum += percentages[i];
    }

    for (i = 0; i < commitees; i++) {
        percentages[i] = percentages[i]/sum;
        array_to_fill[i] = (int)(percentages[i] * (entitled_to_vote*attendance));
    }

    return array_to_fill;
}

void max_value_and_index(int votes_array[], size_t size, int *max_votes_index) {
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
    int index, i, j, commitees, mandates_to_give, entitled_to_vote;
    time_t rawtime;
    char buffer [255];
    float *data_read, *sounding_results;
    int *main_results;
    float real_attendance, attendance;
    time (&rawtime);
    sprintf(buffer,"dHondt_%s",ctime(&rawtime) );

    srand(time(0));
    data_read = read_data_from_file("data.txt");
    entitled_to_vote = (int)data_read[1];
    real_attendance = data_read[0];
    commitees = (int)data_read[2];
    mandates_to_give = (int)data_read[3];

    int dHondt_results[commitees];
    int mandates[commitees];

    for (i = 0; i < commitees; i ++) {
        mandates[i] = 0;
        dHondt_results[i] = 0;
    }

    attendance = real_attendance + (float)((rand() % 400) - 200)/2000; // (+/- 20%)
    sounding_results = generate_sounding_results(commitees);
    main_results = generate_random_votes(sounding_results, commitees, entitled_to_vote, attendance);

    FILE *f = fopen(buffer, "w");

    printf("Entitled to vote: %d\tAttendance: %0.2f\n", entitled_to_vote, attendance);
    fprintf(f, "Entitled to vote: %d\tAttendance: %0.2f\n", entitled_to_vote, attendance);

    for (i = 0; i < mandates_to_give; i++) {
        for (j = 0; j < commitees; j++){
            dHondt_results[j] = main_results[j] / (mandates[j] + 1);
        }
        max_value_and_index(dHondt_results, sizeof(dHondt_results) / sizeof(int) - 1, &index);
        mandates[index] += 1;
    }

    printf("\n\nElection results:\n");
    fprintf(f, "\n\nElection results:\n");

    for (i = 0; i < commitees; i++) {
        if (i == commitees - 1) {
            printf("\nInvalid votes:\t%d\t(%0.2f%%)", main_results[i], (main_results[commitees - 1]/(entitled_to_vote*attendance)*100));
            fprintf(f, "\nInvalid votes:\t%d\t(%0.2f%%)", main_results[i], (main_results[commitees - 1]/(entitled_to_vote*attendance)*100));
        } else {
            printf("Party %d:\t%d Mandates\t(%d Votes) \n", i+1, mandates[i], main_results[i]);
            fprintf(f, "Party %d:\t%d Mandates\t(%d Votes) \n", i+1, mandates[i], main_results[i]);
        }
    }

	return 0;
}

