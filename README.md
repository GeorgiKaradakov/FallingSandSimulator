# Falling Sand Simulator

## Description

This is a falling sand simulator created using [SDL2](https://www.libsdl.org/). The simulator features sand particles falling in a variety of rainbow colors. Current functionalities include the basic mechanics of sand movement. Future updates will introduce the ability to clear the window of sand and implement varying speeds for the falling sand particles.

### Key Features

- **Rainbow Colored Sand:** Each grain of sand is rendered in a distinct color from a rainbow palette, creating a visually striking effect as they cascade down the screen.
- **SDL2 Framework:** Utilizing the powerful SDL2 library, the simulator ensures smooth and efficient rendering, providing an engaging user experience.

### Upcoming Features

- **Clearing Mechanism:** A future update will add the ability to clear the window of sand, allowing users to reset or clean their canvas as they experiment with different patterns and behaviors.
- **Variable Speeds:** To enhance realism and visual interest, sand particles will eventually fall at different speeds, simulating a more natural and diverse falling effect.

### How to Compile and Run

To compile and run it run this command in you cmd "g++ main.cpp -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -o falling_sand && ./falling_sand"

### Dependencies

- **SDL2:** This project is built using the SDL2 library. Make sure SDL2 is installed and properly configured in your development environment.
- **MinGW** This project uses "g++" to compile.
