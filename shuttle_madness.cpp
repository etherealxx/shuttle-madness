// 2023 Jibril Wathon
// github.com/etherealxx
// Public Domain

#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <filesystem>
#include "shuttle_extras.h"
#include "shuttle_cocks.h"
#include "shuttle_icon.h"
#include "shuttle_sound.h"
#include "shuttle_image.h"
#include "shuttle_explosion.h"
#include "shuttle_menu.h"
// #include "shuttle_debug.h"

State gameState = Menu;

// Game variables, sorted by the moment it was being added
// Except images and sounds as they're grouped together
bool keyLeftPressed = false;
bool keyRightPressed = false;
int jumpCount = 0;

float playerX = 0.0f;
float playerY = 0.0f;
float playerVelocityX = 0.0f;
float playerVelocityY = 0.0f;
float playerSizeY = 1.0f;

bool isDucking = false;
bool isGrounded = true;
bool isFastFalling = false;

int extraJumpTimer = 0;
float extraJumpHeight = 0.0f;

bool isSwingingRacket = false;
int racketTimer = 0;
int racketCooldownTimer = 0;

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

std::string scriptDir;
Sound mainThemeSound("maintheme_20.wav", INIT_ONLY | IS_LOOPING);
Sound jumpSound("jump.wav", INIT_ONLY);
Sound racketSwingSound("racketswing.wav", INIT_ONLY);
Sound racketHitSound("rackethit.wav", INIT_ONLY);
Sound damagedSound("damaged.wav", INIT_ONLY);
Sound clankSound("clank_50.wav", INIT_ONLY);
Sound explosionSound("explosion_50.wav", INIT_ONLY);
Sound gameOverSound("gameover.wav", INIT_ONLY);
Sound pauseSound("pause_60.wav", INIT_ONLY);
Sound menuThemeSound("menutheme_70.wav", IS_LOOPING);
Sound menuHoverSound("menuhover_amp.wav", INIT_ONLY);
Sound menuClickSound("menuclick_amp.wav", INIT_ONLY);

std::vector<Shuttlecock> shuttlecocks;
std::vector<std::string> textLines;

Image shuttleKnight("shuttle_knight.png");
Image shuttleKnightDuck("shuttle_knight_duck.png");
Image shuttleKnightDefeat("shuttle_knight_defeat.png");
Image shuttleMenuBackground("shuttle_menu_background_2.png");

float shuttlecockSpawnTimer = 0.0f;
float shuttlecockTimeToSpawn = 3.0f;
int debugTimer = 0;
int secondPassed = 0;

bool isKnightLoaded = shuttleKnight.isLoaded();
int score = 0;
bool isPlaying = true;
bool deadTrigger = false;
int highScore = 0;

bool isPaused = false;

Title menuItem({ "Play","How to Play","Options","Credit", "Quit" });
Title optionsItem({ "[v] Music", "[v] Sound Effects" }, 0,
    { "[  ] Music", "[  ] Sound Effects" });

bool checkmutedsfx = false;
bool checkmutedmusic = false;

std::vector<std::string> creditbefore = { "Game made by etherealxx","With occasional help from ChatGPT","" };
std::vector<std::string> creditafter = { "","shuttle_knight.png by Ikhsan Ridwan","",
"Additional credits and information can be read at https://github.com/etherealxx/shuttle-madness" };
int creditpage = 1;

// float pinkColor[3] = { 1.0f, 0.0f, 0.5f };
PlayerHitbox playerHitbox(playerX, playerY, playerSizeY, isKnightLoaded);

void resetValues() {
    keyLeftPressed = false;
    keyRightPressed = false;
    jumpCount = 0;
    playerX = 0.0f;
    playerY = 0.0f;
    playerVelocityX = 0.0f;
    playerVelocityY = 0.0f;
    playerSizeY = 1.0f;
    isDucking = false;
    isGrounded = true;
    isFastFalling = false;
    extraJumpTimer = 0;
    extraJumpHeight = 0.0f;
    isSwingingRacket = false;
    racketTimer = 0;
    racketCooldownTimer = 0;
    racketCenterX = playerX + 2.2f;
    racketCenterY = playerY + 0.5f;
    racketRadiusX = 0.5f;
    racketRadiusY = 0.3f;
    racketHitLimit = 2;
    damageInvincibilityTimer = 0;
    flickeringTimer = 0;
    visibleFrameDuringFlicker = 4;
    shuttleCockOutofBoundLimit = 15.0f;
    shuttlecockSpawnTimer = 0.0f;
    shuttlecockTimeToSpawn = 3.0f;
    debugTimer = 0;
    secondPassed = 0;
    isKnightLoaded = shuttleKnight.isLoaded();
    score = 0;
    isPaused = false;
}

