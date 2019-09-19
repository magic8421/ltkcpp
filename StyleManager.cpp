//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StyleManager.h"
#include "ltk.h"
#include "Window.h"
#include <memory>

using std::unique_ptr;

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

ThemeData * StyleManager::m_sThemeData;

StyleManager::StyleManager()
{
}


StyleManager::~StyleManager()
{
    for (auto iter = m_mapBackgroundStyle.begin();
        iter != m_mapBackgroundStyle.end(); iter++) {
        delete iter->second;
    }
    for (auto iter = m_mapTextFormat.begin();
        iter != m_mapTextFormat.end(); iter++) {
        iter->second->Release();
    }
}

void StyleManager::NewTheme(LPCSTR name)
{
	LTK_ASSERT(name);
	if (!m_sThemeData) {
        m_sThemeData = new ThemeData;
    }
	if (m_sThemeData->MapTheme[name]) {
		//LTK_ASSERT(false);
		LTK_LOG("StyleManager::NewTheme theme already exists: %s", name);
		return;
	}
    m_sThemeData->MapTheme[name] = new StyleManager;
    m_sThemeData->CurrentTheme = name;
}

void StyleManager::SetCurrentTheme(LPCSTR name)
{
	LTK_ASSERT(name);
	m_sThemeData->CurrentTheme = name;
}

StyleManager * StyleManager::Instance()
{
    return m_sThemeData->MapTheme[m_sThemeData->CurrentTheme];
}

void StyleManager::Free()
{
    auto &map = m_sThemeData->MapTheme;
    for (auto &pair : map) {
        delete pair.second;
    }
    delete m_sThemeData;
}

D2D1_COLOR_F StyleManager::ColorFromString(const char *psz)
{
	int len = strlen(psz);
	if (len != 7 && len != 9) {
        LTK_ASSERT(false);
        return D2D1::ColorF(D2D1::ColorF::Cyan);
    }
    if (psz[0] != '#') {
        LTK_ASSERT(false);
        return D2D1::ColorF(D2D1::ColorF::Cyan);
    }
    long bin = strtoul(psz + 1, NULL, 16);
    D2D1_COLOR_F clr;

	if (len == 9) {
		clr.a = (bin & 0xFF) / 256.0f;
		bin >>= 8;
	} else {
		clr.a = 1.0f;
	}
    clr.b = (bin & 0xFF) / 256.0f;
    bin >>= 8;
    clr.g = (bin & 0xFF) / 256.0f;
    bin >>= 8;
    clr.r = (bin & 0xFF) / 256.0f;
    bin >>= 8;
    return clr;
}

AbstractBackground *StyleManager::GetBackground(const char *name) const
{
	LTK_ASSERT(name);
	std::string strName(name);
    auto iter = m_mapBackgroundStyle.find(strName);
    if (iter != m_mapBackgroundStyle.end()) {
        return iter->second;
    }
    else {
        LTK_ASSERT(false);
        return m_mapBackgroundStyle.begin()->second;
    }
}

void StyleManager::AddBackgroundStyle(const char *name, AbstractBackground *bg)
{
	LTK_ASSERT(name);
	std::string strName(name);
	auto iter = m_mapBackgroundStyle.find(strName);
	if (iter != m_mapBackgroundStyle.end()) {
		//LTK_ASSERT(false);
		LTK_LOG("StyleManager::AddBackgroundStyle style already exists: %s", name);
		delete bg;
		return;
	}
    m_mapBackgroundStyle[strName] = bg;
}

void StyleManager::RegisterColor(LPCSTR name, D2D1_COLOR_F color)
{
	LTK_ASSERT(name);
	std::string strName(name);
	auto iter = m_mapColor.find(strName);
	if (iter != m_mapColor.end()) {
		//LTK_ASSERT(false);
		LTK_LOG("StyleManager::RegisterColor() [%s] already exists.", name);
		return;
	}
	m_mapColor[strName] = color;
}

D2D1_COLOR_F StyleManager::GetColor(LPCSTR name)
{
	LTK_ASSERT(name);
	std::string strName(name);
	auto iter = m_mapColor.find(strName);
	if (iter == m_mapColor.end()) {
		LTK_ASSERT(false);
		LTK_LOG("StyleManager::GetColor() [%s] does not exist.", name);
		return D2D1::ColorF(D2D1::ColorF::Cyan);
	}
	return iter->second;
}

