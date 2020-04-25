#pragma once
#include "LtkInterface.h"
#include "..\RTTI.h"

class DesignerWnd : public ltk::RTTI
{
public:
	RTTI_DECLARATIONS(DesignerWnd, ltk::RTTI);

	~DesignerWnd();

	void Create();
	void BuildMenu();

private:
	static int CALLBACK OnDestroy(void * userdata);
	static int CALLBACK OnClose(void * userdata, BOOL* proceed);
	static int CALLBACK OnAction(void* userdata, LPCSTR name);

	HLTK hWindow = NULL;
	HLTK hTreeView = NULL;
	HLTK hRootNode = NULL;
};

