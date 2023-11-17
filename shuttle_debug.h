#ifndef SHUTTLE_DEBUG_H
#define SHUTTLE_DEBUG_H

#include <GL/glut.h>
#include <vector>
#include <string>
#include <sstream>

extern std::vector<std::string> textLines;
const float lineSpacing = 0.2;
const int maxLines = 30; // Maximum number of lines to display
const float maxLineWidth = 7.0; // Maximum line width before wrapping

void drawDebugText(float x, float y) {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    // Display each line of text
    for (int i = 0; i < textLines.size(); i++) {
        glRasterPos2f(x, y - i * 0.2f);

        // Display characters one by one
        for (char c : textLines[i]) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
        }
    }
    glPopMatrix();
}

void writeDebugText(const std::string& text) {
    std::istringstream iss(text);
    std::string line;
    while (std::getline(iss, line, '\n')) {
        std::istringstream wordWrap(line);
        std::string wrappedLine;

        while (std::getline(wordWrap, line, ' ')) {
            // Calculate the line width
            float lineWidth = wrappedLine.empty() ? 0 : (wrappedLine.length() * 0.12 + line.length() * 0.12);

            if (!wrappedLine.empty() && lineWidth > maxLineWidth) {
                textLines.push_back(wrappedLine);
                wrappedLine = line;
            }
            else if (wrappedLine.empty()) {
                wrappedLine = line;
            }
            else {
                wrappedLine += " " + line;
            }
        }

        if (!wrappedLine.empty()) {
            textLines.push_back(wrappedLine);
        }
    }

    // Remove lines that exceed the maximum allowed
    while (textLines.size() > maxLines) {
        textLines.erase(textLines.begin());
    }

    glutPostRedisplay();
}

#endif