// TODO change to ** idom
IDWriteTextFormat * StyleManager::GetTextFormat(LPCSTR name)
{
	LTK_ASSERT(name);
	auto format = m_mapTextFormat[name];
    LTK_ASSERT(format);
    return format;
}

void StyleManager::AddTextFormat(LPCSTR name, IDWriteTextFormat *format)
{
	LTK_ASSERT(name);
	if (m_mapTextFormat[name] != nullptr) {
		//LTK_ASSERT(false);
		LTK_LOG("StyleManager::AddTextFormat [%s] already exists.", name);
		return;
	}
    m_mapTextFormat[name] = format;
    format->AddRef();
}

void StyleManager::AddTextFormat2(LPCSTR name, LPCWSTR font_family,
    DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, float size,
    DWRITE_TEXT_ALIGNMENT hAlign, DWRITE_PARAGRAPH_ALIGNMENT vAlign)
{
    HRESULT hr = E_FAIL;
    IDWriteTextFormat *format = nullptr;
    hr = GetDWriteFactory()->CreateTextFormat(
        font_family,
        NULL,
        weight,
        style,
        DWRITE_FONT_STRETCH_NORMAL,
        size,
        L"zh-cn",
        &format
    );
    LTK_ASSERT(SUCCEEDED(hr));
    
    hr = format->SetTextAlignment(hAlign);
    LTK_ASSERT(SUCCEEDED(hr));

    hr = format->SetParagraphAlignment(vAlign);
    LTK_ASSERT(SUCCEEDED(hr));

    this->AddTextFormat(name, format);
    format->Release();
}

RectF StyleManager::RectFromXml(tinyxml2::XMLElement *elm)
{
    RectF rc;
    rc.X = (float)elm->IntAttribute("x");
    rc.Y = (float)elm->IntAttribute("y");
    rc.Width = (float)elm->IntAttribute("w");
    rc.Height = (float)elm->IntAttribute("h");
    return rc;
}

Margin StyleManager::MarginFromXml(tinyxml2::XMLElement *elm)
{
    Margin margin;
    margin.left = (float)elm->IntAttribute("left");
    margin.top = (float)elm->IntAttribute("top");
    margin.right = (float)elm->IntAttribute("right");
    margin.bottom = (float)elm->IntAttribute("bottom");
    return margin;
}

bool StyleManager::TextureFromXml(tinyxml2::XMLElement *elm, TextureInfo *tex)
{
    auto atlas_elm = elm->FirstChildElement("Atlas");
    if (!atlas_elm) return false;
    tex->atlas = RectFromXml(atlas_elm);
    auto margin_elm = elm->FirstChildElement("Margin");
    if (!margin_elm) return false;
    tex->margin = MarginFromXml(margin_elm);

    return true;
}

