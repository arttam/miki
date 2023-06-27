# FFmpeg definitions

[Source](https://github.com/leandromoreira/ffmpeg-libav-tutorial/tree/db409110471aec8310c0bbdd6282fcb0892ff616)

## PTS - Presentation timestamp
When we're designing a video player we need to play each frame at a given pace,   
otherwise it would be hard to pleasantly see the video either because it's playing so fast or so slow.

Therefore we need to introduce some logic to play each frame smoothly.   
For that matter, each frame has a presentation timestamp (PTS)   
which is an increasing number factored in a timebase that is a rational number   
(where the denominator is known as timescale) divisible by the frame rate (fps).



It's easier to understand when we look at some examples,   
let's simulate some scenarios.

For a fps=60/1 and timebase=1/60000   
each PTS will increase timescale / fps = 1000   
therefore the PTS real time for each frame could be (supposing it started at 0):

- frame=0, PTS = 0, PTS_TIME = 0
- frame=1, PTS = 1000, PTS_TIME = PTS * timebase = 0.016
- frame=2, PTS = 2000, PTS_TIME = PTS * timebase = 0.033

For almost the same scenario but with a timebase equal to 1/60.
- frame=0, PTS = 0, PTS_TIME = 0
- frame=1, PTS = 1, PTS_TIME = PTS * timebase = 0.016
- frame=2, PTS = 2, PTS_TIME = PTS * timebase = 0.033
- frame=3, PTS = 3, PTS_TIME = PTS * timebase = 0.050

For a fps=25/1 and timebase=1/75 each PTS will increase timescale / fps = 3 and the PTS time could be:
- frame=0, PTS = 0, PTS_TIME = 0
- frame=1, PTS = 3, PTS_TIME = PTS * timebase = 0.04
- frame=2, PTS = 6, PTS_TIME = PTS * timebase = 0.08
- frame=3, PTS = 9, PTS_TIME = PTS * timebase = 0.12
- ...
- frame=24, PTS = 72, PTS_TIME = PTS * timebase = 0.96
- ...
- frame=4064, PTS = 12192, PTS_TIME = PTS * timebase = 162.56

Now with the pts_time we can find a way to render this synched with audio pts_time or with a system clock.   
The FFmpeg libav provides these info through its API:

```
fps = AVStream->avg_frame_rate
tbr = AVStream->r_frame_rate
tbn = AVStream->time_base
```

## Filtering flow

The general usage of FFmpeg or the libav follows a pattern/architecture or workflow:

- protocol layer - it accepts an input (a file for instance but it could be a rtmp or HTTP input as well)
- format layer - it demuxes its content, revealing mostly metadata and its streams
- codec layer - it decodes its compressed streams data 
- pixel layer - it can also apply some filters to the raw frames (like resizing)
- and then it does the reverse path
- codec layer - it encodes (or re-encodes or even transcodes) the raw frames
- format layer - it muxes (or remuxes) the raw streams (the compressed data)
- protocol layer - and finally the muxed data is sent to an output (another file or maybe a network remote server)

In case of transmuxing (changing container format - codel, pixel and codec again - are not needed


![Filtering flow](../../images/ffmpeg_libav_workflow.jpeg "Filtering flow")
