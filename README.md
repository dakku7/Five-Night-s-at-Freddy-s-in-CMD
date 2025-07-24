# Five-Nights-at-Freddys-in-CMD

This project is a console-based adaptation of the Fnaf 1 from scratch, by me. The implementation is done in C++.

## Code 
- **Main Functionality**: The `main.cpp` file contains the main game loop, where threads for animatronic movement, energy management, and rendering are created and managed.
- **Game Logic**: The `func.h` and `func.cpp` files define the game classes, including `Map`, `Player`, and `Animatronic`.
- **Animatronic Behavior**: Each animatronic class (e.g., `Freddy`, `Chika`, `Bonny`, `Foxy`) inherits from the base `Animatronic` class.

## My thoughts
This is my very first relatively large project that I wrote in C++. I wanted to write something that interested me, and I chose Fnaf because I was very interested in it as a child. 


# How to Run the Game (For Beginners)

## Simple Compilation on Windows

1. Install **Code::Blocks** (a free C++ IDE).
2. Download the project files (`main.cpp`, `func.h`, `func.cpp`).
3. Open Code::Blocks and create a new project.
4. Add the downloaded files to the project.
5. Click the **Build and Run** button (or press F9).

## Simple Compilation on Linux/macOS

1. Open the terminal.
2. Make sure you have `g++` installed (type `g++ --version`).
3. If it's not installed, use the following command:
   ```bash
   sudo apt install g++  # for Linux
   brew install gcc      # for macOS

   
4. Navigate to the folder with the project files.
5. Enter the command to compile:
    ```
   g++ main.cpp func.cpp -o fnaf
   
   ```
6. Run the game:
    ```
   ./fnaf
   
   ```

## P.S
If anyone sees this, I would appreciate any criticism or simply your opinion (I have only just started learning C++ and this is my first language).

<img width="1200" height="675" alt="441" src="https://github.com/user-attachments/assets/66b102cf-309a-4678-a457-814bdb401adb" />
I’m not the owner of this digital art.
