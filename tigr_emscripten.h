#pragma once
#include "tigr.h"

void tigrPlatformInit(Tigr* bmp);
void tigrPlatformPresent(Tigr* bmp);
void tigrSetMainLoop(void (*fn)(void));

