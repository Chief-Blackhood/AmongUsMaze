## 2D Maze game using OpenGL 

This project tries to showcase how to make 2D game in openGL.
The game is inspired from the famous game Among Us.
You are a crew mate who has to complete 2 tasks before the time runs out.
The imposter will follow you and will kill you as soon as he touches you.
But you have the advantage of being faster than the imposter.

## How to Run

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./Hello-World
```

## Key Bindings

The keys to move the player around are:
- <kbd>←</kbd>: To move left
- <kbd>→</kbd>: To move right
- <kbd>↑</kbd>: To move up
- <kbd>↓</kbd>: To move down
- <kbd>L</kbd>: To toggle central light system 
- <kbd>Q</kbd>: To stop game and display score
- <kbd>esc</kbd>: To kill the window

## Score

- The score increases by 10 whenever you collect coins.
- The score decreases by 10 whenever you touch a bomb.
- The score increases by 1 per second if you have the lights on.
- The score increases by 3 per second if you have the lights off.

## Winning/Losing

- To win the game you have to complete both the tasks (touch both the buttons) and then the closed gate will disappear and you can exit.
- If the imposter touches you or time runs out then game over! 