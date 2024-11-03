#include<iostream>
#include<stdio.h>
#include<cstdint>
#include<sys/time.h>
#include<chrono>
using namespace std;

#ifndef SIZE
#define SIZE 64000 
#endif
#define Guesses 1000
#define ll long long

unsigned ll arr[SIZE];

int main(){
    for (unsigned ll i = 0; i < SIZE; i++){
        arr[i]=i;
    }
    struct timeval tv;
    struct timezone tz;
    struct timeval tv, tv1; 
    struct timezone tz, tz1;
    double a;
    // chrono::steady_clock::time_point start = chrono::steady_clock::now();
    gettimeofday(&tv,&tz);
    unsigned ll int access=0;
    for (unsigned ll i = 0; i < Guesses; i++){
        a=arr[rand()%SIZE];
    }
    gettimeofday(&tv1,&tz1);
    double acc_time = (double)((tv1.tv_sec-tv.tv_sec)*1000000 + (double)(tv1.tv_usec - tv.tv_usec))/Guesses;
    printf("%f\n", acc_time);
}