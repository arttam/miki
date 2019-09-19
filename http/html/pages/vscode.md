# Transversal Programming Test
## Component
Component code located in **src** folder.
In order to build and run test run following command
```sh
make run
```
Various command combinations can be tested as follows:
```sh
cat test/TestInput.txt| ./bin/test <option1> <option2> ...
```

I'll be more than happy to discuss possible extension of implementation during face-to-face interview

## Sketch
- After reading entire log, I would create ```map``` of uninterrupted sequence of consequtive actions agains website.
- Map would have uninterrupted sequence as key and website as value.
- Then I would use ```unique_copy``` to get ```vector``` of unique uninterrupted sequences.
- Using unique uniterrupted sequences - I would create output in form of multimap having:
  - sequence as key
  - pair of website and count of sequence as value
- Sort output by count of sequence
- Output first <num> results
    
