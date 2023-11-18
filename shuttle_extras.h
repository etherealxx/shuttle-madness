#ifndef SHUTTLE_EXTRAS_H
#define SHUTTLE_EXTRAS_H

#include <GL/glut.h>
#include <cmath>
#include <vector>

const int numSegments = 100;
extern bool isKnightLoaded;

// Function to draw the player character
void drawPlayerSquare(float playerX, float playerY, float playerSizeY, bool lineonly = false) {
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glTranslatef(playerX, playerY, 0.0f);
    // Scale the character based on the playerSizeY
    glScalef(1.0f, playerSizeY, 1.0f);

    // Draw the player character (a red square)

    if (lineonly) {
        glLineWidth(1); // Set the line width
        glLineStipple(1, 0x00FF);
        glEnable(GL_LINE_STIPPLE);
        glBegin(GL_LINE_LOOP);
    }
    else { glBegin(GL_QUADS); }

    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();
    if (lineonly) { glDisable(GL_LINE_STIPPLE); }
    glPopMatrix();
}

// For debugging
void drawPlayerHitbox(float playerX, float playerY, float playerSizeY, bool lineonly = false) {
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glTranslatef(playerX, playerY, 0.0f);
    // Scale the character based on the playerSizeY
    glScalef(1.0f, playerSizeY, 1.0f);

    // Draw the player character (a red square)

    if (lineonly) {
        glLineWidth(1); // Set the line width
        glLineStipple(1, 0x00FF);
        glEnable(GL_LINE_STIPPLE);
        glBegin(GL_LINE_LOOP);
    }
    else { glBegin(GL_QUADS); }

    // based on drawPlayerCharacter
    glVertex2f(0.2f, 0.0f);
    glVertex2f(0.2f, 0.8f);
    glVertex2f(0.85f, 0.8f);
    glVertex2f(0.85f, 0.0f);
    glEnd();
    if (lineonly) { glDisable(GL_LINE_STIPPLE); }
    glPopMatrix();
}

void drawPlayerCharacter(float playerX, float playerY, float playerSizeY, float isDucking) {
    float duckValue = isDucking ? 0.2f : 0.0f;
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glTranslatef(playerX, playerY, 0.0f);
    // // Scale the character based on the playerSizeY
    // glScalef(1.0f, playerSizeY, 1.0f);
    // Draw the player character (a red square)
    glBegin(GL_POLYGON);
    glVertex2f(0.55f, 0.2f - (duckValue / 4)); // crotch
    glVertex2f(0.35f, 0.0f); // right part of left leg
    glVertex2f(0.2f, 0.0f); // left part of left leg
    glVertex2f(0.35f, 0.2f - (duckValue / 4)); // left waist
    glVertex2f(0.25f, 0.2f - (duckValue / 4)); // bottom left square corner

    glVertex2f(0.25f, 0.7f - duckValue / 0.7);
    glVertex2f(0.35f, 0.7f - duckValue / 0.7);
    glVertex2f(0.35f, 0.8f - duckValue / 0.7);
    glVertex2f(0.65f, 0.8f - duckValue / 0.7);
    glVertex2f(0.65f, 0.7f - duckValue / 0.7);
    glVertex2f(0.85f, 0.7f - duckValue / 0.7);

    glVertex2f(0.85f, 0.4f - (duckValue / 3));
    glVertex2f(1.0f, 0.5f); // top part of hand
    glVertex2f(1.0f, 0.4f); // bottom part of hand
    glVertex2f(0.85f, 0.35f);

    glVertex2f(0.85f, 0.2f - (duckValue / 4)); // right waist
    glVertex2f(0.7f, 0.2f - (duckValue / 4)); // bottom right square corner

    glVertex2f(0.85f, 0.0f);
    glVertex2f(0.65f, 0.0f); // left part of right leg
    glEnd();
    glPopMatrix();
}

void drawHeart(float x, float y, float size) {
    glPushMatrix();
    if (isKnightLoaded) { glColor3f(0.56f, 0.25f, 0.29f); }
    else { glColor3f(0.5f, 0.0f, 0.0f); } // Dark red
    glTranslatef(x, y, 0.0f);
    glScalef(size, size, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0.45f, 0.0f);
    glVertex2f(0.0f, 0.3f); // symmetry a
    glVertex2f(0.0f, 0.55f); // symmetry b
    glVertex2f(0.15f, 0.75f); // symmetry c
    glVertex2f(0.35f, 0.75f); // symmetry d
    glVertex2f(0.5f, 0.6f);
    glVertex2f(0.65f, 0.75f); // symmetry d
    glVertex2f(0.85f, 0.75f); // symmetry c
    glVertex2f(1.0f, 0.55f); // symmetry b
    glVertex2f(1.0f, 0.3f); // symmetry a
    glVertex2f(0.55f, 0.0f);
    glEnd();
    glPopMatrix();
}

void drawCircle(float cx, float cy, float r, float color[3]) {
    glColor3fv(color); // Set the color

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);

    for (int i = 0; i <= numSegments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }

    glEnd();
}

void drawEllipse(float cx, float cy, float radiusX, float radiusY, float color[3]) {
    glColor3fv(color); // Set the color

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);

    for (int i = 0; i <= numSegments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = radiusX * cosf(theta);
        float y = radiusY * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }

    glEnd();
}

void drawHollowEllipse(float cx, float cy, float radiusX, float radiusY, float lineWidth, float color[3]) {
    glColor3fv(color); // Set the color
    glLineWidth(lineWidth); // Set the line width

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = radiusX * cosf(theta);
        float y = radiusY * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }

    glEnd();
}