// void printAudioFilePaths() { // for testing
//     for (Sound* sound : Sound::everySound) {
//         string audioFilePath = scriptDir + "\\" + sound->audioFilename;  // Construct the relative path
//         writeDebugText(audioFilePath);
//     }
// }

void gameOverTrigger() {
    if (!deadTrigger) {
        deadTrigger = true;
        isPlaying = false;
        stopLoop(mainThemeSound);
        playerVelocityX = 0;
        playerVelocityY = 0;
        playAudioOnClick(gameOverSound, checkmutedsfx);
        if (score > highScore) highScore = score;
    }
}

void resetGame() {
    lives = 3;
    isPlaying = true;
    deadTrigger = false;
    resetValues();
    restoreLoop(mainThemeSound);
    for (Shuttlecock& shuttlecock : shuttlecocks) {
        shuttlecock.toRemove = true; // Draw the shuttlecocks
    }
}

void playingKeyboardDown(unsigned char& key) {
    if (isPlaying) {
        if (!isPaused) {
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
                        extraJumpTimer = 0; // Reset the timer.
                    }
                    // Start jumping if 'W' is pressed, the jump count is less than 2,
                    // the player is grounded, and the player is not ducking
                    jumpCount++;
                    playerVelocityY = 0.2f + extraJumpHeight;
                    isGrounded = false;
                    playAudioOnClick(jumpSound, checkmutedsfx);
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
                        playerVelocityY -= 0.2f;
                        isFastFalling = true;
                    }

                }
            }
            else if (key == ' ') { // spacebar
                if (racketTimer == 0 && racketCooldownTimer == 0) {
                    racketTimer = 10;
                    racketCooldownTimer = 20;
                    playAudioOnClick(racketSwingSound, checkmutedsfx);
                }
            }
        }
        else { // game paused
            if (key == 8) { // backspace
                playAudioOnClick(menuClickSound, checkmutedsfx);
                gameState = Menu;
                if (menuThemeSound.isStopped() && !checkmutedmusic) restoreLoop(menuThemeSound);
            }
            if (key == 127) resetGame(); // Delete button
        }

        if (key == 27) { // esc
            isPaused = !isPaused; // toggle/trigger pausing
            if (isPaused) playAudioOnClick(pauseSound, checkmutedsfx);
            else restoreLoop(mainThemeSound, "resume");
        }
    }
    else {
        if (key == 8) { // backspace
            playAudioOnClick(menuClickSound, checkmutedsfx);
            gameState = Menu;
            if (menuThemeSound.isStopped() && !checkmutedmusic) restoreLoop(menuThemeSound);
        }
        if (key == 10 or key == 13) resetGame(); // enter
    }
}

void keyup_or_w_pressed() {
    if (gameState == Menu) {
        menuItem.selectUpward();
        playAudioOnClick(menuHoverSound, checkmutedsfx);
    }
    else if (gameState == Options) {
        optionsItem.selectUpward();
        playAudioOnClick(menuHoverSound, checkmutedsfx);
    }
    else if (gameState == Credit) {
        creditpage = (creditpage == 1) ? 2 : 1; // toggle between 1 or 2
        playAudioOnClick(menuHoverSound, checkmutedsfx);
    }
    // towrite = "menu item: " + to_string(menuItem.getSelected()); // debug
}

void keydown_or_s_pressed() {
    if (gameState == Menu) {
        menuItem.selectDownward();
        playAudioOnClick(menuHoverSound, checkmutedsfx);
    }
    else if (gameState == Options) {
        optionsItem.selectDownward();
        playAudioOnClick(menuHoverSound, checkmutedsfx);
    }
    else if (gameState == Credit) {
        creditpage = (creditpage == 1) ? 2 : 1; // toggle between 1 or 2
        playAudioOnClick(menuHoverSound, checkmutedsfx);
    }
    // towrite = "menu item: " + to_string(menuItem.getSelected()); // debug
}

