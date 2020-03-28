### Little Player v0.2.0
---


#### Goal
Project for learning FFmpeg with C++.

#### Feature
Read file and play. 

#### Run
1. compile code to littlePlayer.exe
1. run: ./littlePlayer.exe /path/to/target/xxx.mp4


#### for test
- main.cpp Ϊ���;
- �޸�main�����е�ע��
- testPlayVideo ��������Ƶ�������Լ���·�����޸�inputPath
- testPlayAudio ��������Ƶ�������Լ���·�����޸�inputPath



#### Constraint 
1. FFmpeg latest API
1. Easy API
1. SDL2
1. C++11  
1. Video only
1. CMake
1. Audio some day?


#### Steps
- [X] Read media file and transfer it to pixel format file.
- [X] Play pixel format file using ffplay.
- [X] Read pixel format file and draw data on SDL.
- [X] Read media file and draw picture using SDL.
- [X] Read mp3 file and play it.
- [X] Play video with audio.
- [ ] play file with audio only or video only.


