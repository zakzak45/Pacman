
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Screen
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 32;
const int FPS = 60;

// Map
vector<string> gameMap = {
    "#################",
    "#.......#.......#",
    "#.##.###.###.##.#",
    "#.##.....P.##.##.#",
    "#......#####.....#",
    "#.###.#.....#.###.#",
    "#...#.#.###.#...#.#",
    "#.##.#.#...#.##.#.#",
    "#.#.....#.#.....#.#",
    "#.#.###.#.###.#.#.#",
    "#.#.#...#...#.#.#.#",
    "#.#.#.#######.#.#.#",
    "#...#.........#...#",
    "#################"
};

// Game states
struct Ghost {
    int x, y;
    int dx, dy;
    int moveCounter;
};

int px = 1, py = 1;
int nextPx = 1, nextPy = 1;
int score = 0;
int lives = 3;
int dotsRemaining = 0;
bool running = true;
bool gameOver = false;

Ghost ghosts[4];
const int NUM_GHOSTS = 4;

// SDL objects
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Initialize SDL
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow(
        "Pac-Man Prototype",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialize ghosts
    ghosts[0] = {7, 6, 1, 0, 0};
    ghosts[1] = {8, 6, -1, 0, 0};
    ghosts[2] = {7, 7, 0, 1, 0};
    ghosts[3] = {8, 7, 0, -1, 0};

    // Count remaining dots
    for (int y = 0; y < gameMap.size(); y++) {
        for (int x = 0; x < gameMap[y].size(); x++) {
            if (gameMap[y][x] == '.') dotsRemaining++;
        }
    }

    return window && renderer;
}

// Ghost AI movement
void updateGhosts() {
    for (int i = 0; i < NUM_GHOSTS; i++) {
        ghosts[i].moveCounter++;
        if (ghosts[i].moveCounter >= 30) {
            ghosts[i].moveCounter = 0;

            int nx = ghosts[i].x + ghosts[i].dx;
            int ny = ghosts[i].y + ghosts[i].dy;

            // Check if ghost can move in current direction
            if (nx < 0 || nx >= gameMap[0].size() || ny < 0 || ny >= gameMap.size() || gameMap[ny][nx] == '#') {
                // Random new direction
                int randDir = rand() % 4;
                ghosts[i].dx = (randDir == 0) ? 1 : (randDir == 1) ? -1 : 0;
                ghosts[i].dy = (randDir == 2) ? 1 : (randDir == 3) ? -1 : 0;
            } else {
                ghosts[i].x = nx;
                ghosts[i].y = ny;
            }
        }
    }
}

// Input
void handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running = false;

        if (event.type == SDL_KEYDOWN) {
            int nx = px;
            int ny = py;

            switch (event.key.keysym.sym) {
                case SDLK_w: ny--; break;
                case SDLK_s: ny++; break;
                case SDLK_a: nx--; break;
                case SDLK_d: nx++; break;
                case SDLK_ESCAPE: running = false;
            }

            if (gameMap[ny][nx] != '#') {
                px = nx;
                py = ny;
            }
        }
    }
}

// Update
void update() {
    if (gameMap[py][px] == '.') {
        gameMap[py][px] = ' ';
        score += 10;
        dotsRemaining--;
    }

    // Check collision with ghosts
    for (int i = 0; i < NUM_GHOSTS; i++) {
        if (ghosts[i].x == px && ghosts[i].y == py) {
            lives--;
            if (lives <= 0) {
                gameOver = true;
                running = false;
            } else {
                px = 1;
                py = 1;
            }
        }
    }

    // Check win condition
    if (dotsRemaining <= 0) {
        cout << "You Won! Final Score: " << score << endl;
        running = false;
    }

    updateGhosts();
}

// Render
void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < gameMap.size(); y++) {
        for (int x = 0; x < gameMap[y].size(); x++) {
            SDL_Rect tile = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            if (gameMap[y][x] == '#') {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderFillRect(renderer, &tile);
            }

            if (gameMap[y][x] == '.') {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect dot = {
                    tile.x + TILE_SIZE / 3,
                    tile.y + TILE_SIZE / 3,
                    TILE_SIZE / 3,
                    TILE_SIZE / 3
                };
                SDL_RenderFillRect(renderer, &dot);
            }
        }
    }

    // Pac-Man
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect pacman = { px * TILE_SIZE, py * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_RenderFillRect(renderer, &pacman);

    // Draw ghosts
    for (int i = 0; i < NUM_GHOSTS; i++) {
        SDL_SetRenderDrawColor(renderer, 255, 100 + (i * 50), 100 + (i * 50), 255);
        SDL_Rect ghost = { ghosts[i].x * TILE_SIZE, ghosts[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        SDL_RenderFillRect(renderer, &ghost);
    }

    SDL_RenderPresent(renderer);
}

// Cleanup
void cleanUp() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Main Loop
int main() {
    if (!initSDL()) return -1;

    const int frameDelay = 1000 / FPS;

    while (running) {
        Uint32 frameStart = SDL_GetTicks();

        handleInput();
        update();
        render();

        int frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    cout << "Final Score: " << score << endl;
    cleanUp();
    return 0;
}
