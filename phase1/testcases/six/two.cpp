#include<iostream>
#include<stdio.h>
#include <cstdint>
#include <sys/time.h>
#include<chrono>
using namespace std;

#ifndef SIZE
#define SIZE 33554432 
#endif
#define num_guesses 10000
#define prime 97

unsigned long long arr[SIZE];

double compute_average_access_time(unsigned long long assumed_size){
    struct timeval tv; struct timezone tz;
    struct timeval tv1; struct timezone tz1;
    double a;
    for (unsigned int j = 0; j < 10; j++)
    for (unsigned long long i = 0; i < assumed_size; i++){
        arr[i]=i;
    }
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    // gettimeofday(&tv,&tz);
    // unsigned long long int access=0;
    for (unsigned long long i = 0; i < num_guesses; i++){
        // srand(rand());
        // printf("%d\n", rand()%SIZE);
        // printf("1 %lx\n", (uintptr_t)(arr[rand()%SIZE]));
        a=arr[rand()%assumed_size];
        // access+=(100*(unsigned long long)SIZE)/(num_guesses)+5;
        // access%=(unsigned long long)SIZE;
    }
    // gettimeofday(&tv1,&tz1);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    return (double)chrono::duration_cast<chrono::microseconds>(end - start).count()/num_guesses;
    return (double)((tv1.tv_sec-tv.tv_sec)*1000000 + (double)(tv1.tv_usec - tv.tv_usec))/num_guesses;
    // return time_taken;
}

int main(){
    for (unsigned long long i = 0; i < SIZE; i++){
        arr[i]=i;
    }
    struct timeval tv;
    struct timezone tz;
    unsigned long long assumed_size=1024;
    for (unsigned int i = 10; i <= 18; i++){
        double acc_time = compute_average_access_time(assumed_size);
        printf("%llu : %f\n", assumed_size, acc_time);
        assumed_size*=2;
    }
    // gettimeofday(&tv,&tz);
    // printf("Seconds since 1/1/1970: %d\n",tv.tv_usec);unsige
    // gettimeofday(&tv,&tz);
    // printf("Seconds since 1/1/1970: %d\n",tv.tv_usec);
}

// #include<iostream>
// #include<stdio.h>
// #include <cstdint>
// #include <sys/time.h>
// using namespace std;

// #ifndef SIZE
// #define SIZE 1048576 
// #endif
// #define num_guesses 1000
// #define prime 97

// unsigned long long arr[SIZE];

// double compute_average_access_time(){
//     struct timeval tv; struct timezone tz;
//     struct timeval tv1; struct timezone tz1;
//     gettimeofday(&tv,&tz);
//     unsigned long long int access=0;
//     for (unsigned long long i = 0; i < num_guesses; i++){
//         arr[i];
//         access+=(100*(unsigned long long)SIZE)/(num_guesses)+5;
//         access%=(unsigned long long)SIZE;
//     }
//     gettimeofday(&tv1,&tz1);
//     return (double)((tv1.tv_sec-tv.tv_sec)*1000000 + (double)(tv1.tv_usec - tv.tv_usec))/num_guesses;
//     // return time_taken;
// }

// int main(){
//     // fill(arr[0], arr[0]+SIZE, 0);
//     int values_to_check[]={1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288};
//     for (unsigned long long k: values_to_check){
//         struct timeval tv;
//         struct timeval tv2;
//         gettimeofday(&tv,NULL);
//         unsigned long long val=0;
//         for (unsigned long long i=0; i < num_guesses; i++){
//             arr[val];
//             val+=239;
//             val%=k;
//             // for (unsigned long long j = 0; j < k; j++){
//             //     arr[val];
//             //     val+=269;
//             //     val%=k;
//             // }
//         }
//         gettimeofday(&tv2, NULL);
//         printf("%llu %f\n", k, (double)((tv2.tv_sec - tv.tv_sec) * 1000000 + (double)(tv2.tv_usec - tv.tv_usec))/num_guesses);
//     }
//     // struct timeval tv;
//     // struct timezone tz;
//     // double acc_time = compute_average_access_time();
//     // printf("%f\n", acc_time);
//     // gettimeofday(&tv,&tz);
//     // printf("Seconds since 1/1/1970: %d\n",tv.tv_usec);
//     // gettimeofday(&tv,&tz);
//     // printf("Seconds since 1/1/1970: %d\n",tv.tv_usec);
// }