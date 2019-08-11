#pragma once

#include "ltk.h"
#include "RefCounted.h"

namespace ltk {

class Window;
class AbstractBackground;

class StyleManager
{
public:
    static StyleManager *Instance();
    static void Free();

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

    AbstractBackground *GetBackground(const char *name) const;
    bool AddBackgroundStyle(const char *name, AbstractBackground *bg);

    static RectF RectFromXml(tinyxml2::XMLElement *elm);
    static Margin MarginFromXml(tinyxml2::XMLElement *elm);
    static bool TextureFromXml(tinyxml2::XMLElement *elm, TextureInfo *tex);

    bool LoadFromXml(LPCSTR file_name);

    bool IsDebuggingLayout();
    void SetDebuggingLayout(bool);

private:
    StyleManager();
    ~StyleManager();
    static StyleManager *m_instance;

    bool m_bDebugLayout = false;
    std::vector<D2D1_COLOR_F> m_colors;
    std::vector<float> m_measurements;
    std::unordered_map<std::string, AbstractBackground*> m_mapBackgroundStyle;
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
    
    IconInfo iconNormal;
    IconInfo iconHover;
    IconInfo iconPressed;
    IconInfo iconDisable;
};


struct FourStateColor {
    D2D1_COLOR_F clrNormal;
    D2D1_COLOR_F clrHover;
    D2D1_COLOR_F clrPressed;
    D2D1_COLOR_F clrDisable;
};

class VectorBackground : public AbstractBackground
{
public:
    virtual void Draw(Window *wnd, ID2D1RenderTarget *targe, const RectF &rc,
        State state, float blend) override;

    FourStateColor borderColors;
    FourStateColor backgroundColors;
    // TODO add Gradient
    bool hasBorder = true;
    float roundCorner = 0.0f;
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

}
