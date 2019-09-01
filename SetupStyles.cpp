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

    NinePatchBackground *nine_in_one = nullptr;
    nine_in_one = new NinePatchBackground;
    nine_in_one->Normal() .SetAtals(4, 3, 21, 67) .SetMargin(8, 8, 8, 9);
    nine_in_one->Hover()  .SetAtals(54, 3, 21, 67).SetMargin(8, 8, 8, 9);
    nine_in_one->Pressed().SetAtals(79, 3, 21, 67).SetMargin(8, 8, 8, 9);
    nine_in_one->Disable().SetAtals(29, 3, 21, 67).SetMargin(8, 8, 8, 9);
    sm->AddBackgroundStyle("default_btn_bg", nine_in_one);

    nine_in_one = new NinePatchBackground;
    nine_in_one->Normal().SetAtals(125, 68, 6, 15).SetMargin(2, 3, 2, 3);
    nine_in_one->Hover() .SetAtals(142, 68, 6, 15).SetMargin(2, 3, 2, 3);
    nine_in_one->Pressed() = nine_in_one->Normal();
    nine_in_one->Disable() = nine_in_one->Normal();
    sm->AddBackgroundStyle("scrollbar_v", nine_in_one);

    nine_in_one = new NinePatchBackground;
    nine_in_one->Normal().SetAtals(121, 86, 15, 6).SetMargin(3, 2, 3, 2);
    nine_in_one->Hover() .SetAtals(138, 86, 15, 6).SetMargin(3, 2, 3, 2);
    nine_in_one->Pressed() = nine_in_one->Normal();
    nine_in_one->Disable() = nine_in_one->Normal();
    sm->AddBackgroundStyle("scrollbar_h", nine_in_one);

    nine_in_one = new NinePatchBackground;
    nine_in_one->Normal() .SetAtals(177, 10, 8, 23).SetMargin(3, 3, 3, 3);
    nine_in_one->Hover()  .SetAtals(189, 10, 8, 23).SetMargin(3, 3, 3, 3);
    nine_in_one->Pressed().SetAtals(201, 10, 8, 23).SetMargin(3, 3, 3, 3);
    nine_in_one->Disable() = nine_in_one->Normal();
    sm->AddBackgroundStyle("header_btn_bg", nine_in_one);

    nine_in_one = new NinePatchBackground;
    nine_in_one->Normal().SetAtals(2, 98, 39, 66).SetMargin(17, 45, 17, 17);
    nine_in_one->Hover() = nine_in_one->Normal();
    nine_in_one->Pressed() = nine_in_one->Normal();
    nine_in_one->Disable() = nine_in_one->Normal();
    sm->AddBackgroundStyle("window_bg", nine_in_one);

    OnePatchBackground *one_patch = new OnePatchBackground;
    one_patch->Normal() .SetAtlas(103, 3, 22, 20);
    one_patch->Hover()  .SetAtlas(103, 23, 22, 20);
    one_patch->Pressed().SetAtlas(103, 43, 22, 20);
    one_patch->Disable() = one_patch->Normal();
    sm->AddBackgroundStyle("min_btn_bg", one_patch);

    one_patch = new OnePatchBackground;
    one_patch->Normal() .SetAtlas(125, 3, 22, 20);
    one_patch->Hover()  .SetAtlas(125, 23, 22, 20);
    one_patch->Pressed().SetAtlas(125, 43, 22, 20);
    one_patch->Disable() = one_patch->Normal();
    sm->AddBackgroundStyle("max_btn_bg", one_patch);

    one_patch = new OnePatchBackground;
    one_patch->Normal() .SetAtlas(147, 3, 22, 20);
    one_patch->Hover()  .SetAtlas(147, 23, 22, 20);
    one_patch->Pressed().SetAtlas(147, 43, 22, 20);
    one_patch->Disable() = one_patch->Normal();
    sm->AddBackgroundStyle("close_btn_bg", one_patch);

	nine_in_one = new NinePatchBackground;
    nine_in_one->Normal().SetAtals(100, 70, 12, 12).SetMargin(2, 2, 2, 2); // TODO try zero size
    nine_in_one->Hover()  .SetAtals(171, 36, 18, 29).SetMargin(6, 6, 6, 6);
    nine_in_one->Pressed().SetAtals(193, 36, 18, 29).SetMargin(6, 6, 6, 6);
    nine_in_one->Disable() = nine_in_one->Normal();
    sm->AddBackgroundStyle("menu_bar_btn_bg", nine_in_one); // TODO change to menu_bar_btn_bg

	nine_in_one = new NinePatchBackground;
	nine_in_one->Normal().SetAtals(2, 126, 39, 38).SetMargin(18, 18, 18, 18);
	nine_in_one->Hover() = nine_in_one->Normal();
	nine_in_one->Pressed() = nine_in_one->Normal();
	nine_in_one->Disable() = nine_in_one->Normal();
	sm->AddBackgroundStyle("popup_menu_bg", nine_in_one);

    sm->AddTextFormat2("default_btn_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_LEADING,
        DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	sm->AddTextFormat2("default_label_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_CENTER,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	sm->AddTextFormat2("item_text_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_CENTER,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	sm->AddTextFormat2("tree_item_text_fmt", L"Î¢ÈíÑÅºÚ", DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, 14, DWRITE_TEXT_ALIGNMENT_LEADING,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    sm->AddTextFormat2("caption_fmt", L"ËÎÌå", DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, 12, DWRITE_TEXT_ALIGNMENT_LEADING,
        DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	const ColorDesc colors[] = {
		{ "default_btn_clr", "#362d01" },
		{ "default_label_clr", "#362d01" },
		{ "item_text_clr", "#362d01" },
		{ "item_hover_clr", "#fafad2" },
		{ "item_selected_clr", "#bbb073" },
		{ "item_selected_text_clr", "#eae9ce" },
		{ "caption_clr", "#fbfbf6" },
		//{ "", "" },
		{NULL, NULL}
	};
	sm->RegisterColorBulk(colors);
}

void SetupVectorStyle1()
{
    StyleManager *sm = StyleManager::Instance();

    RectangleBackground *rect_bg = new RectangleBackground;
    rect_bg->BorderColor().SetColor("#00cc00", "#00ff00", "#00aa00", "#cccccc");
    rect_bg->InnerColor().SetColor("#000000", "#003300", "#000000", "#cccccc");
    sm->AddBackgroundStyle("default_btn_bg", rect_bg);

    NinePatchBackground *nine_in_one = nullptr;

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

	const ColorDesc colors[] = {
		{ "default_btn_clr", "#00ff00" },
		{ "default_label_clr", "#00ff00" },
		{ "item_text_clr", "#00ff00" },
		{ "item_hover_clr", "#004400" },
		{ "item_selected_clr", "#00ff00" },
		{ "item_selected_text_clr", "#000000" },
		{ "caption_clr", "#00ff00" },
		//{ "", "" },
		{ NULL, NULL }
	};
	sm->RegisterColorBulk(colors);
}