void drawRotatedHollowEllipse(float cx, float cy, float radiusX, float radiusY, float lineWidth, float angle, float color[3]) {
    glColor3fv(color); // Set the color
    glLineWidth(lineWidth); // Set the line width

    glPushMatrix();

    glTranslatef(cx, cy, 0.0f); // Translate to the center of the ellipse
    glRotatef(angle, 0.0f, 0.0f, 1.0f); // Rotate around the center of the ellipse

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = radiusX * cosf(theta);
        float y = radiusY * sinf(theta);
        glVertex2f(x, y);
    }

    glEnd();

    glPopMatrix();
}

void drawRacketNet(float cx, float cy, float radiusX, float radiusY, float lineWidth, float color[3], int numGridLines) {
    glColor3fv(color); // Set the color
    glLineWidth(lineWidth); // Set the line width
    int numHorizontalLines = numGridLines;
    int numVerticalLines = numGridLines;

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = radiusX * cosf(theta);
        float y = radiusY * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }

    glEnd();

    glLineWidth(lineWidth * 2 / 3); // Set the line width of the strings

    // Draw horizontal grid lines inside the ellipse
    for (int i = 1; i <= numHorizontalLines; i++) {
        float y = cy - radiusY + 2.0 * radiusY * i / (numHorizontalLines + 1);

        // Calculate the x-coordinates based on the ellipse equation
        float x1 = cx - sqrt(radiusX * radiusX * (1 - (y - cy) * (y - cy) / (radiusY * radiusY)));
        float x2 = cx + sqrt(radiusX * radiusX * (1 - (y - cy) * (y - cy) / (radiusY * radiusY)));

        glBegin(GL_LINES);
        glVertex2f(x1, y);
        glVertex2f(x2, y);
        glEnd();
    }

    // Draw vertical grid lines inside the ellipse
    for (int i = 1; i <= numGridLines; i++) {
        float x = cx - radiusX + 2.0 * radiusX * i / (numGridLines + 1);

        // Calculate the y-coordinates based on the ellipse equation
        float y1 = cy - sqrt(radiusY * radiusY * (1 - (x - cx) * (x - cx) / (radiusX * radiusX)));
        float y2 = cy + sqrt(radiusY * radiusY * (1 - (x - cx) * (x - cx) / (radiusX * radiusX)));

        glBegin(GL_LINES);
        glVertex2f(x, y1);
        glVertex2f(x, y2);
        glEnd();
    }
}

void drawQuad(float anchorX, float anchorY, float width, float height, float color[3]) {
    glColor3fv(color); // Set the color

    glBegin(GL_QUADS);

    glVertex2f(anchorX - width / 2, anchorY - height / 2); // Bottom-left vertex
    glVertex2f(anchorX - width / 2, anchorY + height / 2); // Top-left vertex
    glVertex2f(anchorX + width / 2, anchorY + height / 2); // Top-right vertex
    glVertex2f(anchorX + width / 2, anchorY - height / 2); // Bottom-right vertex

    glEnd();
}

void drawRotatedQuad(float anchorX, float anchorY, float width, float height, float angle, float color[3]) {
    glColor3fv(color); // Set the color

    glPushMatrix();

    glTranslatef(anchorX, anchorY, 0.0f); // Translate to the anchor point
    glRotatef(angle, 0.0f, 0.0f, 1.0f); // Rotate around the anchor point

    glBegin(GL_QUADS);

    glVertex2f(-width / 2, -height / 2); // Bottom-left vertex
    glVertex2f(-width / 2, height / 2);  // Top-left vertex
    glVertex2f(width / 2, height / 2);   // Top-right vertex
    glVertex2f(width / 2, -height / 2);  // Bottom-right vertex

    glEnd();

    glPopMatrix();
}

void drawText(float x, float y, std::string texttotype, void* font = GLUT_BITMAP_9_BY_15,
    std::vector<float> colorVector = { 1.0f, 1.0f, 1.0f }) {

    glPushMatrix();
    float color[3];
    for (size_t i = 0; i < colorVector.size(); ++i) {
        color[i] = colorVector[i];
    }
    glColor3fv(color);
    // Display each line of text
    glRasterPos2f(x, y);

    // Display characters one by one
    for (char c : texttotype) {
        glutBitmapCharacter(font, c);
    }
    glPopMatrix();
}

float calculateTextX_Center(float x, const std::string& text, void* font = GLUT_BITMAP_9_BY_15) {
    int count = text.size();
    float alignmentFactor;

    if (font == GLUT_BITMAP_9_BY_15) alignmentFactor = 0.05f;
    else if (font == GLUT_BITMAP_TIMES_ROMAN_24) alignmentFactor = 0.085f;
    else if (font == GLUT_BITMAP_HELVETICA_18) alignmentFactor = 0.058f;
    else {
        alignmentFactor = 0.05f;
    }

    float xPos = x - (count * alignmentFactor);
    return xPos;
}

void drawTextCenter(float x, float y, const std::string& text, void* font = GLUT_BITMAP_9_BY_15) {
    float adjusted_xpos = calculateTextX_Center(x, text, font);
    // drawText(x, y, to_string(adjusted_xpos));
    // drawText(x, y, text);
    drawText(adjusted_xpos, y, text, font);
}

void drawDebugLine() { // Draw two vertical lines in the middle of the screen
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINES);
    glVertex2f(5.0, 0.0);
    glVertex2f(5.0, 6.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0.0, 3.0);
    glVertex2f(10.0, 3.0);
    glEnd();
}

// class PlayerHitbox {}

#endif
