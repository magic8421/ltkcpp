//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

namespace ltk {

class ShadowFrame
{
public:
    enum Mode {
        eLeft, eTop, eRight, eBottom
    };
    explicit ShadowFrame(Mode m);
    ~ShadowFrame();

    static void Init();
    static void Free();
    void Create();
	void SetParent(HWND hParent);
    void Destroy();
    void Update(HWND hParent, HDWP &hdwp, bool bRedraw);
    void OnDraw(const RECT &rc);
    void DrawShadow(Gdiplus::Graphics &g, Gdiplus::Rect rc);
	void SetCaption(LPCWSTR);

    static void RegisterWndClass();
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT HandleMouseMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void HandleMouseMove(long x, long y);

private:
    HWND m_hwnd = 0;
	HWND m_hParent = 0;
    Mode m_mode = eLeft;
	std::wstring m_caption;
	Gdiplus::Font *m_font = nullptr;
    static Gdiplus::Bitmap *m_bitmap;
	bool m_bCapture = false;
	POINT m_ptClick;
	RECT m_oldRc;

	enum Action
	{
		aNone, aCaption, aLeft, aLeftTop, aRightTop
	};
	Action m_action = aNone;
};

}
