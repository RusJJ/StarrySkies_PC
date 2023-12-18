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
extern bool bWideFix, bDisableStars;
extern float StarCoorsX[SSidesCount][AMOUNT_OF_SIDESTARS], StarCoorsY[SSidesCount][AMOUNT_OF_SIDESTARS], StarSizes[SSidesCount][AMOUNT_OF_SIDESTARS];
extern float fSmallStars, fMiddleStars, fBiggestStars, fBiggestStarsSpawnChance;
extern CVector PositionsTable[SSidesCount];
extern uint8_t nStarsHourStart, nStarsHourLast;

#define RandomIt(_min, _max) ((((float)rand()) / (float)RAND_MAX) * (_max - _min) + _min)

#endif