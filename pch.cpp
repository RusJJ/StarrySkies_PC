#include "pch.h"

CVector* CamPosPtr;

bool (*CalcScreenCoors)(CVector*, CVector*, float*, float*, bool, bool);
void (*RenderBufferedOneXLUSprite)(CVector pos, float w, float h, uint8_t r, uint8_t g, uint8_t b, short intens, float recipZ, uint8_t a);

float StarCoorsX[SSidesCount][AMOUNT_OF_SIDESTARS], StarCoorsY[SSidesCount][AMOUNT_OF_SIDESTARS], StarSizes[SSidesCount][AMOUNT_OF_SIDESTARS];
float fSmallStars, fMiddleStars, fBiggestStars, fBiggestStarsSpawnChance;
CVector PositionsTable[SSidesCount] =
{
    { 100.0f,  0.0f,   10.0f}, // Left
    {-100.0f,  0.0f,   10.0f}, // Right
    {   0.0f,  100.0f, 10.0f}, // Front
    {   0.0f, -100.0f, 10.0f}, // Back
    {   0.0f,  0.0f,   95.0f}, // Up
};