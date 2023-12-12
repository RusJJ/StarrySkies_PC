#include "pch.h"

bool bWideFix = true, bDisableStars = false;
float StarCoorsX[SSidesCount][AMOUNT_OF_SIDESTARS], StarCoorsY[SSidesCount][AMOUNT_OF_SIDESTARS], StarSizes[SSidesCount][AMOUNT_OF_SIDESTARS];
float fSmallStars = 0.15f, fMiddleStars = 0.6f, fBiggestStars = 1.2f, fBiggestStarsSpawnChance = 20.0f;
CVector PositionsTable[SSidesCount] =
{
    { 100.0f,  0.0f,   10.0f}, // Left
    {-100.0f,  0.0f,   10.0f}, // Right
    {   0.0f,  100.0f, 10.0f}, // Front
    {   0.0f, -100.0f, 10.0f}, // Back
    {   0.0f,  0.0f,   95.0f}, // Up
};