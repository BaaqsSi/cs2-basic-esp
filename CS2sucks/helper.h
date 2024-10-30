#pragma once
#include "datatypes.h"
#include <cmath> 

class offsets {
public:
        //offsetebi sachiro
        int  dwViewMatrix = 0x1A33E30;
        int dwEntityList = 0x19D1A98;
        int dwLocalPlayerController = 0x1A219F0;


        //client dll
        int  m_hPlayerPawn = 0x80C;;
        int m_vOldOrigin = 0x1324;
        int teamnum = 0x3E3;
        int m_iHealth = 0x344;
        int  m_modelState = 0x170;
        int m_pGameSceneNode = 0x328;
        int m_skeletonInstance = 0x50;
        int m_vecViewOffset = 0xCB0;
        int m_vecOldViewAngles = 0x1C4;
    };



inline bool W2S(const Vec3& Vecorigin, Vec2& VecScreen, float* matrix) {
    VecScreen.X = Vecorigin.X * matrix[0] + Vecorigin.Y * matrix[1] + Vecorigin.Z * matrix[2] + matrix[3];
    VecScreen.Y = Vecorigin.X * matrix[4] + Vecorigin.Y * matrix[5] + Vecorigin.Z * matrix[6] + matrix[7];
    float W = Vecorigin.X * matrix[12] + Vecorigin.Y * matrix[13] + Vecorigin.Z * matrix[14] + matrix[15];

    if (W < 0.1f) {
        return false;
    }

    Vec2 NDC;
    NDC.X = VecScreen.X / W;
    NDC.Y = VecScreen.Y / W;

    VecScreen.X = (1920 / 2.0f) * (NDC.X + 1.0f);
    VecScreen.Y = (1080 / 2.0f) * (1.0f - NDC.Y);

    return true;
}

inline float CalculateDistance(const Vec2& a, const Vec2& b) {
    return sqrt(pow(b.X - a.X, 2) + pow(b.Y - a.Y, 2));
}

