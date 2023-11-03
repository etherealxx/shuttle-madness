#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <filesystem>
#include "shuttle_extras.h"
#include "shuttle_cocks.h"
#include "shuttle_icon.h"
#include "shuttle_sound.h"
#include "shuttle_debug.h"
#include "shuttle_image.h"
#include "shuttle_explosion.h"

// Define boolean flags to track key states
bool keyLeftPressed = false;
bool keyRightPressed = false;

int jumpCount = 0; // Initialize the jump count to 0

// Global variable to track whether the 'S' key is currently held down
bool keySPressed = false;

// Game variables
float playerX = 0.0f;
float playerY = 0.0f;
float playerVelocityX = 0.0f;
float playerVelocityY = 0.0f;
float playerSizeY = 1.0f;

// bool ducking = false;
bool isDucking = false;
bool isGrounded = true;
bool isFastFalling = false;

bool releasedS = false;
int extraJumpTimer = 0;
float extraJumpHeight = 0.0f;

bool isSwingingRacket = false;
int racketTimer = 0;
int racketCooldownTimer = 0;

// float shuttleCockSpeed = 0.07f;
// float shuttleCockX = 10.0f;

float racketCenterX = playerX + 2.2f;
float racketCenterY = playerY + 0.5f;
float racketRadiusX = 0.5f;
float racketRadiusY = 0.3f;
int racketHitLimit = 2;

int lives = 3;
int damageInvincibilityTimer = 0;
int flickeringTimer = 0;
int visibleFrameDuringFlicker = 4;
float shuttleCockOutofBoundLimit = 15.0f;

Sound mainThemeSound("maintheme_20.wav", IS_LOOPING);
Sound jumpSound("jump.wav", INIT_ONLY);
Sound racketSwingSound("racketswing.wav", INIT_ONLY);
Sound racketHitSound("rackethit.wav", INIT_ONLY);
Sound damagedSound("damaged.wav", INIT_ONLY);
Sound clankSound("clank_50.wav", INIT_ONLY);
Sound explosionSound("explosion_50.wav", INIT_ONLY);
std::string scriptDir;

std::vector<Shuttlecock> shuttlecocks;
std::vector<std::string> textLines;

Image shuttleKnight("shuttle_knight.png");
Image shuttleKnightDuck("shuttle_knight_duck.png");

float shuttlecockSpawnTimer = 0.0f;
float shuttlecockTimeToSpawn = 3.0f;
int debugTimer = 0;
int secondPassed = 0;

bool isKnightLoaded = shuttleKnight.isLoaded();
int score = 0;

// void printAudioFilePaths() { //for testing
//     for (Sound* sound : Sound::everySound) {
//         string audioFilePath = scriptDir + "\\" + sound->audioFilename;  // Construct the relative path
//         writeDebugText(audioFilePath);
//     }
// }

PlayerHitbox playerHitbox(playerX, playerY, playerSizeY, isKnightLoaded);

