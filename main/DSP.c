#include "HEADER.h"

double PI = (double)22/7;

void _FFT(cplx buf[], cplx out[], int n, int step)
{
	if (step < n) {
		_FFT(out, buf, n, step * 2);
		_FFT(out + step, buf + step, n, step * 2);
 
		for (int i = 0; i < n; i += 2 * step) {
			cplx t = cexp(-I * PI * i / n) * out[i + step];
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
		}
	}
}

void FFT(cplx buf[], int n)
{
    cplx * out = (cplx*)malloc(n*sizeof(cplx)); 
	for (int i = 0; i < n; i++) out[i] = buf[i];
	_FFT(buf, out, n, 1);
    free(out); 
}

void show(const char * s, float buf[],int samples)
{
	printf("%s", s);
	for (int i = 0; i < samples; i++)
			printf("%f\n", buf[i]);
		
}

void play_audio(float analogSig[],int samples)
{
    int i,j;
    dac_output_voltage(DAC_CHANNEL_1,0);
    gpio_set_level(LED_PLAY, 1);
    if(samples == Input_Samples)
    {
        printf("\nPlaying Original sound\n");
        for(i = 0; i < Input_Samples; i++ )
        {
            dac_output_voltage(DAC_CHANNEL_1,analogSig[i]/16);
            for(j = 0; j < 2047; j++); //850
        }
        dac_output_voltage(DAC_CHANNEL_1,0);
    }
    gpio_set_level(LED_PLAY, 0);
}

void init_ESP()
{
    dac_output_enable(DAC_CHANNEL_1);

    adc_power_acquire();
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);

    /*LED_REC = ON -> record \ LED_PLAY = ON -> play*/
    gpio_set_direction(LED_REC, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_PLAY, GPIO_MODE_OUTPUT);

    /*button for recording*/
    gpio_set_direction(REC_button, GPIO_MODE_INPUT);
    gpio_set_pull_mode(REC_button, GPIO_PULLDOWN_ONLY);

    /*button for playing input signal*/
    gpio_set_direction(PLAY_INPUT_button, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PLAY_INPUT_button, GPIO_PULLDOWN_ONLY);

    /*button for xcorr*/
    gpio_set_direction(XCORRMATCH, GPIO_MODE_INPUT);
    gpio_set_pull_mode(XCORRMATCH, GPIO_PULLDOWN_ONLY);


     /*button for rms*/
    gpio_set_direction(RMSMATCH, GPIO_MODE_INPUT);
    gpio_set_pull_mode(RMSMATCH, GPIO_PULLDOWN_ONLY);
    
}

float* Record(float Signal[] , cplx copyFFT[])
{
    gpio_set_level(LED_REC, 1);
    for (int i = 0; i < Input_Samples; i++)
    {
        Signal[i] = adc1_get_raw(ADC1_CHANNEL_0);
        for(int j = 0; j < 1600; j++);
    }
    for (int i = 0; i < Input_Samples; i++)
    {
        copyFFT[i] = Signal[i]; 
    }
    gpio_set_level(LED_REC, 0);
    return Signal;
}



float* Magnitude_only(cplx Signal[])
{
    float* MAG_COMP_Sig = (float*)malloc(Input_Samples*sizeof(float));
    for(int i = 0; i < Input_Samples; i++)
    {
        MAG_COMP_Sig[i] = sqrt(pow(creal((float)Signal[i]),2) + pow(cimag((float)Signal[i]),2));
    }
    return MAG_COMP_Sig;
}

float* Normalize(float Signal[])
{
    float max = 0.0;
    float min = 1000000.0;
    for(int i = 0; i < Input_Samples; i++)
    {
        max = (max<(float)Signal[i]) ? (float)Signal[i] : max;
        min = (min>(float)Signal[i]) ? (float)Signal[i] : min;
    }
    for(int i = 0; i < Input_Samples; i++)
    {
        Signal[i] = (Signal[i] - min) / (max - min);
    }
    return Signal;
}
