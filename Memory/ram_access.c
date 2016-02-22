#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LOOP_TIME_TEST_NUM 1000000
static const int arr_min_size = 256;
static const int stride_min_size = 4096;

void measure_latency(float overhead){
    unsigned t;
    float avg, stddev;
    int arr_size, stride_size, int_arr_size, int_stride_size;
    void **measure;

    for(int i=0; i<1; i++){
        stride_size = stride_min_size * pow(2,i);
        for(int j=0; j<20; j++){
            arr_size = arr_min_size * pow(2,j);
            int ** array = (int **) malloc(arr_size);

            int_arr_size = arr_size / sizeof(int *);
            int_stride_size = stride_size / sizeof(int *);
            for (int k = 0; k < int_arr_size; k++) {
                array[k] = (int *) &array[(k + int_stride_size) % int_arr_size];
            }

            measure = (void **)array;
            avg = 0.0;
            stddev = 0.0;
            for(int k=1; k<=LOOP_TIME_TEST_NUM; k++){
                t = ccnt_read();
                measure = *measure;
                t = ccnt_read() - t;

                float prev_avg = avg;
                avg += (t - overhead - prev_avg) / k;
                stddev += (t - overhead - prev_avg) * (t - overhead - avg);
            }
            stddev = sqrt(stddev / (LOOP_TIME_TEST_NUM - 1));
            printf("arr size = %d, stride size = %d, average = %f, std = %f\n", arr_size, stride_size, avg, stddev);

            free(array);
        }
    }
    
}

int main(){
    measure_latency(0);
}