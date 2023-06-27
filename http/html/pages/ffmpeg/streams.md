# Stream selections

ffmpeg provides the -map option for manual control of stream selection in each output file.   
Users can skip `-map` and let ffmpeg perform automatic stream selection as described below.   
The `-vn / -an / -sn / -dn` options can be used to skip inclusion of video, audio, subtitle and data streams respectively,   
whether manually mapped or automatically selected, except for those streams which are outputs of complex filtergraphs.

## Description
The sub-sections that follow describe the various rules that are involved in stream selection.   
The examples that follow next show how these rules are applied in practice.

While every effort is made to accurately reflect the behavior of the program,   
FFmpeg is under continuous development and the code may have changed since the time of this writing.

## Automatic stream selection
In the absence of any map options for a particular output file, ffmpeg inspects the output format to check which type of streams can be included in it, viz. video, audio and/or subtitles.   
For each acceptable stream type, ffmpeg will pick one stream, when available, from among all the inputs.  

It will select that stream based upon the following criteria:  

- for video, it is the stream with the highest resolution,  
- for audio, it is the stream with the most channels,  
- for subtitles, it is the first subtitle stream found but there’s a caveat. The output format’s default subtitle encoder can be either text-based or image-based, and only a subtitle stream of the same type will be chosen.
In the case where several streams of the same type rate equally, the stream with the lowest index is chosen.

Data or attachment streams are not automatically selected and can only be included using `-map`.

## Manual stream selection
When -map is used, only user-mapped streams are included in that output file, with one possible exception for filtergraph outputs described below.

## Complex filtergraphs
If there are any complex filtergraph output streams with unlabeled pads, they will be added to the first output file.   
This will lead to a fatal error if the stream type is not supported by the output format.   
In the absence of the map option, the inclusion of these streams leads to the automatic stream selection of their types being skipped.   
If map options are present, these filtergraph streams are included in addition to the mapped streams.   

Complex filtergraph output streams with labeled pads must be mapped once and exactly once.   

## Stream handling
Stream handling is independent of stream selection, with an exception for subtitles described below.    
Stream handling is set via the -codec option addressed to streams within a specific output file.    
In particular, codec options are applied by ffmpeg after the stream selection process and thus do not influence the latter.     
If no -codec option is specified for a stream type, ffmpeg will select the default encoder registered by the output file muxer.   

An exception exists for subtitles.    
If a subtitle encoder is specified for an output file, the first subtitle stream found of any type, text or image, will be included.    
ffmpeg does not validate if the specified encoder can convert the selected stream or if the converted stream is acceptable within the output format.   
This applies generally as well: when the user sets an encoder manually, the stream selection process cannot check if the encoded stream can be muxed into the output file.    
If it cannot, ffmpeg will abort and all output files will fail to be processed.   

# Examples
The following examples illustrate the behavior, quirks and limitations of ffmpeg’s stream selection methods.   

They assume the following three input files.   
```
input file 'A.avi'
      stream 0: video 640x360
      stream 1: audio 2 channels

input file 'B.mp4'
      stream 0: video 1920x1080
      stream 1: audio 2 channels
      stream 2: subtitles (text)
      stream 3: audio 5.1 channels
      stream 4: subtitles (text)

input file 'C.mkv'
      stream 0: video 1280x720
      stream 1: audio 2 channels
      stream 2: subtitles (image)
```
Example: automatic stream selection
```
ffmpeg -i A.avi -i B.mp4 out1.mkv out2.wav -map 1:a -c:a copy out3.mov
```
There are three output files specified, and for the first two, no -map options are set, so ffmpeg will select streams for these two files automatically.   

out1.mkv is a Matroska container file and accepts video, audio and subtitle streams, so ffmpeg will try to select one of each type.   
For video, it will select stream 0 from B.mp4, which has the highest resolution among all the input video streams.   
For audio, it will select stream 3 from B.mp4, since it has the greatest number of channels.   
For subtitles, it will select stream 2 from B.mp4, which is the first subtitle stream from among A.avi and B.mp4.   

out2.wav accepts only audio streams, so only stream 3 from B.mp4 is selected.   