void keyboardDown(unsigned char key, int x, int y) {
    if (gameState == Play) playingKeyboardDown(key);
    else {
        if (key == 'w') keyup_or_w_pressed();
        else if (key == 's') keydown_or_s_pressed();
        else if (key == 10 or key == 13) { // enter key
            if (gameState == Menu) {
                playAudioOnClick(menuClickSound, checkmutedsfx);
                gameState = static_cast<State>(menuItem.getSelected() + 1);
                if (gameState == Play) {
                    stopLoop(menuThemeSound);
                    if (mainThemeSound.isInitOnly()) {
                        playAudioOnClick(mainThemeSound, checkmutedmusic);
                    }
                }
            }
            else if (gameState == Options) {
                playAudioOnClick(menuClickSound, checkmutedsfx);
                optionsItem.switchItemState();
                checkmutedmusic = (optionsItem.getStateFromIndex(0) == 1);
                checkmutedsfx = (optionsItem.getStateFromIndex(1) == 1);
                if (checkmutedmusic) stopLoop(menuThemeSound);
                else {
                    if (menuThemeSound.isStopped()) restoreLoop(menuThemeSound);
                }

            }
        }
        if (gameState != Menu) { // not in the menu
            if (key == 8) { // backspace
                playAudioOnClick(menuClickSound, checkmutedsfx);
                gameState = Menu;
            }
        }
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    if (gameState == Play) {
        if (isPlaying) {
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
                    // Stop ducking if 'S' is released, and the player is grounded
                    isDucking = false;
                    playerSizeY = 1.0f;
                    if (keyRightPressed) playerVelocityX = 0.1f;
                    if (keyLeftPressed) playerVelocityX = -0.1f;
                    extraJumpTimer = 10; // Set the timer duration
                }
                else {}

            }
        }
    }
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) keyup_or_w_pressed();
    else if (key == GLUT_KEY_DOWN) keydown_or_s_pressed();

    glutPostRedisplay();
}

void playingupdate() { // splitted from void update(), this is everything inside if (isPlaying)
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
            playAudioOnClick(damagedSound, checkmutedsfx);
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
}

void update(int value) {

    switch (gameState) {
    case Play:
        if (lives <= 0) gameOverTrigger();

        if (!isPaused) {
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

            if (isPlaying) playingupdate();
        }
        else stopLoop(mainThemeSound);
        break;
    case Quit:
        glutDestroyWindow(glutGetWindow());
        exit(0); // just in case
        break;
    };

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 60 FPS
}

