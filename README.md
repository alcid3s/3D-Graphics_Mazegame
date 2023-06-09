# OpenGL Maze Game

## Description
This project is an interactive maze game developed using OpenGL and C++. In this game, players spawn inside a maze and their objective is to navigate through the maze to reach the altar, where they must open a book to advance to the next level. However, there's a twist - a relentless monster is also roaming the maze, actively searching for the player.

## Features
- First-person perspective maze exploration.
- Randomly generated maze layout for each level.
- Real-time lighting and shading effects using OpenGL.
- Sound effects and background music to enhance the gaming experience.
- AI-controlled monster that actively hunts the player.
- Multiple levels with increasing difficulty.
- Player movement controlled by keyboard input.

## Installation
To run this project on your local machine, follow these steps:

1. Clone the repository to your local machine or download the project files.
```bash
    git clone https://github.com/alcid3s/3D-Graphics_Mazegame.git
```

2. Navigate to the project directory.
```bash
    cd 3D-Graphics_Mazegame
```
3. Copy or move the files from the sfml_libs directory into the x64/Debug directory of the project
``` bash
    mv sfml_libs/* x64/Debug/ && rmdir sfml_libs
```
4. Compile the project using your preferred C++ compiler, ensuring that you have the necessary dependencies installed.
5. Run the compiled executable to start the game.

## Usage
- Use the arrow keys or WASD keys to move the player within the maze.
- Navigate through the maze and locate the altar.
- Once you reach the altar, press the designated key to open the book and proceed to the next level.
- Be cautious of the monster, as it will actively try to find and catch you.
- If the monster catches you, the game ends and you can choose to play again.

## Dependencies
This project relies on the following dependencies:
- OpenGL
- C++
- SFML (Simple and Fast Multimedia Library)

Make sure you have these dependencies installed before attempting to compile and run the game.