For out3.mov, since a -map option is set, no automatic stream selection will occur.   
The -map 1:a option will select all audio streams from the second input B.mp4.   
No other streams will be included in this output file.   

For the first two outputs, all included streams will be transcoded.   
The encoders chosen will be the default ones registered by each output format, which may not match the codec of the selected input streams.

For the third output, codec option for audio streams has been set to copy, so no decoding-filtering-encoding operations will occur, or can occur.   
Packets of selected streams shall be conveyed from the input file and muxed within the output file.   

Example: automatic subtitles selection
```
ffmpeg -i C.mkv out1.mkv -c:s dvdsub -an out2.mkv
```
Although out1.mkv is a Matroska container file which accepts subtitle streams, only a video and audio stream shall be selected.   
The subtitle stream of C.mkv is image-based and the default subtitle encoder of the Matroska muxer is text-based,   
so a transcode operation for the subtitles is expected to fail and hence the stream isn’t selected. However, in out2.mkv, a   
subtitle encoder is specified in the command and so, the subtitle stream is selected, in addition to the video stream.    
The presence of -an disables audio stream selection for out2.mkv.  

Example: unlabeled filtergraph outputs
```
ffmpeg -i A.avi -i C.mkv -i B.mp4 -filter_complex "overlay" out1.mp4 out2.srt
```
A filtergraph is setup here using the -filter_complex option and consists of a single video filter.   
The overlay filter requires exactly two video inputs, but none are specified, so the first two available video streams are used, those of A.avi and C.mkv.    
The output pad of the filter has no label and so is sent to the first output file out1.mp4.    
Due to this, automatic selection of the video stream is skipped, which would have selected the stream in B.mp4.    
The audio stream with most channels viz. stream 3 in B.mp4, is chosen automatically.    
No subtitle stream is chosen however, since the MP4 format has no default subtitle encoder registered, and the user hasn’t specified a subtitle encoder.   

The 2nd output file, out2.srt, only accepts text-based subtitle streams.   
So, even though the first subtitle stream available belongs to C.mkv, it is image-based and hence skipped.    
The selected stream, stream 2 in B.mp4, is the first text-based subtitle stream.   

Example: labeled filtergraph outputs
```
ffmpeg -i A.avi -i B.mp4 -i C.mkv -filter_complex "[1:v]hue=s=0[outv];overlay;aresample" \
       -map '[outv]' -an        out1.mp4 \
                                out2.mkv \
       -map '[outv]' -map 1:a:0 out3.mkv
```
The above command will fail, as the output pad labelled [outv] has been mapped twice. None of the output files shall be processed.

```
ffmpeg -i A.avi -i B.mp4 -i C.mkv -filter_complex "[1:v]hue=s=0[outv];overlay;aresample" \
       -an        out1.mp4 \
                  out2.mkv \
       -map 1:a:0 out3.mkv
```
This command above will also fail as the hue filter output has a label, [outv], and hasn’t been mapped anywhere.

The command should be modified as follows,

```
ffmpeg -i A.avi -i B.mp4 -i C.mkv -filter_complex "[1:v]hue=s=0,split=2[outv1][outv2];overlay;aresample" \
        -map '[outv1]' -an        out1.mp4 \
                                  out2.mkv \
        -map '[outv2]' -map 1:a:0 out3.mkv
```
The video stream from B.mp4 is sent to the hue filter, whose output is cloned once using the split filter, and both outputs labelled.    
Then a copy each is mapped to the first and third output files.   

The overlay filter, requiring two video inputs, uses the first two unused video streams. Those are the streams from A.avi and C.mkv.   
The overlay output isn’t labelled, so it is sent to the first output file out1.mp4, regardless of the presence of the -map option.   

The aresample filter is sent the first unused audio stream, that of A.avi.     
Since this filter output is also unlabelled, it too is mapped to the first output file.    
The presence of -an only suppresses automatic or manual stream selection of audio streams, not outputs sent from filtergraphs.    
Both these mapped streams shall be ordered before the mapped stream in out1.mp4.

The video, audio and subtitle streams mapped to out2.mkv are entirely determined by automatic stream selection.   

out3.mkv consists of the cloned video output from the hue filter and the first audio stream from B.mp4.   
