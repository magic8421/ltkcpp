#pragma once

#include "resource.h"


#define  LTK_LOG(...) LtkLogImpl(__FILE__, __LINE__, __VA_ARGS__)

void LtkLogImpl(const char* source, int line, const char* format, ...);
