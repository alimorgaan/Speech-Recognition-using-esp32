# Speech-Recognition-using-esp32
## Recognition of simple short words using FFT on esp32


### Input and Sampling Rate
- A mic is connected to amplifier circuit then to esp32 ADC. 
- Used ADC to store 8192 sample in one second. 
- A speaker is connected with DAC to playback the recorded word

![photo_2023-01-04_04-16-53](https://user-images.githubusercontent.com/32411364/210472482-ee8e29e7-74b0-4fe6-a291-a99edabbec25.jpg)




https://user-images.githubusercontent.com/32411364/210846747-2c44ee65-18c5-4e62-b1fb-abaaf631d09a.mp4


### Word Detection Algorithm
- Calculate FFT for input samples  
- Calculate the magnitude and Normalize the spectrum 
- Compute the cross-correlation between the spectrum of the input and each refrence data stored in the memory
- the most symmtric cross-correlation result represents the best match for input word

![Capture](https://user-images.githubusercontent.com/32411364/210472642-de7bd34d-96fa-4be7-8d4f-ad255792e877.PNG)
