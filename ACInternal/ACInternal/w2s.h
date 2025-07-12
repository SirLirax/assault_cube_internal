#pragma once
#include "vector_helper.h"
inline bool WorldToScreen(Vector3 pos, Vector3& screen, float ViewMatrix[16], int windowWidth, int windowHeight)
{
    //Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
    Vector4 clipCoords;
    clipCoords.x = pos.x * ViewMatrix[0] + pos.y * ViewMatrix[4] + pos.z * ViewMatrix[8] + ViewMatrix[12];
    clipCoords.y = pos.x * ViewMatrix[1] + pos.y * ViewMatrix[5] + pos.z * ViewMatrix[9] + ViewMatrix[13];
    clipCoords.z = pos.x * ViewMatrix[2] + pos.y * ViewMatrix[6] + pos.z * ViewMatrix[10] + ViewMatrix[14];
    clipCoords.w = pos.x * ViewMatrix[3] + pos.y * ViewMatrix[7] + pos.z * ViewMatrix[11] + ViewMatrix[15];

    if (clipCoords.w < 0.1f)
        return false;

    //perspective division, dividing by clip.W = Normalized Device Coordinates
    Vector3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    //Transform to window coordinates
    screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
    screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
    return true;
}