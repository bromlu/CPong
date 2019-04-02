# CPong

A circular pong game written in C++ using SFML.

## Install

You will need to install SFML. You can see instructions on how to do that on their 
[official website](https://www.sfml-dev.org/download.php).

I also did not upload any of the assets (audio files and a font) to the git repo. So in order to run the game you
either have to find your own, or remove them from the code. Here is a list of the assets:

- assets/ConnectionII.otf      
- assets/Off_Limits.wav
- assets/sfx_alarm_loop8.wav
- assets/sfx_damage_hit1.wav
- assets/sfx_menu_move1.wav
- assets/sfx_menu_select1.wav
- assets/sfx_sounds_error7.wav

Once you have done the above, run make to compile the game.

```bash
$ make
g++  -o pong pong.cpp -stdlib=libc++ -lsfml-graphics -lsfml-window -lsfml-system -I/usr/local/Cellar/sfml/2.4.2_1/lib/ -lsfml-audio -lsfml-network
```

## Run

Just run the binary.

```
$ ./pong
```
