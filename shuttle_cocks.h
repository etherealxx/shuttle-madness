#ifndef SHUTTLE_COCKS_H
#define SHUTTLE_COCKS_H

#include <GL/glut.h>
#include <cmath>
#include <vector>
#include "shuttle_sound.h"
#include "shuttle_explosion.h"

extern float racketRadiusX;
extern float racketRadiusY;
extern float playerX;
extern float playerY;
extern float playerSizeY;
extern bool isSwingingRacket;
extern int racketHitLimit;
extern float shuttleCockOutofBoundLimit;
extern Sound racketHitSound;
extern Sound clankSound;
extern Sound explosionSound;
extern int lives;
extern int damageInvincibilityTimer;
extern int score;
extern bool checkmutedsfx;

// shuttlecock variant
const int SC_BLACK = 0x01;  // 00000001
const int SC_YELLOW = 0x02;   // 00000010

void drawHalfCircle(float x, float y, float radius, int numSegments) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 3.14159265f * i / numSegments;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }
    glEnd();
    glPopMatrix();
}

void drawTrapezium(float x, float y, float topWidth, float bottomWidth, float height) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-bottomWidth / 2, 0.0f);
    glVertex2f(bottomWidth / 2, 0.0f);
    glVertex2f(topWidth / 2, height);
    glVertex2f(-topWidth / 2, height);
    glEnd();
    glPopMatrix();
}

void drawFeather(float x, float y, float angle) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 0.18f, 1.0f); // Adjust the scale for feather shape
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawShuttlecock(float noseposX, float noseposY, float shuttlecockAngle, float scaleFactor, float color[3], float transparency = 1.0f) {
    glPushMatrix();
    if (transparency != 1.0f) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    glColor4f(color[0], color[1], color[2], transparency);
    // glColor3fv(color); // Set the color
    glTranslatef(noseposX, noseposY, 0.0f);
    glRotatef(shuttlecockAngle, 0.0f, 0.0f, 1.0f); // Apply rotation to the entire shuttlecock
    glScalef(scaleFactor * 3 / 4, scaleFactor, 1.0f); // Apply scaling to the entire shuttlecock

    // Draw the half-circle nose
    drawHalfCircle(0.0f, 0.0f, 0.5f, 100);

    // Draw the trapezium skirt
    drawTrapezium(0.0f, -0.7f, 1.0f, 1.5f, 0.5f);

    // Draw the feathers
    for (int i = 1; i < 7; i++) {
        float angle = (i - 1) * 10.0f + 70.0f;
        drawFeather(i * 0.31f - 1.05f, -1.0f, angle);
    }
    if (transparency != 1.0f)glDisable(GL_BLEND);
    glPopMatrix();
}

class Shuttlecock {
public:
    float x;
    float y;
    float speed;
    float angle;
    bool inMotion;
    bool gotHit;
    bool toRemove;
    int flags;

    Shuttlecock(float initialX, float initialY, float initialSpeed, float initialAngle, int initialFlags = 0)
        : x(initialX), y(initialY), speed(initialSpeed), angle(initialAngle), flags(initialFlags),
        inMotion(true), gotHit(false), toRemove(false) {
    }

    void update() {
        if (inMotion) {
            x -= speed;

            // Collision detection with the racket
            float racketCenterX = playerX + 2.2f;
            float racketCenterY = playerY + 0.5f;
            float hitboxRacketRadiusX = racketRadiusX + 0.1f;
            float hitboxRacketRadiusY = racketRadiusY + 0.1f;
            // float racketRadiusX = 0.5;
            // float racketRadiusY = 0.3;

            // Calculate the distance between the shuttlecock and the center of the ellipse
            float distanceX = x - racketCenterX;
            float distanceY = y - racketCenterY;

            // Check if the shuttlecock is inside the ellipse
            if ((distanceX * distanceX) / (hitboxRacketRadiusX * hitboxRacketRadiusX) + (distanceY * distanceY) / (hitboxRacketRadiusY * hitboxRacketRadiusY) <= 1.0) {
                if (isSwingingRacket && !gotHit) {
                    if (flags & SC_BLACK) {
                        gotHit = true;
                        playAudioOnClick(clankSound, checkmutedsfx);
                    }
                    else {
                        if (racketHitLimit > 0) {
                            if (flags & SC_YELLOW) {
                                toRemove = true;
                                racketHitLimit--;
                                damageInvincibilityTimer = 120;
                                lives--;
                                explode();
                            }
                            else {
                                // Adjust anchor point rotation
                                racketHitLimit--;
                                gotHit = true;
                                x += 0.5f;
                                // Collision detected, reverse the speed and rotate by 180 degrees
                                speed = -speed;
                                angle += 180.0f; // Rotate the shuttlecock by 180 degrees
                                playAudioOnClick(racketHitSound, checkmutedsfx);
                                if (lives > 0) score++;
                            }
                        }
                    }
                }
            }

            // Check if a shuttlecock is out of bounds or needs to be reset
            if (x < -0.5f && (flags == 0)) {
                x = 10.0f; // Reset the shuttlecock's position
                // Randomize the y coordinate between 0.1 and 5.8
                y = 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / 5.7f);
            }
        }
    }

    void draw() const {
        if (inMotion) {

            std::vector<float> shuttleCockColorVector;

            switch (flags) {
            case SC_BLACK:
                shuttleCockColorVector = { 0.3f, 0.3f, 0.3f }; break;
            case SC_YELLOW:
                shuttleCockColorVector = { 1.0f, 1.0f, 0.0f }; break;
            default:
                shuttleCockColorVector = { 1.0f, 0.0f, 0.5f };
            }
            // Convert the vector into a C-style array
            float shuttleCockColor[3];
            for (size_t i = 0; i < shuttleCockColorVector.size(); ++i) {
                shuttleCockColor[i] = shuttleCockColorVector[i];
            }

            drawShuttlecock(x, y, angle, 0.3f, shuttleCockColor);
        }
    }

    bool isOutOfBounds() {
        return x > shuttleCockOutofBoundLimit;
    }

    void checkExplode() {
        if (flags & SC_YELLOW) explode();
    }

    void explode() {
        createExplosionInstance(x, y);
        playAudioOnClick(explosionSound, checkmutedsfx);
    }

    int getFlags() { return flags; }
    int getX() { return x; }
    int getY() { return y; }
};

