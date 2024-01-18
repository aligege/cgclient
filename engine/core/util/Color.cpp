
//
// (c) 2021 Eduardo Doria.
//

#include "Color.h"

#include <math.h>

using namespace cg;

float Color::GAMMA = 2.2;
float Color::INV_GAMMA = 1.0 / GAMMA;

Vector3 Color::linearTosRGB(Vector3 color){
    return Vector3(pow(color.x, INV_GAMMA), pow(color.y, INV_GAMMA), pow(color.z, INV_GAMMA));
}

Vector4 Color::linearTosRGB(Vector4 color){
    return Vector4(linearTosRGB(Vector3(color.x, color.y, color.z)), color.w);
}

Vector3 Color::sRGBToLinear(Vector3 srgbIn){
    return Vector3(pow(srgbIn.x, GAMMA), pow(srgbIn.y, GAMMA), pow(srgbIn.z, GAMMA));
}

Vector4 Color::sRGBToLinear(Vector4 srgbIn){
    return Vector4(sRGBToLinear(Vector3(srgbIn.x, srgbIn.y, srgbIn.z)), srgbIn.w);
}