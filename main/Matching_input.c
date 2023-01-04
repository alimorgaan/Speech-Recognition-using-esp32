#include "HEADER.h"

float *cross_correlation(float *series_x, float *series_y){	
	int delay_max = Input_Samples;
	float * cross_correlate = (float*)malloc(((delay_max*2)-1)*sizeof(float));
	for (int i = 0; i < 2*Input_Samples-1; i++)
    {
        cross_correlate[i] = 0;
        for (int j = 0; j < Input_Samples; j++)
        {
            if (i-j >= 0 && i-j < Input_Samples)
            {
                cross_correlate[i] += series_x[j] * series_y[i-j];
            }
        }
    }
    int maxv = cross_correlate[0];
    int minv = cross_correlate[0];
    for(int i = 0; i<2*Input_Samples-1;i++)
    {
       maxv = (cross_correlate[i] > maxv) ? (cross_correlate[i]) : maxv;
       minv = (cross_correlate[i] < minv) ? (cross_correlate[i]) : minv;
    }

    for (int i = 0; i < 2*Input_Samples-1; i++)
    {
       cross_correlate[i] = (cross_correlate[i] - minv)/(maxv-minv);
    }
	return cross_correlate;
}

int getMaxIndex(float * input , int size){
    int maxIndex = 0 ;
    for (int i = 0; i < size ; i++){
        if(input[i] > input[maxIndex])
            maxIndex = i ; 
    }
    return maxIndex ; 
}

float symMatch(float * input , float * ref){
    float *crossCorrelationArray = cross_correlation(input , ref); 
    int maxIndex = getMaxIndex(crossCorrelationArray , Input_Samples*2 - 1); 
    int startPtr = 0 ; 
    int endPtr = (Input_Samples*2-1)-1;
    int errorSize = (endPtr - maxIndex) > maxIndex ? (endPtr - maxIndex) : maxIndex ;
    float * errorArray = (float*)malloc(errorSize*sizeof(float)); 

    int errorPtr = 0; 
    while(startPtr < maxIndex && endPtr > maxIndex){
        errorArray[errorPtr] = crossCorrelationArray[endPtr] - crossCorrelationArray[startPtr] ; 
        if(errorArray[errorPtr] < 0.0)
            errorArray[errorPtr] *= -1; 
        endPtr--; 
        startPtr++ ; 
        errorPtr++; 
    }

    while(startPtr < maxIndex){
        errorArray[errorPtr] = crossCorrelationArray[startPtr];
        if(errorArray[errorPtr] < 0.0)
            errorArray[errorPtr] *= -1; 
        startPtr++;
        errorPtr++; 
    }

    while(endPtr > maxIndex){
        errorArray[errorPtr] = crossCorrelationArray[endPtr];
        if(errorArray[errorPtr] < 0.0)
            errorArray[errorPtr] *= -1; 
        endPtr--;
        errorPtr++; 
    }
    float errorResult = 0;
    for(int i = 0 ; i < errorSize ; i++){
        errorResult += errorArray[i]; 
    }
    free(errorArray);
    free(crossCorrelationArray);
    return errorResult; 
}

float error_rms (float ToBeMatched[], float CurrRef[])
{
    float temp,correlation_indicator = 0;
    for(int i = 0; i < Input_Samples; i++)
    {
        temp = pow(ToBeMatched[i],2) - pow(CurrRef[i],2);
        correlation_indicator += (temp>=0) ? sqrt(temp) : sqrt(-1*temp);
    }
    return correlation_indicator;
}



void Match_Word(float ToBeMatched[] , int mode)
{
    float * CurrRef = 0;
    
    float final_match[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    int max_corr_id = -1;
    
    for(int i = 0; i < 10; i++) //10
    {
        if(i==1 || i==7 || i==0)
        {
            switch (i)
            {
                case 0 : CurrRef = callref0(); printf("\nMatching with zero"); break;
                case 1 : CurrRef = callref1(); printf("\nMatching with one"); break;
                // case 2 : CurrRef = callref2(); printf("\ntest2\n"); break;
                // case 3 : CurrRef = callref3(); printf("\ntest3\n"); break;
                // case 4 : CurrRef = callref4(); printf("\ntest4\n"); break;
                // case 5 : CurrRef = callref5(); printf("\ntest5\n"); break;
                // case 6 : CurrRef = callref6(); printf("\ntest6\n"); break;
                case 7 : CurrRef = callref7(); printf("\nMatching with seven"); break;
                // case 8 : CurrRef = callref8(); printf("\ntest8\n"); break;
                // case 9 : CurrRef = callref9(); printf("\ntest9\n"); break;
            }
            if(mode == 1){
                final_match[i] = symMatch(ToBeMatched, CurrRef);
            }else{
                final_match[i] = error_rms(ToBeMatched, CurrRef);
            }
            
            free(CurrRef);
        }
    }
    
        printf("\n\nError comparing to zero : %f\n",final_match[0]);
        printf("Error comparing to one : %f\n",final_match[1]);
        printf("Error comparing to seven : %f\n",final_match[7]);

            if(final_match[1] < final_match[0]){
                max_corr_id = 1;
            }else{
                max_corr_id = 0; 
            }

            if(final_match[7] < final_match[max_corr_id]){
                max_corr_id = 7; 
            }
            
    printf("\nWORD DETECTED : %d\n",max_corr_id);
}

/*
idf.py build
idf.py flash -p COM13
idf.py monitor -p COM13
start idf.py menuconfig
*/