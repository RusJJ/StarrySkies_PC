#ifndef PCH_H
#define PCH_H

#include "framework.h"
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include "SimpleGTA.h"

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
extern float StarCoorsX[SSidesCount][AMOUNT_OF_SIDESTARS], StarCoorsY[SSidesCount][AMOUNT_OF_SIDESTARS], StarSizes[SSidesCount][AMOUNT_OF_SIDESTARS];
extern float fSmallStars, fMiddleStars, fBiggestStars, fBiggestStarsSpawnChance;
extern CVector PositionsTable[SSidesCount];

extern bool (*CalcScreenCoors)(CVector*, CVector*, float*, float*, bool, bool);
extern void (*RenderBufferedOneXLUSprite)(CVector pos, float w, float h, uint8_t r, uint8_t g, uint8_t b, short intens, float recipZ, uint8_t a);

extern CVector* CamPosPtr;
inline float RandomIt(float min, float max)
{
    return (((float)rand()) / (float)RAND_MAX) * (max - min) + min;
}

#endif