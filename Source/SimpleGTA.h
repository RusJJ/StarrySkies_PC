#pragma once

#include <stdint.h>
#include <math.h>

#define RwV2d CVector2D
#define RwV3d CVector

struct CVector2D
{
	float x, y;
};
struct CVector : CVector2D
{
	float z;
    inline CVector operator+(const CVector& vecTwo)
    {
        return { x + vecTwo.x, y + vecTwo.y, z + vecTwo.z };
    }
    inline CVector operator*(const CVector& vecTwo)
    {
        return { x * vecTwo.x, y * vecTwo.y, z * vecTwo.z };
    }
    inline CVector operator*(float f)
    {
        return { x * f, y * f, z * f };
    }
    inline void Normalise()
    {
        float sqMul = x * x + y * y + z * z;
        if (sqMul <= 0.0f) return;

        float recip = 1.0f / sqrtf(sqMul);
        x *= recip;
        y *= recip;
        z *= recip;
    }
};

class CMatrix
{
public:
    union
    {
        float f[4][4];
        struct
        {
            float rx, ry, rz, rw;
            float fx, fy, fz, fw;
            float ux, uy, uz, uw;
            float px, py, pz, pw;
        };
        struct
        {
            CVector      right;
            unsigned int flags;
            CVector      up;
            unsigned int pad1;
            CVector      at;
            unsigned int pad2;
            CVector      pos;
            unsigned int pad3;
        };
        struct // RwV3d style
        {
            CVector      m_right;
            unsigned int m_flags;
            CVector      m_forward;
            unsigned int m_pad1;
            CVector      m_up;
            unsigned int m_pad2;
            CVector      m_pos;
            unsigned int m_pad3;
        };
    };

    void*        m_pAttachedMatrix;
    bool         m_bOwnsAttachedMatrix;
    char         matrixpad[3];
};

struct RwIm3DVertex
{
    CVector pos, normal;
    uint32_t color;
    float u, v;
};