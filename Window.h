//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "stdafx.h"
#include "ShadowFrame.h"
#include "Event.h"
#include "ImeInput.h"
#include "MulticastDelegate.h"
#include "Object.h"

namespace ltk {

class Button;
class Label;
class WindowLayout;
class AbstractBackground;
class MenuBar;

class LTK_CPP_API Window : public Object, public ILtkWindow
{
public:
	RTTI_DECLARATIONS(Window, Object);

	/**
	 * 窗口构造函数
	 */
    Window(void);

	/**
	 * 窗口析构函数
	 */
    virtual ~Window(void);

	LTK_OBJECT_IMPL()

	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;

	STDMETHOD_(void, SetEventListener)(ILtkWindowListener* listener) override;

	/**
	 * 创建窗口
	 * @param parent 父窗口 如果为nullptr则父窗口为桌面
	 * @param rc 窗口的大小
	 */
    STDMETHOD(Create)(HWND hParent, LtkRect* rc) override;

	/**
	 * 创建窗口 并居中
	 * @param parent 父窗口 如果为nullptr则父窗口为桌面
	 * @param size 窗口的大小
	 */
	STDMETHOD(CreateCentered)(HWND hParent, LtkSize* size) override;

	/**
	 * 设置窗口矩形 移动窗口
	 * @param rc 窗口矩形以屏幕坐标系 单位是真实像素
	 */
	void SetRect(RectF rc);

	/**
	 * 获得窗口矩形
	 * @return 屏幕坐标系矩形 单位是真实像素
	 */
    RectF GetRect();

    SizeF GetClientSize();

    void SetCaption(LPCWSTR text);

	static void RegisterWndClass();

    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void OnPaint( HWND hwnd );

	STDMETHOD_(HWND, GetHWND)();

	void OnImeInput(LPCTSTR text);
	void SetImePosition( float x, float y );

	Widget *GetRootWidget();
	STDMETHOD_(void, SetClientWidget)(ILtkWidget* w);

	MenuBar *SetMenu(MenuBar *);
	MenuBar *GetMenu();

	void SetFocusWidget(Widget *sp);
	Widget *GetFocusWidget();
	void DisableFocusChange();

	void SetCaretHeight(float);

	void ShowCaret();
	void HideCaret();

	void SetCapture(Widget *sp);
	void ReleaseCapture();
    bool IsCapturing(Widget *sp);

    void TrackMouseLeave(Widget *sp);

    void BeginAnimation(Widget *sp);
    void EndAnimation(Widget *sp);

    void Minimize();
    void Maximize();
	void CloseWindow();

    virtual bool OnSize(float cx, float cy, DWORD flag);
	virtual BOOL OnClose() { return FALSE; } // 返回TRUE阻止关闭窗口
	virtual void OnDestroy();

    ID2D1Bitmap *GetAtlasBitmap();
    ID2D1SolidColorBrush *GetStockBrush();
    void SetBackground(LPCSTR style);
    
	STDMETHOD_(void, UpdateTheme)();
    virtual void OnThemeChanged() {}

public:
	MulticastDelegate0 DestroyDelegate;

private:
	void HandleMouseMessage(UINT message, WPARAM wparam, LPARAM lparam);
    void HandleMouseLeave();
    void DrawNonClient();
    void RecreateResouce();
    void UpdateShadowFrame(bool bRedraw);
	Events TranslateMessageCode(UINT message);

    LRESULT OnImeEvent(UINT message, WPARAM wparam, LPARAM lparam);
    LRESULT HandleNcHitTest(const POINT &pt);
    enum { TIMER_ANIMATION = 100 };
    static const wchar_t * ClsName;

private:
	HWND m_hwnd = NULL;

	ImeInput m_ime;
	RECT m_rectComposition;
	int m_caretHeight;

	WindowLayout *m_root = nullptr; // owner

	bool m_bEnableFocusChange = true;
	Widget *m_spFocus = nullptr;
	Widget *m_spCapture = nullptr;
	Widget *m_spHover = nullptr;
	std::unordered_set<Widget *> m_setTrackMouseLeave;
    std::unordered_set<Widget *> m_setAnimation;

    ID2D1HwndRenderTarget *m_target = nullptr; // owner
    ID2D1SolidColorBrush *m_brush = nullptr; // owner

    ShadowFrame m_shadowLeft;
    ShadowFrame m_shadowTop;
    ShadowFrame m_shadowRight;
    ShadowFrame m_shadowBottom;

    ID2D1Bitmap *m_atlas = nullptr; // owner TODO share across multiple Window
    AbstractBackground *m_background = nullptr;
    std::string m_styleName;

	ILtkWindowListener *m_listener = nullptr;
};

} // namespace ltk