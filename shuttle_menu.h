// 2023 Jibril Wathon
// github.com/etherealxx
// Public Domain

#ifndef SHUTTLE_MENU_H
#define SHUTTLE_MENU_H

#include <GL/glut.h>
// #include <GL/freeglut.h>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;

float menuSpacing = 0.4; // spacing between menu items
string towrite = "test";
extern string scriptDir;

enum State {
    Menu,
    Play,
    How_to_Play,
    Options,
    Credit,
    Quit
};

void drawText_menu(float x, float y, string texttotype, void* font = GLUT_BITMAP_HELVETICA_18) {
    glPushMatrix();
    glRasterPos2f(x, y);

    // Display characters one by one
    for (char c : texttotype) {
        glutBitmapCharacter(font, c);
    }
    glPopMatrix();
}

float calculateTextX_Center_menu(float x, const string& text, void* font = GLUT_BITMAP_HELVETICA_18) {
    int count = text.size();
    float alignmentFactor;

    if (font == GLUT_BITMAP_9_BY_15) alignmentFactor = 0.057f;
    else if (font == GLUT_BITMAP_TIMES_ROMAN_24) alignmentFactor = 0.065f;
    else if (font == GLUT_BITMAP_HELVETICA_18) alignmentFactor = 0.05f;
    else {
        alignmentFactor = 0.05f;
    }

    float xPos = x - (count * alignmentFactor);
    return xPos;
}

void drawTextCenter_menu(float x, float y, const string& text, void* font = GLUT_BITMAP_HELVETICA_18) {
    float adjusted_xpos = calculateTextX_Center_menu(x, text, font);
    drawText_menu(adjusted_xpos, y, text, font);
}

void drawFromTxt(float x, float starty, float spacing, string txtFile, const vector<string>& writeBefore = vector<string>(),
    const vector<string>& writeAfter = vector<string>(), void* font = GLUT_BITMAP_8_BY_13) {
    string filePath = scriptDir + "/" + txtFile;

    // Open the file
    ifstream inputFile(filePath);

    // Check if the file is opened successfully
    if (inputFile.is_open()) {
        vector<string> lines;

        if (!writeBefore.empty()) lines = writeBefore;

        string line;
        while (getline(inputFile, line)) {
            lines.push_back(line);
        }

        if (!writeAfter.empty()) lines.insert(lines.end(), writeAfter.begin(), writeAfter.end());

        inputFile.close();

        float texty = starty;
        // Output each line from the vector
        for (const auto& line : lines) {
            // std::cout << line << std::endl;
            drawText_menu(x, texty, line, font);
            texty -= spacing;
        }
    }
    else {
        drawText_menu(x, starty, txtFile + " not found", font);
    }
}

class Title {
public:
    Title(const vector<string>& tobe_item,
        int tobe_select = 0,
        const vector<string>& tobe_itemsecond = vector<string>()) :
        item(tobe_item), selectedItem(tobe_select), itemSecondState(tobe_itemsecond) {


        if (!itemSecondState.empty()) {
            originalItem = item;
            itemSecondStateSwitch = vector<int>(itemSecondState.size(), 0);
        }
    }

    void calculateTextsCoordinate(float x, float y, float spacing) {
        vector<pair<float, float>> coordinatePair;
        int itemCount = item.size();

        vector<int> charCounts;

        for (const string& text : item) {
            int count = text.size();
            charCounts.push_back(count);
        }

        auto longestCharacterSearch = max_element(charCounts.begin(), charCounts.end());
        int longestCharacter = *longestCharacterSearch;

        // Let's say you have glOrtho(a, b, c, d, e, f);
        // then alignmentFactor is about ((b - a) * 3 / 500) based on my trial and error
        // old calculation: ((b - a) / 200)
        float alignmentFactor = 0.06f;

        for (int i = 0; i < itemCount; i++) {
            float yOffset; // this one is odd only: (itemCount / 2 - i) * spacing;
            if (itemCount % 2 == 0) {
                // When itemCount is even, exclude the middle position
                yOffset = ((itemCount / 2 - i) - 0.5) * spacing;
            }
            else {
                // When itemCount is odd, keep the original calculation
                yOffset = (itemCount / 2 - i) * spacing;
            }
            float yPos = y + yOffset;
            float xPos = x + ((longestCharacter - charCounts[i]) * alignmentFactor) - (longestCharacter * alignmentFactor);
            // drawText(xPos, yPos, textToWrite[i]);
            coordinatePair.push_back(make_pair(xPos, yPos));
        }

        itemCoordinates = coordinatePair;
    }

    void draw() {
        if (item.size() == itemCoordinates.size()) {
            glPushMatrix();
            for (int i = 0; i < item.size(); i++) {
                if (i == selectedItem) glColor3f(1.0, 0.0, 0.0);
                else glColor3f(1.0, 1.0, 1.0);
                drawText_menu(itemCoordinates[i].first, itemCoordinates[i].second, item[i]);
                glColor3f(1.0, 1.0, 1.0);
            }
            glPopMatrix();
        }
    }

    void selectUpward() {
        selectedItem = (selectedItem - 1 + item.size()) % item.size();
    }

    void selectDownward() {
        selectedItem = (selectedItem + 1) % item.size();
    }

    int getSelected() { return selectedItem; }

    void switchItemState() {

        size_t index = selectedItem;

        if (itemSecondStateSwitch[index] == 0) { // not switched yet
            // Update the nth element of B with the value of the nth element of A
            if (index < item.size() && index < itemSecondState.size()) {
                item[index] = itemSecondState[index];
            }
            itemSecondStateSwitch[index] = 1; // switched
        }
        else {
            if (index < item.size() && index < originalItem.size()) {
                item[index] = originalItem[index];
            }
            itemSecondStateSwitch[index] = 0;
        }

    }

private:
    vector<string> item;
    vector<pair<float, float>> itemCoordinates;
    int selectedItem;
    vector<string> itemSecondState;
    vector<int> itemSecondStateSwitch;
    vector<string> originalItem;
};

#endif