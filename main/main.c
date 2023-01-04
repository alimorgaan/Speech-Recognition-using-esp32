#include "HEADER.h"

float * Input_Signal;
cplx  * Input_FFT;
float * Magnitude_Signal;

void app_main()
{
    init_ESP();
    int Rec_flag = 0;
    while (1)
    {
        //  Waiting for Button
        
        if(gpio_get_level(REC_button))
        {
            while(gpio_get_level(REC_button));
            if(Rec_flag) free(Input_Signal);
            if(Rec_flag) free(Input_FFT);
            if(Rec_flag) free(Magnitude_Signal);
            Rec_flag = 1;
            

            Input_Signal = (float*)malloc(Input_Samples*sizeof(float));
            Input_FFT = (cplx*)malloc(Input_Samples*sizeof(cplx));

            printf("\n\nRecord\n");
            Input_Signal = Record(Input_Signal , Input_FFT);
            printf("Record done\n");

            
            printf("\n\nFFT\n");
            FFT(Input_FFT,Input_Samples); 
            printf("nCalculating FFT Finished\n");

            printf("\n\nCalculating Magnitude\n");
            Magnitude_Signal = Magnitude_only(Input_FFT);

            printf("Calculating Magnitude finished \n");

            printf("\n\nNormalization\n");
            Magnitude_Signal = Normalize(Magnitude_Signal);
            //show("kdfalsdjf \n", Magnitude_Signal, Input_Samples);
            printf("Normalization finished\n");

        } 
        if(gpio_get_level(PLAY_INPUT_button))
        {
            while(gpio_get_level(PLAY_INPUT_button));
            if(Rec_flag) play_audio(Input_Signal,Input_Samples);
            else printf("Please Record first");
        }
        if(gpio_get_level(XCORRMATCH))
        {
            while(gpio_get_level(XCORRMATCH));
            printf("\n--------Matching using cross correlation method-------\n");
            printf("this may take some time\n");
            Match_Word(Magnitude_Signal , 1);
            printf("Matching done\n");
        }
        if(gpio_get_level(RMSMATCH))
        {
            while(gpio_get_level(RMSMATCH));
            printf("\n\nMatching using RMS method\n");
            Match_Word(Magnitude_Signal , 0);
        }
        
        vTaskDelay(5);
    }
}

/*
idf.py build
idf.py flash -p COM13
idf.py monitor -p COM13
start idf.py menuconfig
*/

    // 8129 976 rec 1300 play 
// 16384 120 rec 800 play