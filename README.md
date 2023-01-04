# Speech-Recognition-using-esp32
## Recognition of simple short words using FFT on esp32


### Input and Sampling Rate
- A mic is connected to amplifier circuit then to esp32 ADC. 
- Used ADC to store 8192 sample in one second. 
- A speaker is connected with DAC to playback the recorded word

### Word Detection Algorithm
- Calculate FFT for input samples  
- Calculate the magnitude and Normalize the spectrum 
- Compute the cross-correlation between the spectrum of the input and each refrence data stored in the memory
- the most symmtric cross-correlation result represents the best match for input word
