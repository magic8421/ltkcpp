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
struct ButtonStyle;
struct ListViewStyle;
class StyleManager;

struct ThemeData
{
    std::unordered_map<std::string, StyleManager *> MapTheme;
    std::string CurrentTheme;
};

class StyleManager
{
public:
    static void NewTheme(LPCSTR name);
    static StyleManager *Instance();
    static void Free();

    void Clear();

    enum Colors {
        clrTextNormal,
        clrTextHover,
        clrTextCaption,
        clrListBoxHover,
        clrListBoxSelected,
        clrLast
    };
    D2D1_COLOR_F GetColor(Colors clr);

    enum Measurement {
        mSysButtonWidth,
        mSysButtonHeight,
        mCaptionHeight,
        mWindowBorder, // TODO
        mWindowMaxBorder,
        mListHeaderHeight,
        mLast
    };
    float GetMeasurement(Measurement m);

    static D2D1_COLOR_F ColorFromString(const char *psz);

    AbstractBackground *GetBackground(LPCSTR name) const;
    void AddBackgroundStyle(LPCSTR name, AbstractBackground *bg);

    IDWriteTextFormat *GetTextFormat(LPCSTR name);
    void AddTextFormat(LPCSTR name, IDWriteTextFormat *format);
    void AddTextFormat2(LPCSTR name, LPCWSTR font_family, DWRITE_FONT_WEIGHT weight,
        DWRITE_FONT_STYLE style, float size, DWRITE_TEXT_ALIGNMENT hAlign,
        DWRITE_PARAGRAPH_ALIGNMENT vAlign);

    ButtonStyle *GetButtonStyle(LPCSTR name);
    void AddButtonStyle(LPCSTR name, ButtonStyle *style);

    ListViewStyle *GetListViewStyle(LPCSTR name);
    void AddListViewStyle(LPCSTR name, ListViewStyle *style);

    static RectF RectFromXml(tinyxml2::XMLElement *elm);
    static Margin MarginFromXml(tinyxml2::XMLElement *elm);
    static bool TextureFromXml(tinyxml2::XMLElement *elm, TextureInfo *tex);

    bool LoadFromXml(LPCSTR file_name);

    bool IsDebuggingLayout();
    void SetDebuggingLayout(bool);

private:
    StyleManager();
    ~StyleManager();
    static ThemeData *m_sThemeData;

    bool m_bDebugLayout = false;
    std::vector<D2D1_COLOR_F> m_colors;
    std::vector<float> m_measurements;
    std::unordered_map<std::string, AbstractBackground*> m_mapBackgroundStyle;
    std::unordered_map<std::string, IDWriteTextFormat*> m_mapTextFormat;
    std::unordered_map<std::string, ButtonStyle*> m_mapButtonStyle;
    std::unordered_map<std::string, ListViewStyle*> m_mapListViewStyle;
};

class AbstractBackground
{
public:
    enum State {Normal, Hover, Normal2Hover, Hover2Normal, Pressed, Disable};
    virtual void Draw(Window *wnd, ID2D1RenderTarget *targe, const RectF &rc,
        State state, float blend) = 0;
};

class NinePatchBackground : public AbstractBackground
{

public:
    virtual void Draw(Window *wnd, ID2D1RenderTarget *targe, const RectF &rc,
        State state, float blend) override;

    TextureInfo& Normal() { return texNormal; }
    TextureInfo& Hover() { return texHover; }
    TextureInfo& Pressed() { return texPressed; }
    TextureInfo& Disable() { return texDisable; }

    TextureInfo texNormal;
    TextureInfo texHover;
    TextureInfo texPressed;
    TextureInfo texDisable;
};

class OnePatchBackground : public AbstractBackground
{
public:
    virtual void Draw(Window *wnd, ID2D1RenderTarget *targe, const RectF &rc,
        State state, float blend) override;
    
    IconInfo &Normal() { return iconNormal; }
    IconInfo &Hover() { return iconHover; }
    IconInfo &Pressed() { return iconPressed; }
    IconInfo &Disable() { return iconDisable; }

    IconInfo iconNormal;
    IconInfo iconHover;
    IconInfo iconPressed;
    IconInfo iconDisable;
};


struct FourStateColor {
    void SetColor(LPCSTR normal, LPCSTR hover, LPCSTR pressed, LPCSTR disable);

    D2D1_COLOR_F clrNormal;
    D2D1_COLOR_F clrHover;
    D2D1_COLOR_F clrPressed;
    D2D1_COLOR_F clrDisable;
};

class RectangleBackground : public AbstractBackground
{
public:
    virtual void Draw(
        Window *wnd, ID2D1RenderTarget *targe, const RectF &rc,
        State state, float blend) override;

    FourStateColor &BorderColor() { return borderColors; }
    FourStateColor &InnerColor() { return innerColors; }

private:
    FourStateColor borderColors;
    FourStateColor innerColors;
    // TODO add Gradient
    bool hasBorder = true;
    float roundCorner = -1.0f;
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

struct ButtonStyle
{
    void SetStyle(LPCSTR background, LPCSTR text_format, LPCSTR text_color);

    std::string BackgroundStyle;
    std::string TextFormat;
    D2D1_COLOR_F TextColor;
};

struct ListViewStyle
{
    void SetColors(LPCSTR textColor, LPCSTR hoverColor, LPCSTR selectedColor,
        LPCSTR selectedTextColor);

    D2D1_COLOR_F TextColor;
    D2D1_COLOR_F HoverColor;
    D2D1_COLOR_F SelectedColor;
    D2D1_COLOR_F SelectedTextColor;
};

struct LableStyle
{
    D2D1_COLOR_F TextColor;
    std::string TextFormat;
};

}
