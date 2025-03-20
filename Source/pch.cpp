#include "pch.h"

bool bWideFix = true, bDisableStars = false, bDrawEasterEgg = true, bDrawFallingStar = true;
float StarCoorsX[SSidesCount][AMOUNT_OF_SIDESTARS], StarCoorsY[SSidesCount][AMOUNT_OF_SIDESTARS], StarSizes[SSidesCount][AMOUNT_OF_SIDESTARS];
float fSmallStars = 0.15f, fMiddleStars = 0.6f, fBiggestStars = 1.2f, fBiggestStarsSpawnChance = 20.0f;
uint8_t nStarsHourStart = 22, nStarsHourLast = 5;
CVector PositionsTable[SSidesCount] =
{
    { 100.0f,  0.0f,   10.0f}, // Left
    {-100.0f,  0.0f,   10.0f}, // Right
    {   0.0f,  100.0f, 10.0f}, // Front
    {   0.0f, -100.0f, 10.0f}, // Back
    {   0.0f,  0.0f,   95.0f}, // Up
};

// https://github.com/gta-reversed/gta-reversed/blob/f5cf5eb71ef9addf32470a7f0490ebba8eccceb3/source/game_sa/Clouds.cpp#L430
float RockStar_StarX[9]    = { 0.00f, 0.05f, 0.13f, 0.40f, 0.70f, 0.60f, 0.27f, 0.55f, 0.75f };
float RockStar_StarY[9]    = { 0.00f, 0.45f, 0.90f, 1.00f, 0.85f, 0.52f, 0.48f, 0.35f, 0.20f };
float RockStar_StarSize[9] = { 1.00f, 1.40f, 0.90f, 1.00f, 0.60f, 1.50f, 1.30f, 1.00f, 0.80f };
CVector RockStar_MainStarOff = { 100.f, -90.f, 10.f };
int16_t pShootingStarIndices[] = { 0, 1 };

RwIm3DVertex Skies_TempBufferRenderVertices[32] { 0 };
int Skies_TempBufferVerts = 0;