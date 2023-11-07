#ifndef SHUTTLE_IMAGE_H
#define SHUTTLE_IMAGE_H

#include <GL/glut.h>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
extern string scriptDir;

// class Image {
// public:
//     int width, height, numChannels, GLcolorchannel;
//     string imageFilePath;
//     unsigned char* image = nullptr;

//     Image() {};
//     Image(string filename) {
//         imageFilePath = scriptDir + "\\" + filename;  // Construct the relative path
//         image = stbi_load(imageFilePath.c_str(), &width, &height, &numChannels, 0);
//         if (image) {
//             stbi__vertical_flip(image, width, height, numChannels);
//             if (numChannels == 3) GLcolorchannel = GL_RGB;
//             else if (numChannels == 4) GLcolorchannel = GL_RGBA;
//         }
//     }

//     void free() {
//         stbi_image_free(image);
//     }

//     void draw() {
//         glDrawPixels(width, height, GLcolorchannel, GL_UNSIGNED_BYTE, image);
//     }
// };

class Image {
public:
    Image(const std::string& filename) : image(nullptr) {
        std::string imagePath = findImage(filename);
        // std::string imagePath = scriptDir + "\\" + filename;  // Construct the relative path
        if (loadImage(imagePath, image, width, height, numChannels)) {
            stbi__vertical_flip(image, width, height, numChannels);
            if (numChannels == 3) GLcolorchannel = GL_RGB;
            else if (numChannels == 4) GLcolorchannel = GL_RGBA;
        }
        else { image = nullptr; }
    }

    ~Image() {
        if (image) { stbi_image_free(image); }
    }

    void draw(float x, float y) const {
        if (image) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glRasterPos2f(x, y);
            glDrawPixels(width, height, GLcolorchannel, GL_UNSIGNED_BYTE, image);
            glDisable(GL_BLEND);
        }
    }

    int getWidth() const { return width; }

    int getHeight() const { return height; }

    unsigned char* getImageData() const { return image; }

    bool isLoaded() const { return image != nullptr; }

private:
    int width, height, numChannels, GLcolorchannel;
    unsigned char* image;

    static std::string findImage(const std::string& imageName) {
        char buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::string executablePath = buffer;
        size_t lastBackslashPos = executablePath.find_last_of("\\");
        if (lastBackslashPos != std::string::npos) {
            std::string executableDirectory = executablePath.substr(0, lastBackslashPos + 1);
            std::string imagePath = executableDirectory + imageName;
            if (std::filesystem::exists(imagePath)) {
                return imagePath;
            }
        }
        return "";
    }

    static bool loadImage(const std::string& imagePath, unsigned char*& image, int& width, int& height, int& numChannels) {
        if (imagePath.empty()) { return false; }
        image = stbi_load(imagePath.c_str(), &width, &height, &numChannels, 0);
        return image != nullptr;
    }
};

#endif