void gameDisplay() { // splitted from void display(), everything inside it before the menu
    // drawPlayerSquare(playerX, playerY, playerSizeY); // Draw the player character (replaced with the image sprite)

    if (flickeringTimer < visibleFrameDuringFlicker) {
        if (isKnightLoaded) {
            if (lives > 0) {
                if (!isDucking) shuttleKnight.draw(playerX, playerY);
                else shuttleKnightDuck.draw(playerX, playerY);
            }
            else shuttleKnightDefeat.draw(playerX, playerY);
        }
        else { drawPlayerCharacter(playerX, playerY, playerSizeY, isDucking); } // Fallback when knight isn't loaded
        // drawPlayerCharacter(playerX, playerY, playerSizeY, isDucking); // for debugging
    }
    // drawPlayerHitbox(playerX, playerY, playerSizeY, true); // Debugging hitbox
    // playerHitbox.drawDummy(); // Debugging hitbox
    float racketColor[3] = { 1.0, 1.0, 1.0 };
    if (isSwingingRacket) {
        drawQuad(playerX + 1.35, playerY + 0.45, 0.7, 0.05, racketColor);
        drawHollowEllipse(playerX + 2.2, playerY + 0.5, 0.5, 0.3, 4, racketColor);
        drawRacketNet(playerX + 2.2, playerY + 0.5, 0.5, 0.3, 4, racketColor, 6);
    }
    else { // folded racket
        drawRotatedQuad(playerX + 1.09, playerY + 0.8, 0.7, 0.05, 80, racketColor);
        drawRotatedHollowEllipse(playerX + 1.2, playerY + 1.6, 0.5, 0.1, 4, 80, racketColor);
    }

    for (const Shuttlecock& shuttlecock : shuttlecocks) {
        shuttlecock.draw(); // Draw the shuttlecocks
    }

    drawExplosion();

    if (lives > 0) {
        for (int i = 0; i < lives; i++) {
            drawHeart(i * 0.7f + 0.1f, 5.5f, 0.6f);
        }
    }
    if (isPlaying) {
        drawText(0.1f, 5.3f, "Score: " + std::to_string(score));
        drawText(0.1f, 5.1f, "Highscore: " + std::to_string(highScore));
    }
    else {
        drawTextCenter(5.0f, 3.2f, "Game Over", GLUT_BITMAP_TIMES_ROMAN_24);
        drawTextCenter(5.0f, 2.9f, "Score: " + std::to_string(score), GLUT_BITMAP_HELVETICA_18);
        drawTextCenter(5.0f, 2.7f, "Highscore: " + std::to_string(highScore), GLUT_BITMAP_HELVETICA_18);
        drawTextCenter(5.0f, 2.5f, "You survived for " + std::to_string(secondPassed) + " seconds", GLUT_BITMAP_HELVETICA_18);
        drawTextCenter(5.0f, 2.3f, "Press Enter to play again", GLUT_BITMAP_HELVETICA_18);
        drawTextCenter(5.0f, 2.1f, "Press Backspace to go to menu", GLUT_BITMAP_HELVETICA_18);
    }

    if (isPaused) {
        drawTextCenter(5.0f, 3.0f, "Game Paused", GLUT_BITMAP_TIMES_ROMAN_24);
        drawTextCenter(5.0f, 2.7f, "Press Esc to continue playing", GLUT_BITMAP_HELVETICA_18);
        drawTextCenter(5.0f, 2.5f, "Press Backspace to go to menu", GLUT_BITMAP_HELVETICA_18);
        drawTextCenter(5.0f, 2.3f, "Press Delete key to restart game", GLUT_BITMAP_HELVETICA_18);
    }

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (gameState) {
    case Play:
        gameDisplay();
        break;
    case Menu:
        shuttleMenuBackground.draw(0.1f, 0.1f);
        // drawDebugLine();
        drawTextCenter_menu(5.0f, 5.0f, "Shuttle Madness", GLUT_BITMAP_TIMES_ROMAN_24);
        menuItem.draw();
        drawTextCenter_menu(5.0f, 0.4f, "Press Enter to select", GLUT_BITMAP_9_BY_15);
        // drawShuttlecock(5.0f, 3.0f, 90, 1.0f, pinkColor); // unused for background
        break;
    case How_to_Play:
        drawFromTxt(0.3f, 4.0f, 0.15f, "shuttle_howtoplay.txt",
            std::vector<std::string>(), std::vector<std::string>(), GLUT_BITMAP_9_BY_15,
            { 0, 35 });

        drawTextCenter_menu(5.0f, 0.2f, "Press Backspace to go back", GLUT_BITMAP_9_BY_15);
        break;
    case Options:
        optionsItem.draw();
        drawTextCenter_menu(5.0f, 0.4f, "Press Enter to toggle", GLUT_BITMAP_9_BY_15);
        drawTextCenter_menu(5.0f, 0.2f, "Press Backspace to go back", GLUT_BITMAP_9_BY_15);
        break;
    case Credit:
        if (creditpage == 1) {
            drawFromTxt(0.3f, 5.5f, 0.12f, "shuttle_audio_credit.txt", creditbefore,
                std::vector<std::string>(), GLUT_BITMAP_8_BY_13, { 0, 37 });
        }
        else if (creditpage == 2) {
            drawFromTxt(0.3f, 5.5f, 0.12f, "shuttle_audio_credit.txt", std::vector<std::string>(),
                creditafter, GLUT_BITMAP_8_BY_13, { 38, -1 });
        }
        drawTextCenter_menu(5.0f, 0.4f, "Up/Down arrow to switch pages", GLUT_BITMAP_9_BY_15);
        drawTextCenter_menu(5.0f, 0.2f, "Press Backspace to go back", GLUT_BITMAP_9_BY_15);
        break;
    case Quit:
        drawTextCenter_menu(5.0f, 3.0f, "Quitting...");
        break;
    };

    glutSwapBuffers();
}

// void idle() { // unused, old way to play audio
//     playMainTheme();
// }

const float aspectRatio = 4.0 / 3.0;

void reshape(int width, int height) {
    // Keep the window dimensions fixed
    glutReshapeWindow(800, 600);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shuttle Madness");

    // Seed the random number generator with the current time
    srand(static_cast<unsigned>(time(nullptr)));

    GLdouble rightOrtho = 10.0;
    GLdouble topOrtho = 6.0;
    glOrtho(0.0, rightOrtho, 0.0, topOrtho, -1.0, 1.0);

    glutDisplayFunc(display);
    // glutKeyboardFunc(keyboard);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys);
    // glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    // resetValues();

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

    // initializeShuttlecocks(); // early testing for balancing

    scriptDir = std::filesystem::path(argv[0]).parent_path().string();
    // writeDebugText("scriptDir: " + scriptDir);

    // printAudioFilePaths(); // testing

    initializeEveryAudio();

    menuItem.calculateTextsCoordinate(rightOrtho / 2, topOrtho / 2, menuSpacing);
    optionsItem.calculateTextsCoordinate(rightOrtho / 2, topOrtho / 2, menuSpacing);

    // spawnShuttlecock(0, 10.5f, 1.1f); // testing hitbox

    glutMainLoop();

    uninitEveryAudio(); // can this be reached? idk
    // consider freeglut with glLeaveMainLoop()

    return 0;
}
