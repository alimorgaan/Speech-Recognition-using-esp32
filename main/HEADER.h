#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/adc.h>
#include <driver/dac.h>
#include <esp_adc_cal.h>
#include <math.h>
#include "ref.h"
void adc_power_acquire(void);

#define LED_REC             2
#define LED_PLAY            27
#define REC_button         GPIO_NUM_19
#define PLAY_INPUT_button  GPIO_NUM_16
#define XCORRMATCH  GPIO_NUM_15
#define RMSMATCH  GPIO_NUM_4
//16384 8192
#define Input_Samples 8192

#define downsample_factor (Input_Samples/Input_Samples)

typedef float complex cplx;

void _FFT(cplx buf[], cplx out[], int n, int step);
void FFT(cplx buf[], int n);
void show(const char * s, float buf[],int samples);
void play_audio(float Signal[],int samples);
void init_ESP();
float* Record(float Signal[] ,  cplx copyFFT[]);
float* Compress(float Signal[]);
float* Magnitude_only(cplx Signal[]);
float* Normalize(float Signal[]);
float *cross_correlation(float *series_x, float *series_y);
int getMaxIndex(float * input , int size);
float symMatch(float * input , float * ref);
void Match_Word(float ToBeMatched[], int mode);
float error_rms(float ToBeMatched[], float CurrRef[]); 

float* callref0();
float* callref1();
float* callref2();
float* callref3();
float* callref4();
float* callref5();
float* callref6();
float* callref7();
float* callref8();
float* callref9();