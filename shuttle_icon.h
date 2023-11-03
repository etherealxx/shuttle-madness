#ifndef SHUTTLE_ICON_H
#define SHUTTLE_ICON_H

#include <windows.h>

HICON CreateIconFromData(const unsigned char* imageData, int width, int height) {
    HICON hIcon = NULL;
    ICONINFO iconInfo;

    memset(&iconInfo, 0, sizeof(iconInfo));
    iconInfo.fIcon = TRUE;
    iconInfo.xHotspot = 0;
    iconInfo.yHotspot = 0;
    iconInfo.hbmMask = CreateBitmap(width, height, 1, 1, NULL);
    iconInfo.hbmColor = CreateBitmap(width, height, 1, 32, imageData);

    if (iconInfo.hbmMask && iconInfo.hbmColor) {
        hIcon = CreateIconIndirect(&iconInfo);
        DeleteObject(iconInfo.hbmMask);
        DeleteObject(iconInfo.hbmColor);
    }

    return hIcon;
}

unsigned char* GenerateIconData() {
    unsigned char* icon_data = new unsigned char[32 * 32 * 4];
    for (int i = 0; i < 32 * 32; ++i) {
        int x = i % 32;
        int y = i / 32;
        int index = 4 * i;

        // Initialize to transparent
        icon_data[index] = 0;
        icon_data[index + 1] = 0;
        icon_data[index + 2] = 0;
        icon_data[index + 3] = 0;

        // Red square
        if (x < 24) {
            icon_data[index] = 0;     // Blue component
            icon_data[index + 1] = 0; // Green component
            icon_data[index + 2] = 255; // Red component
        } else {
            icon_data[index] = 255;   // White component
            icon_data[index + 1] = 255; // White component
            icon_data[index + 2] = 255; // White component
        }
        icon_data[index + 3] = 255; // Alpha component (opaque)

        // Black outline
        if (x < 3 || x >= 28 || y < 3 || y >= 28) {
            icon_data[index] = 0;     // Black outline
            icon_data[index + 1] = 0; // Black outline
            icon_data[index + 2] = 0; // Black outline
            icon_data[index + 3] = 255; // Opaque
        }
    }
    return icon_data;
}

#endif
