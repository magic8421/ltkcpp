#pragma once
#include "LtkObject.h"

class LtkWidget : public LtkObject
{
public:
	explicit LtkWidget(HLTK hltk) : LtkObject(hltk)
	{}
};

