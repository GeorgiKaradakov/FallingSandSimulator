# Falling Sand Simulator

## Description

This is a very simple falling sand simulator created using [SDL2](https://www.libsdl.org/). The simulator features sand particles falling in a variety of rainbow colors. Current functionalities include the basic mechanics of sand movement. 

### Key Features

- **Rainbow Colored Sand:** Each grain of sand is rendered in a distinct color from a rainbow palette, creating a visually striking effect as they cascade down the screen.
- **SDL2 Framework:** Utilizing the powerful SDL2 library, the simulator ensures smooth and efficient rendering, providing an engaging user experience.
- **Clearing Mechanism:** The ability to clear the window of sand by pressing SPACE, allowing users to reset or clean their canvas as they experiment with different patterns and behaviors.

### How to Compile and Run

To compile and run it run this command in you cmd "g++ main.cpp -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -o falling_sand && ./falling_sand"

### Dependencies

- **SDL2:** This project is built using the SDL2 library. Make sure SDL2 is installed and properly configured in your development environment.
- **MinGW:** This project uses "g++" to compile.
