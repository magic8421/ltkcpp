//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SetupStyles.h"
#include "StyleManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif


using namespace ltk;

void SetupPixelStyle1()
{
    StyleManager *sm = StyleManager::Instance();

    //sm->AddTextFormat2("default_btn_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
    //    DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_LEADING,
    //    DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	//sm->AddTextFormat2("default_label_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
	//	DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_CENTER,
	//	DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	//sm->AddTextFormat2("item_text_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
	//	DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_CENTER,
	//	DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	//sm->AddTextFormat2("tree_item_text_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
	//	DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_LEADING,
	//	DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

 //   sm->AddTextFormat2("caption_fmt", L"ËÎÌå", DWRITE_FONT_WEIGHT_NORMAL,
 //       DWRITE_FONT_STYLE_NORMAL, 12, DWRITE_TEXT_ALIGNMENT_LEADING,
 //       DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	//sm->AddTextFormat2("popup_menu_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
	//	DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_LEADING,
	//	DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void SetupVectorStyleDark()
{
    StyleManager *sm = StyleManager::Instance();

    RectangleBackground *rect_bg = new RectangleBackground;
    rect_bg->BorderColor().SetColor("#00cc00", "#00ff00", "#00aa00", "#cccccc");
    rect_bg->InnerColor().SetColor("#000000", "#003300", "#000000", "#cccccc");
    sm->AddBackgroundStyle("default_btn_bg", rect_bg);

    rect_bg = new RectangleBackground;
    rect_bg->BorderColor().SetColor("#00cc00", "#00ff00", "#00aa00", "#cccccc");
    rect_bg->InnerColor().SetColor("#000000", "#003300", "#000000", "#cccccc");
    sm->AddBackgroundStyle("scrollbar_h", rect_bg);


    rect_bg = new RectangleBackground;
    rect_bg->BorderColor().SetColor("#00cc00", "#00ff00", "#00aa00", "#cccccc");
    rect_bg->InnerColor().SetColor("#000000", "#003300", "#000000", "#cccccc");
    sm->AddBackgroundStyle("scrollbar_v", rect_bg);

    rect_bg = new RectangleBackground;
    rect_bg->BorderColor().SetColor("#00cc00", "#00ff00", "#00aa00", "#cccccc");
    rect_bg->InnerColor().SetColor("#000000", "#003300", "#000000", "#cccccc");
    sm->AddBackgroundStyle("header_btn_bg", rect_bg);

    rect_bg = new RectangleBackground;
	rect_bg->BorderColor().SetColor("#00cc00", "#00ff00", "#00aa00", "#cccccc");
	rect_bg->InnerColor().SetColor("#000000", "#003300", "#000000", "#cccccc");
    sm->AddBackgroundStyle("window_bg", rect_bg);


    OnePatchBackground *one_patch = new OnePatchBackground;
    one_patch->Normal() .SetAtlas(215, 4, 22, 18);
    one_patch->Hover()  .SetAtlas(215, 24, 22, 18);
    one_patch->Pressed().SetAtlas(215, 44, 22, 18);
    one_patch->Disable() = one_patch->Normal();
    sm->AddBackgroundStyle("min_btn_bg", one_patch);

    one_patch = new OnePatchBackground;
    one_patch->Normal() .SetAtlas(237, 4, 21, 18);
    one_patch->Hover()  .SetAtlas(237, 24, 21, 18);
    one_patch->Pressed().SetAtlas(237, 44, 21, 18);
    one_patch->Disable() = one_patch->Normal();
    sm->AddBackgroundStyle("max_btn_bg", one_patch);

    one_patch = new OnePatchBackground;
    one_patch->Normal() .SetAtlas(258, 4, 21, 18);
    one_patch->Hover()  .SetAtlas(258, 24, 21, 18);
    one_patch->Pressed().SetAtlas(258, 44, 21, 18);
    one_patch->Disable() = one_patch->Normal();
    sm->AddBackgroundStyle("close_btn_bg", one_patch);

	rect_bg = new RectangleBackground;
	rect_bg->BorderColor().SetColor("#000000", "#00ff00", "#00aa00", "#cccccc");
	rect_bg->InnerColor().SetColor("#000000", "#003300", "#000000", "#cccccc");
	sm->AddBackgroundStyle("menu_bar_btn_bg", rect_bg);

	rect_bg = new RectangleBackground;
	rect_bg->BorderColor().SetColor("#00cc00", "#00ff00", "#00aa00", "#cccccc");
	rect_bg->InnerColor().SetColor("#000000", "#003300", "#000000", "#cccccc");
	sm->AddBackgroundStyle("popup_menu_bg", rect_bg);

    sm->AddTextFormat2("default_btn_fmt", L"ËÎÌå", DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	sm->AddTextFormat2("default_label_fmt", L"ËÎÌå", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_CENTER,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	sm->AddTextFormat2("item_text_fmt", L"ËÎÌå", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 12, DWRITE_TEXT_ALIGNMENT_CENTER,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	sm->AddTextFormat2("tree_item_text_fmt", L"ËÎÌå", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 12, DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    sm->AddTextFormat2("caption_fmt", L"ËÎÌå", DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, 12, DWRITE_TEXT_ALIGNMENT_LEADING,
        DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	sm->AddTextFormat2("popup_menu_fmt", L"ËÎÌå", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	const ColorDesc colors[] = {
		{ "default_btn_clr", "#00ff00" },
		{ "default_label_clr", "#00ff00" },
		{ "item_text_clr", "#00ff00" },
		{ "item_hover_clr", "#004400" },
		{ "item_selected_clr", "#00ff00" },
		{ "item_selected_text_clr", "#000000" },
		{ "caption_clr", "#00ff00" },
		{ "menu_hover_clr", "#004400" },
		{ NULL, NULL }
	};
	sm->RegisterColorBulk(colors);
}

void SetupVectorStyleLight()
{
	StyleManager *sm = StyleManager::Instance();

	RectangleBackground *rect_bg = new RectangleBackground;
	rect_bg->BorderColor().SetColor("#00cc00", "#00ff00", "#00aa00", "#cccccc");
	rect_bg->InnerColor().SetColor("#a1d4d9", "#cbebee", "#6ca7ad", "#aeaeae");
	rect_bg->hasBorder = false;
	rect_bg->roundCorner = 5.f;
	sm->AddBackgroundStyle("default_btn_bg", rect_bg);

	rect_bg = new RectangleBackground;
	rect_bg->BorderColor().SetColor("#00cc00", "#00ff00", "#00aa00", "#cccccc");
	rect_bg->InnerColor().SetColor("#a1d4d9", "#cbebee", "#6ca7ad", "#aeaeae");
	rect_bg->hasBorder = false;
	sm->AddBackgroundStyle("scrollbar_h", rect_bg);


	rect_bg = new RectangleBackground;
	rect_bg->BorderColor().SetColor("#00cc00", "#00ff00", "#00aa00", "#cccccc");
	rect_bg->InnerColor().SetColor("#a1d4d9", "#cbebee", "#6ca7ad", "#aeaeae");
	rect_bg->hasBorder = false;
	sm->AddBackgroundStyle("scrollbar_v", rect_bg);

	rect_bg = new RectangleBackground;
	rect_bg->BorderColor().SetColor("#19767f", "#19767f", "#19767f", "#cccccc");
	rect_bg->InnerColor().SetColor("#a1d4d9", "#cbebee", "#6ca7ad", "#aeaeae");
	sm->AddBackgroundStyle("header_btn_bg", rect_bg);

	rect_bg = new RectangleBackground;
	rect_bg->BorderColor().SetColor("#084248", "#084248", "#084248", "#cccccc");
	rect_bg->InnerColor().SetColor("#fbfefe", "#fbfefe", "#fbfefe", "#cccccc");
	sm->AddBackgroundStyle("window_bg", rect_bg);


	OnePatchBackground *one_patch = new OnePatchBackground;
	one_patch->Normal().SetAtlas(215, 4, 22, 18);
	one_patch->Hover().SetAtlas(215, 24, 22, 18);
	one_patch->Pressed().SetAtlas(215, 44, 22, 18);
	one_patch->Disable() = one_patch->Normal();
	sm->AddBackgroundStyle("min_btn_bg", one_patch);

	one_patch = new OnePatchBackground;
	one_patch->Normal().SetAtlas(237, 4, 21, 18);
	one_patch->Hover().SetAtlas(237, 24, 21, 18);
	one_patch->Pressed().SetAtlas(237, 44, 21, 18);
	one_patch->Disable() = one_patch->Normal();
	sm->AddBackgroundStyle("max_btn_bg", one_patch);

	one_patch = new OnePatchBackground;
	one_patch->Normal().SetAtlas(258, 4, 21, 18);
	one_patch->Hover().SetAtlas(258, 24, 21, 18);
	one_patch->Pressed().SetAtlas(258, 44, 21, 18);
	one_patch->Disable() = one_patch->Normal();
	sm->AddBackgroundStyle("close_btn_bg", one_patch);

	rect_bg = new RectangleBackground;
	rect_bg->BorderColor().SetColor("#000000", "#00ff00", "#00aa00", "#cccccc");
	rect_bg->InnerColor().SetColor("#fbfefe", "#cbebee", "#6ca7ad", "#cccccc");
	rect_bg->hasBorder = false;
	rect_bg->roundCorner = 5.f;
	sm->AddBackgroundStyle("menu_bar_btn_bg", rect_bg);

	rect_bg = new RectangleBackground;
	rect_bg->BorderColor().SetColor("#00cc00", "#00ff00", "#00aa00", "#cccccc");
	rect_bg->InnerColor().SetColor("#cbebee", "#cbebee", "#cbebee", "#cccccc");
	rect_bg->SetMargin(7, 7, 7, 7);
	rect_bg->hasBorder = false;
	rect_bg->roundCorner = 5.f;
	sm->AddBackgroundStyle("popup_menu_bg", rect_bg);

	sm->AddTextFormat2("default_btn_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	sm->AddTextFormat2("default_label_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_CENTER,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	sm->AddTextFormat2("item_text_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 12, DWRITE_TEXT_ALIGNMENT_CENTER,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	sm->AddTextFormat2("tree_item_text_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 12, DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	sm->AddTextFormat2("caption_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 12, DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	sm->AddTextFormat2("popup_menu_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	const ColorDesc colors[] = {
		{ "default_btn_clr", "#000000" },
		{ "default_label_clr", "#000000" },
		{ "item_text_clr", "#000000" },
		{ "item_hover_clr", "#fafad2" },
		{ "item_selected_clr", "#bbb073" },
		{ "item_selected_text_clr", "#eae9ce" },
		{ "caption_clr", "#000000" },
		{ "menu_hover_clr", "#fafad277" },
		{ NULL, NULL }
	};
	sm->RegisterColorBulk(colors);
}

