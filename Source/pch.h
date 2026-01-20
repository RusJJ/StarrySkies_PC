#ifndef PCH_H
#define PCH_H

#include "framework.h"
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include "SimpleGTA.h"

#define CONFIG_FILENAME "StarrySkies"

#define AMOUNT_OF_SIDESTARS 100
enum eStarSides : uint8_t
{
    SSide_Left = 0,
    SSide_Right,
    SSide_Front,
    SSide_Back,
    SSide_Up,

    SSidesCount
};
extern bool bWideFix, bDisableStars, bDrawEasterEgg, bDrawFallingStar, bForceDisableFallingStar, bForceInteriorStars;
extern float StarCoorsX[SSidesCount][AMOUNT_OF_SIDESTARS], StarCoorsY[SSidesCount][AMOUNT_OF_SIDESTARS], StarSizes[SSidesCount][AMOUNT_OF_SIDESTARS];
extern float fSmallStars, fMiddleStars, fBiggestStars, fBiggestStarsSpawnChance, fRockStarEasterScale;
extern CVector PositionsTable[SSidesCount];
extern uint8_t nStarsHourStart, nStarsHourLast;
extern float RockStar_StarX[9], RockStar_StarY[9], RockStar_StarSize[9];
extern CVector RockStar_MainStarOff;
extern int16_t pShootingStarIndices[];
extern HMODULE hWideFix;

extern RwIm3DVertex Skies_TempBufferRenderVertices[32];

#define RandomIt(_min, _max) ((((float)rand()) / (float)RAND_MAX) * (_max - _min) + _min)

#endif