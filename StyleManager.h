//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "ltk.h"
#include "RefCounted.h"

namespace ltk {

class Window;
class AbstractBackground;
class StyleManager;

struct ButtonStyle;
struct TreeViewStyle;

struct ThemeNode
{
    LPCSTR name;
    StyleManager* theme;
};

struct ThemeData
{
    std::vector<ThemeNode> MapTheme;
    LPCSTR CurrentTheme;
};

struct ColorDesc
{
	LPCSTR name;
	LPCSTR color;
};

class LTK_CPP_API StyleManager
{
public:
    static void NewTheme(LPCSTR name);
    static void SwitchTheme(LPCSTR name);
    static StyleManager *Instance();
    static void Free();

    static D2D1_COLOR_F ColorFromString(const char *psz);

    AbstractBackground *GetBackground(LPCSTR name) const;
    void AddBackgroundStyle(LPCSTR name, AbstractBackground *bg);

	void RegisterColor(LPCSTR name, D2D1_COLOR_F color);
	void RegisterColorBulk(const ColorDesc *colors);
	D2D1_COLOR_F GetColor(LPCSTR name);

	IDWriteTextFormat *GetTextFormat(LPCSTR name);
    void AddTextFormat(LPCSTR name, IDWriteTextFormat *format);
    void AddTextFormat2(LPCSTR name, LPCWSTR font_family, DWRITE_FONT_WEIGHT weight,
        DWRITE_FONT_STYLE style, float size, DWRITE_TEXT_ALIGNMENT hAlign,
        DWRITE_PARAGRAPH_ALIGNMENT vAlign);

	static std::vector<std::string> SplitString(const char *str, char c = ',');
    static RectF RectFromXml(tinyxml2::XMLElement *elm);
    static Margin MarginFromXml(tinyxml2::XMLElement *elm);
	static RectF RectFromString(LPCSTR str);
	static Margin MarginFromString(LPCSTR str);
    static bool TextureFromXml(tinyxml2::XMLElement *elm, TextureInfo *tex);

	void LoadNinePatchBackgroundFromXml(tinyxml2::XMLElement *root);
	void LoadOnePatchBackgroundFromXml(tinyxml2::XMLElement *root);
	void LoadTextFormatFromXml(tinyxml2::XMLElement *root);
	void LoadColorsFromXml(tinyxml2::XMLElement *root);
	bool LoadThemeXml(LPCSTR file_name);

    bool IsDebuggingLayout();
    void SetDebuggingLayout(bool);


private:
    StyleManager();
    ~StyleManager();
    static ThemeData *m_sThemeData;

    bool m_bDebugLayout = false;

    std::unordered_map<std::string, AbstractBackground*> m_mapBackgroundStyle;
	std::unordered_map<std::string, D2D1_COLOR_F> m_mapColor;
	std::unordered_map<std::string, IDWriteTextFormat*> m_mapTextFormat;
};

class LTK_CPP_API AbstractBackground
{
public:
    enum State {Normal, Hover, Normal2Hover, Hover2Normal, Pressed, Disable};
    virtual void Draw(Window *wnd, ID2D1RenderTarget *targe, const RectF &rc,
        State state, float blend) = 0;
    virtual ~AbstractBackground() {}
};

class LTK_CPP_API NinePatchBackground : public AbstractBackground
{

public:
    virtual void Draw(Window *wnd, ID2D1RenderTarget *targe, const RectF &rc,
        State state, float blend) override;
    virtual ~NinePatchBackground() {}

    TextureInfo& Normal() { return texNormal; }
    TextureInfo& Hover() { return texHover; }
    TextureInfo& Pressed() { return texPressed; }
    TextureInfo& Disable() { return texDisable; }

    TextureInfo texNormal;
    TextureInfo texHover;
    TextureInfo texPressed;
    TextureInfo texDisable;
};

class LTK_CPP_API OnePatchBackground : public AbstractBackground
{
public:
    virtual void Draw(Window *wnd, ID2D1RenderTarget *targe, const RectF &rc,
        State state, float blend) override;
    virtual ~OnePatchBackground() {}
    
    IconInfo &Normal() { return iconNormal; }
    IconInfo &Hover() { return iconHover; }
    IconInfo &Pressed() { return iconPressed; }
    IconInfo &Disable() { return iconDisable; }

    IconInfo iconNormal;
    IconInfo iconHover;
    IconInfo iconPressed;
    IconInfo iconDisable;
};


struct LTK_CPP_API FourStateColor {
    void SetColor(LPCSTR normal, LPCSTR hover, LPCSTR pressed, LPCSTR disable);

    D2D1_COLOR_F clrNormal;
    D2D1_COLOR_F clrHover;
    D2D1_COLOR_F clrPressed;
    D2D1_COLOR_F clrDisable;
};

class LTK_CPP_API RectangleBackground : public AbstractBackground
{
public:
    virtual void Draw(
        Window *wnd, ID2D1RenderTarget *targe, const RectF &rc,
        State state, float blend) override;
    virtual ~RectangleBackground() {}

    FourStateColor &BorderColor() { return borderColors; }
    FourStateColor &InnerColor() { return innerColors; }
	void SetMargin(float left, float top, float right, float bottom);

    FourStateColor borderColors;
    FourStateColor innerColors;
    // TODO add Gradient
    bool hasBorder = true;
    float roundCorner = -1.0f;
	Margin margin;
};

struct WindowStyle
{
    float SysButtonWidth = 20;
    float SysButtonHeight = 20;

    std::string MinimizeButtonStyle;
    std::string MaximizeButtonStyle;
    std::string CloseButtonStyle;

    bool HasMaximize = true;
    bool HasMinimize = true;
    bool HasClose = true;

    float BorderSize = 4;
    float CaptionHeight = 35;

    std::string IconStyle;
    float IconSize = 16;
    bool HasIcon = false;

private:
    DISALLOW_COPY_AND_ASSIGN(WindowStyle);
};


struct TreeViewStyle {
    D2D1_COLOR_F TextColor;
    D2D1_COLOR_F HoverColor;
    D2D1_COLOR_F SelectedColor;
    D2D1_COLOR_F SelectedTextColor;
};

}