bool StyleManager::LoadFromXml(LPCSTR file_name)
{
    using namespace tinyxml2;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(file_name) != XML_NO_ERROR) return false;
    auto style_elm = doc.FirstChildElement("Style");
    if (!style_elm) return false;

    auto nine_path_elm = style_elm->FirstChildElement("NinePatch");
    while (nine_path_elm) {
        auto normal_elm = nine_path_elm->FirstChildElement("Normal");
        if (!normal_elm) return false;
        unique_ptr<NinePatchBackground> npbg;
        npbg.reset(new NinePatchBackground);

        if (!TextureFromXml(normal_elm, &npbg->texNormal)) return false;

        auto hover_elm = nine_path_elm->FirstChildElement("Hover");
        if (!hover_elm) {
            npbg->texHover = npbg->texNormal;
        } else if (!TextureFromXml(hover_elm, &npbg->texHover)) {
            return false;
        }
        auto pressed_elm = nine_path_elm->FirstChildElement("Pressed");
        if (!pressed_elm) {
            npbg->texPressed = npbg->texNormal;
        } else if (!TextureFromXml(pressed_elm, &npbg->texPressed)) {
            return false;
        }
        auto disable_elm = nine_path_elm->FirstChildElement("Disable");
        if (!disable_elm) {
            npbg->texDisable = npbg->texNormal;
        } else if (!TextureFromXml(disable_elm, &npbg->texDisable)) {
            return false;
        }
        auto style_name = nine_path_elm->Attribute("name");
        if (!style_name) return false;
        this->AddBackgroundStyle(style_name, npbg.release());

        nine_path_elm = nine_path_elm->NextSiblingElement("NinePatch");
    }

    auto one_path_elm = style_elm->FirstChildElement("OnePatch");
    while (one_path_elm) {
        auto normal_elm = one_path_elm->FirstChildElement("Normal");
        if (!normal_elm) return false;

        unique_ptr<OnePatchBackground> opbg;
        opbg.reset(new OnePatchBackground);
        opbg->iconNormal.atlas = RectFromXml(normal_elm);

        auto hover_elm = one_path_elm->FirstChildElement("Hover");
        if (!hover_elm) {
            opbg->iconHover = opbg->iconNormal;
        } else {
            opbg->iconHover.atlas = RectFromXml(hover_elm);
        }
        auto press_elm = one_path_elm->FirstChildElement("Pressed");
        if (!press_elm) {
            opbg->iconPressed = opbg->iconNormal;
        } else {
            opbg->iconPressed.atlas = RectFromXml(press_elm);
        }
        auto disable_elm = one_path_elm->FirstChildElement("Disable");
        if (!disable_elm) {
            opbg->iconDisable = opbg->iconNormal;
        } else {
            opbg->iconDisable.atlas = RectFromXml(disable_elm);
        }

        auto style_name = one_path_elm->Attribute("name");
        if (!style_name) return false;
        this->AddBackgroundStyle(style_name, opbg.release());

        one_path_elm = one_path_elm->NextSiblingElement("OnePatch");
    }

	/*
    auto color_elm = style_elm->FirstChildElement("Color");
    while (color_elm) {
        auto name = color_elm->Attribute("name");
        auto value = color_elm->Attribute("value");
        if (!name || !value) return false;

        // TODO change to a map
        if (!strcmp(name, "TextNormal")) {
            m_colors[clrTextNormal] = ColorFromString(value);
        } else if (!strcmp(name, "TextHover")) {
            m_colors[clrTextHover] = ColorFromString(value);
        } else if (!strcmp(name, "TextCaption")) {
            m_colors[clrTextCaption] = ColorFromString(value);
        } else if (!strcmp(name, "ListBoxHover")) {
            m_colors[clrListBoxHover] = ColorFromString(value);
        } else if (!strcmp(name, "ListBoxSelected")) {
            m_colors[clrListBoxSelected] = ColorFromString(value);
        }

        color_elm = color_elm->NextSiblingElement("Color");
    }

    auto meauare_elm = style_elm->FirstChildElement("Measurement");
    while (meauare_elm) {
        auto name = meauare_elm->Attribute("name");
        auto value = (float)meauare_elm->IntAttribute("value");
        if (!name || !value) return false;

        // TODO change to a map
        if (!strcmp(name, "SysButtonWidth")) {
            m_measurements[mSysButtonWidth] = value;
        } else if (!strcmp(name, "SysButtonHeight")) {
            m_measurements[mSysButtonHeight] = value;
        } else if (!strcmp(name, "CaptionHeight")) {
            m_measurements[mCaptionHeight] = value;
        } 
        meauare_elm = meauare_elm->NextSiblingElement("Measurement");
    }
	*/
    return true;
}

bool StyleManager::IsDebuggingLayout()
{
    return m_bDebugLayout;
}

void StyleManager::SetDebuggingLayout(bool b)
{
    m_bDebugLayout = b;
}

void StyleManager::RegisterColorBulk(const ColorDesc *colors)
{
	for (UINT i = 0; colors[i].name; i++) {
		LTK_ASSERT(i < 999);
		RegisterColor(colors[i].name, ColorFromString(colors[i].color));
	}
}

