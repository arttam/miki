# Conversion between dB and amplitude

## Conversion Functions  

To convert from amplitude to dB, the formula is:

```cpp
dB = 20 * log10(amplitude)
```  

To convert from dB to amplitude, the formula is:
```cpp
amplitude = 10^(db/20)
```  

Note that when converting audio samples to dB, you want to take the absolute value of the audio sample, since sign doesn’t matter for loudness.   
-1 and +1 have the same loudness (0dB).  

Here’s some c++ code which does those two operations:

```cpp
inline float AmplitudeTodB(float amplitude)
{
  return 20.0f * log10(amplitude);
}
 
inline float dBToAmplitude(float dB)
{
  return pow(10.0f, db/20.0f);
}
```


## Conversion Table
Here are some dB values and corresponding amplitude values to help you better understand how dB and amplitude are related.  


### Decreasing Volume:

||dB	||Amplitude||  
| ---:  | ---: |
|-1	|0.891|   
|-3	|0.708|   
|-6	|0.501|   
|-12	|0.251|   
|-18	|0.126|   
|-20	|0.1|   
|-40	|0.01|   
|-60	|0.001|   
|-96	|0.00002|   


### Increasing Volume:

||dB	||Amplitude||   
| ---:  |  ---: |
|1	|1.122|   
|3	|1.413|   
|6	|1.995|   
|12	|3.981|   
|18	|7.943|   
|20	|10|   
|40	|100|   
|60	|1000|   
|96	|63095.734|   