void keyboardDown(unsigned char key, int x, int y) {
    if (key == 'd') {
        keyRightPressed = true;
        // Adjust the horizontal velocity when ducking
        playerVelocityX = isDucking ? 0.05f : 0.1f;
    }
    else if (key == 'a') {
        keyLeftPressed = true;
        // Adjust the horizontal velocity when ducking
        playerVelocityX = isDucking ? -0.05f : -0.1f;
    }
    else if (key == 'w') {

        racketCenterY = playerY + 0.5;

        if (jumpCount < 2 && !isDucking) {
            if (extraJumpTimer > 0) {
                // Apply extra jump height when 'W' is pressed and 'S' was recently released.
                extraJumpHeight = 0.05f; // Adjust the extra jump height as needed.
                // releasedS = false; // Reset the flag.
                extraJumpTimer = 0; // Reset the timer.
            }
            // Start jumping if 'W' is pressed, the jump count is less than 2,
            // the player is grounded, and the player is not ducking
            jumpCount++;
            playerVelocityY = 0.2f + extraJumpHeight;
            isGrounded = false;
            playAudioOnClick(jumpSound);
        }

    }
    else if (key == 's') {

        racketCenterY = playerY + 0.5;

        if (isGrounded) {
            // Start ducking if 'S' is pressed, and the player is grounded
            isDucking = true;
            playerSizeY = 0.5f;
            if (keyRightPressed) playerVelocityX = 0.05f;
            if (keyLeftPressed) playerVelocityX = -0.05f;
        }
        else {
            if (!isFastFalling) {
                playerVelocityY -= 0.2f; // You can adjust this value for the desired falling speed
                isFastFalling = true;
            }

        }
    }
    else if (key == ' ') { // spacebar
        if (racketTimer == 0 && racketCooldownTimer == 0) {
            racketTimer = 10;
            racketCooldownTimer = 20;
            playAudioOnClick(racketSwingSound);
        }
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    if (key == 'd') {
        keyRightPressed = false;
        if (!keyLeftPressed) {
            playerVelocityX = 0.0f;
        }
    }
    else if (key == 'a') {
        keyLeftPressed = false;
        if (!keyRightPressed) {
            playerVelocityX = 0.0f;
        }
    }
    else if (key == 's') {
        if (isGrounded) {
            // Stop ducking if 'S' is released, and the playenr is grounded
            isDucking = false;
            playerSizeY = 1.0f;
            if (keyRightPressed) playerVelocityX = 0.1f;
            if (keyLeftPressed) playerVelocityX = -0.1f;
            // // Indicate that 'S' was recently released and start the timer.
            // releasedS = true;
            extraJumpTimer = 10; // Set the timer duration (adjust as needed).
        }
        else {}

    }
}

// Function to update the game state
void update(int value) {
    // Update player position based on velocity
    playerX += playerVelocityX;
    playerY += playerVelocityY;

    // Apply gravity to the player (simulate falling)
    playerVelocityY -= 0.01f;

    // Keep the player within the screen bounds
    if (playerX < -0.0f) playerX = -0.0f;
    if (playerX > 9.0f) playerX = 9.0f;

    if (playerY < 0.0f) {
        playerY = 0.0f;
        playerVelocityY = 0.0f; // Stop falling
        jumpCount = 0;
        isGrounded = true;
        isFastFalling = false;
    }

    // Decreasing timer
    if (extraJumpTimer > 0) extraJumpTimer--;
    if (racketTimer > 0) racketTimer--;
    if (racketCooldownTimer > 0) racketCooldownTimer--;
    if (damageInvincibilityTimer > 0) {
        damageInvincibilityTimer--;
        if (flickeringTimer > 0) {
            flickeringTimer--;
        }
        else { flickeringTimer = 7; }
    }
    else { flickeringTimer = 0; }

    if (debugTimer > 0) {
        debugTimer--;
    }
    else {
        debugTimer = 60;
        secondPassed++;
        // writeDebugText("Second passed: " + std::to_string(secondPassed));
        // writeDebugText("player position: " + std::to_string(playerX) + ", "
        //     + std::to_string(playerY));
    }

    if (shuttlecockSpawnTimer > 0.0f) {
        shuttlecockSpawnTimer -= 0.1f;
    }
    else {
        shuttlecockSpawnTimer = shuttlecockTimeToSpawn * 6.0f;
        if (shuttlecockTimeToSpawn > 1.5f) shuttlecockTimeToSpawn -= 0.04f;
        else if (shuttlecockTimeToSpawn > 0.7f) shuttlecockTimeToSpawn -= 0.01f;

        // writeDebugText("shuttlecockTimeToSpawn: " + std::to_string(shuttlecockTimeToSpawn));

        // int randomBlack = rand() % 5; // blacktest (0 to 4)
        // if (randomBlack == 0) spawnShuttlecock(SC_BLACK);

        // type of shuttlecock spawned handled here
        int randomBlack = rand() % 11; // blacktest (0 to 10)
        if (randomBlack < 2) spawnShuttlecock(SC_YELLOW);
        else if (randomBlack > 1 && randomBlack < 4) spawnShuttlecock(SC_BLACK);
        else spawnShuttlecock();

    }

    playerHitbox.update();

    for (Shuttlecock& shuttlecock : shuttlecocks) {
        shuttlecock.update();

        // Check for collision with the player's hitbox
        if (playerHitbox.collidesWith(shuttlecock) && damageInvincibilityTimer == 0) {
            shuttlecock.checkExplode(); // if yellow

            // Destroy the shuttlecock by removing it from the vector
            shuttlecock.toRemove = true;
            damageInvincibilityTimer = 120;
            lives--;
            playAudioOnClick(damagedSound);
        }

        // Check if a shuttlecock is out of bounds
        if (shuttlecock.isOutOfBounds()) {
            shuttlecock.toRemove = true;
        }

    }

    shuttlecocks.erase(std::remove_if(shuttlecocks.begin(), shuttlecocks.end(), [](const Shuttlecock& s) { return s.toRemove; }), shuttlecocks.end());

    // Reset the extra jump height when the timer expires.
    if (extraJumpTimer == 0) extraJumpHeight = 0.0f;

    // True when the timer isn't zero
    isSwingingRacket = racketTimer > 0;
    if (racketTimer == 0) {
        racketHitLimit = 2;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 60 FPS
}

// Function to draw the game scene
void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    // drawPlayerSquare(playerX, playerY, playerSizeY); // Draw the player character

    if (flickeringTimer < visibleFrameDuringFlicker) {
        if (isKnightLoaded) {
            if (!isDucking) shuttleKnight.draw(playerX, playerY);
            else shuttleKnightDuck.draw(playerX, playerY);
        }
        else { drawPlayerCharacter(playerX, playerY, playerSizeY, isDucking); } // Fallback when knight isn't loaded
        // drawPlayerCharacter(playerX, playerY, playerSizeY, isDucking); // for debugging
    }
    // drawPlayerHitbox(playerX, playerY, playerSizeY, true); // Debugging hitbox
    // playerHitbox.drawDummy(); // Debugging hitbox
    float racketColor[3] = { 1.0, 1.0, 1.0 }; // Red color (RGB)
    if (isSwingingRacket) {
        drawQuad(playerX + 1.35, playerY + 0.45, 0.7, 0.05, racketColor);
        drawHollowEllipse(playerX + 2.2, playerY + 0.5, 0.5, 0.3, 4, racketColor);
        drawRacketNet(playerX + 2.2, playerY + 0.5, 0.5, 0.3, 4, racketColor, 6);
    }
    else { // folded racket
        drawRotatedQuad(playerX + 1.09, playerY + 0.8, 0.7, 0.05, 80, racketColor);
        drawRotatedHollowEllipse(playerX + 1.2, playerY + 1.6, 0.5, 0.1, 4, 80, racketColor);
    }
    // drawEllipse(playerX + 2.2, playerY + 0.5, 0.5, 0.3, circleColor);

    for (const Shuttlecock& shuttlecock : shuttlecocks) {
        shuttlecock.draw(); // Draw the shuttlecocks
    }

    drawExplosion();

    if (lives > 0) {
        for (int i = 0; i < lives; i++) {
            drawHeart(i * 0.7f + 0.1f, 5.5f, 0.6f);
        }
    }

    // drawDebugText(3.0f, 5.8f);
    drawText(0.1f, 5.3f, "Score: " + std::to_string(score));

    glutSwapBuffers();
}

// void idle() {
//     playMainTheme();
// }

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shuttle Madness");

    // Seed the random number generator with the current time
    srand(static_cast<unsigned>(time(nullptr)));

    glOrtho(0.0, 10.0, 0.0, 6.0, -1.0, 1.0);

    glutDisplayFunc(display);
    // glutKeyboardFunc(keyboard);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    // glutIdleFunc(idle);
    glutTimerFunc(0, update, 0);

    // Make icon for the game
    unsigned char* icon_data = GenerateIconData();
    HICON hIcon = CreateIconFromData(icon_data, 32, 32);

    // Set the window icon using the Windows API
    if (hIcon) {
        SendMessage(GetActiveWindow(), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(GetActiveWindow(), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }

    delete[] icon_data; // deallocate memory for icon

    // initializeShuttlecocks();

    scriptDir = std::filesystem::path(argv[0]).parent_path().string();
    // writeDebugText("scriptDir: " + scriptDir);

    // printAudioFilePaths();

    initializeEveryAudio();

    glutMainLoop();

    uninitEveryAudio();

    return 0;
}
