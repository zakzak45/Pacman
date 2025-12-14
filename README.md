# Pac-Man Game

A Pac-Man game in C++ using SDL2.

## Install SDL2

```bash
sudo apt-get install libsdl2-dev  # Ubuntu/Debian
brew install sdl2                  # macOS
```

Windows: Download SDL2 from https://www.libsdl.org/download-2.0.php

## Compile

```bash
cd src
g++ main.cpp -o pacman -lSDL2
```

## Run

**Linux/macOS:**
```bash
./pacman
```

**Windows:**
```bash
pacman.exe
```

## Controls

- **W** - Up
- **S** - Down
- **A** - Left
- **D** - Right
- **ESC** - Quit

## How to Play

1. Collect all white dots
2. Avoid 4 ghosts
3. You have 3 lives
4. Get 10 points per dot
5. Win by collecting all dots