extern std::vector<Shuttlecock> shuttlecocks;

class PlayerHitbox {
public:
    float x;
    float y;
    float width;
    float height;
    float initheight;
    bool knightMode;

    PlayerHitbox(float initplayerX, float initplayerY, float initplayerSizeY, bool isKnight = false) {
        // check if the knight sprite is loaded, then adjust the values
        knightMode = isKnight;
        x = initplayerX + (knightMode ? 0.0f : 0.2f);
        y = initplayerY - (knightMode ? 0.5f : 0.0f);
        width = (knightMode ? 1.0f : 0.83f); // - 0.2f for non knight
        height = (knightMode ? 1.0f : 0.8f); // Adjust the height as needed
        initheight = height;
    }

    bool collidesWith(const Shuttlecock& shuttlecock) {
        // Check for collision between the shuttlecock and the hitbox
        return (shuttlecock.x + 0.15f > x - width / 2 &&
            shuttlecock.x - 0.15f < x + width / 2 &&
            shuttlecock.y + 0.15f > y - height / 2 &&
            shuttlecock.y - 0.15f < y + height / 2);
    }

    void drawDummy() { //doesn't work as expected
        glPushMatrix();
        glColor3f(0.0f, 1.0f, 0.0f); // Green color
        glTranslatef(playerX, playerY, 0.0f);
        glLineWidth(1); // Set the line width
        glLineStipple(1, 0x00FF);
        glEnable(GL_LINE_STIPPLE);
        glBegin(GL_LINE_LOOP);

        // forgot why these are commented
        // glVertex2f(x, y); // x, y : (0.2f, 0.0f)
        // glVertex2f(x, y + height); // x, y + height : (0.2f, 0.8f)
        // glVertex2f(x + width, y + height); // x + width, y + height : (0.85f, 0.8f)
        // glVertex2f(x + width, y); // x + width, y : (0.85f, 0.0f)

        glVertex2f((knightMode ? 0.0f : 0.2f), 0.0f); // Start from (0, 0)
        glVertex2f((knightMode ? 0.0f : 0.2f), height); // Move up to (0, height)
        glVertex2f(width, height); // Move right to (width, height)
        glVertex2f(width, 0.0f); // Move down to (width, 0)
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        glPopMatrix();
    }

    void update() {
        x = playerX + (knightMode ? 0.0f : 0.2f);;
        y = playerY + (knightMode ? 0.4f : 0.0f);;
        height = initheight * playerSizeY;
    }
};

void initializeShuttlecocks() { // early debugging
    for (int i = 0; i < 10; i++) {
        float initialX = 9.5f;
        float initialY = 0.5f + 0.3f * i;
        float initialSpeed = 0.07f; // REMEMBER to change back to 0.07
        float initialAngle = 90.0f;
        Shuttlecock shuttlecock(initialX, initialY, initialSpeed, initialAngle);
        shuttlecocks.push_back(shuttlecock);
    }
}

// void spawnShuttlecock(std::string alternate = "") {
void spawnShuttlecock(int initialFlags = 0, float initialX = 10.5f, float initialY = -9.9f) {
    // Randomize the y coordinate between 0.1 and 5.8
    if (initialY == -9.9f) { // default then randomize
        initialY = 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / 5.7f);
    }
    float initialSpeed = 0.07f; // REMEMBER to change back to 0.07
    float initialAngle = 90.0f;
    // float initialBlack = (alternate == "black") ? true : false;
    Shuttlecock shuttlecock(initialX, initialY, initialSpeed, initialAngle, initialFlags);
    shuttlecocks.push_back(shuttlecock);
}

#endif