void NinePatchBackground::Draw(Window *wnd, ID2D1RenderTarget *targe, const RectF &rc, State state, float blend)
{
    auto bmp = wnd->GetAtlasBitmap();
    TextureInfo *tex = nullptr;

    switch (state) {
    case State::Normal:
        tex = &texNormal;
        break;;
    case State::Hover:
        tex = &texHover;
        break;
    case State::Normal2Hover:
    case State::Hover2Normal:
        //LTK_LOG("blend: %f", blend);
        DrawTextureNineInOne(targe, bmp, texNormal.atlas, texNormal.margin, rc, 1.0f - blend, texNormal.scale);
        DrawTextureNineInOne(targe, bmp, texHover.atlas, texHover.margin, rc, blend, texHover.scale);
        return;
    case State::Pressed:
        tex = &texPressed;
        break;
    case State::Disable:
        tex = &texDisable;
        break;
    }
    DrawTextureNineInOne(targe, bmp, tex->atlas, tex->margin, rc, blend, tex->scale);
}

void OnePatchBackground::Draw(Window *wnd, ID2D1RenderTarget *targe, const RectF &rc, State state, float blend)
{
    auto bmp = wnd->GetAtlasBitmap();
    IconInfo *icon = nullptr;
    D2D1_BITMAP_INTERPOLATION_MODE interp_mode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;

    switch (state) {
    case State::Normal:
        icon = &iconNormal;
        break;
    case State::Hover:
        icon = &iconHover;
        break;
    case State::Normal2Hover:
    case State::Hover2Normal:
        targe->DrawBitmap(bmp, D2D1RectF(rc), 1.0f - blend,
            interp_mode,
            D2D1RectF(iconNormal.atlas));
        targe->DrawBitmap(bmp, D2D1RectF(rc), blend,
            interp_mode,
            D2D1RectF(iconHover.atlas));
        return;
    case State::Pressed:
        icon = &iconPressed;
        break;
    case State::Disable:
        icon = &iconDisable;
        break;
    }
    targe->DrawBitmap(bmp, D2D1RectF(rc), 1.0f,
        interp_mode,
        D2D1RectF(icon->atlas));
}

static D2D1_COLOR_F operator*(const D2D1_COLOR_F&lhs, float ratio)
{
    D2D1_COLOR_F ret;
    ret.a = lhs.a * ratio;
    ret.r = lhs.r * ratio;
    ret.g = lhs.g * ratio;
    ret.b = lhs.b * ratio;
    return ret;
}

static D2D1_COLOR_F operator+(const D2D1_COLOR_F&lhs, const D2D1_COLOR_F&rhs)
{
    D2D1_COLOR_F ret;
    ret.a = lhs.a + rhs.a;
    ret.r = lhs.r + rhs.r;
    ret.g = lhs.g + rhs.g;
    ret.b = lhs.b + rhs.b;
    return ret;
}

void RectangleBackground::Draw(Window *wnd, ID2D1RenderTarget *targe, const RectF &rc, State state, float blend)
{
    auto brush = wnd->GetStockBrush();
    D2D1_COLOR_F clrInner;
    D2D1_COLOR_F clrBorder;

    switch (state) {
    case Normal:
        clrInner = innerColors.clrNormal;
        clrBorder = borderColors.clrNormal;
        break;
    case Hover:
        clrInner = innerColors.clrHover;
        clrBorder = borderColors.clrHover;
        break;
    case Normal2Hover:
    case Hover2Normal:
        clrInner = innerColors.clrNormal * (1.0f - blend) + innerColors.clrHover * blend;
        clrBorder = borderColors.clrNormal * (1.0f - blend) + borderColors.clrHover * blend;
        break;;
    case Pressed:
        clrInner = innerColors.clrPressed;
        clrBorder = borderColors.clrPressed;
        break;
    case Disable:
        clrInner = innerColors.clrDisable;
        clrBorder = borderColors.clrDisable;
        break;
    }
    brush->SetColor(clrInner);
    RectF rc2 = rc;
    rc2.X += 0.5f;
    rc2.Y += 0.5f;
    rc2.Width -= 1.0f;
    rc2.Height -= 1.0f;
    targe->FillRectangle(D2D1RectF(rc2), brush);
    if (this->hasBorder) {
        brush->SetColor(clrBorder);
        targe->DrawRectangle(D2D1RectF(rc2), brush);
    }
}

void FourStateColor::SetColor(LPCSTR normal, LPCSTR hover, LPCSTR pressed, LPCSTR disable)
{
    this->clrNormal = StyleManager::ColorFromString(normal);
    this->clrHover = StyleManager::ColorFromString(hover);
    this->clrPressed = StyleManager::ColorFromString(pressed);
    this->clrDisable = StyleManager::ColorFromString(disable);
}

}
