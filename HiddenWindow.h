#pragma once

namespace ltk {

class HiddenWindow
{
public:
	static void Init();
	static LRESULT CALLBACK MyWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	static void PostDeleteLater(Object* obj);
};

} // namespace ltk