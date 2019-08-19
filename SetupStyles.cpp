//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SetupStyles.h"
#include "StyleManager.h"

using namespace ltk;

void SetupPixelStyle1()
{
    StyleManager *sm = StyleManager::Instance();

    NinePatchBackground *nine_in_one = new NinePatchBackground;
    nine_in_one->Normal() .SetAtals(4, 3, 21, 67) .SetMargin(8, 8, 8, 9);
    nine_in_one->Hover()  .SetAtals(54, 3, 21, 67).SetMargin(8, 8, 8, 9);
    nine_in_one->Pressed().SetAtals(79, 3, 21, 67).SetMargin(8, 8, 8, 9);
    nine_in_one->Disable().SetAtals(29, 3, 21, 67).SetMargin(8, 8, 8, 9);
    sm->AddBackgroundStyle("default_button", nine_in_one);

    nine_in_one = new NinePatchBackground;
    nine_in_one->Normal().SetAtals(142, 68, 6, 15).SetMargin(2, 3, 2, 3);
    nine_in_one->Hover() .SetAtals(125, 68, 6, 15).SetMargin(2, 3, 2, 3);
    nine_in_one->Pressed() = nine_in_one->Normal();
    nine_in_one->Disable() = nine_in_one->Normal();
    sm->AddBackgroundStyle("scrollbar_v", nine_in_one);

    nine_in_one = new NinePatchBackground;
    nine_in_one->Normal().SetAtals(138, 86, 15, 6).SetMargin(3, 2, 3, 2);
    nine_in_one->Hover() .SetAtals(121, 86, 15, 6).SetMargin(3, 2, 3, 2);
    nine_in_one->Pressed() = nine_in_one->Normal();
    nine_in_one->Disable() = nine_in_one->Normal();
    sm->AddBackgroundStyle("scrollbar_h", nine_in_one);

    nine_in_one = new NinePatchBackground;
    nine_in_one->Normal() .SetAtals(177, 10, 8, 23).SetMargin(3, 3, 3, 3);
    nine_in_one->Hover()  .SetAtals(189, 10, 8, 23).SetMargin(3, 3, 3, 3);
    nine_in_one->Pressed().SetAtals(201, 10, 8, 23).SetMargin(3, 3, 3, 3);
    nine_in_one->Disable() = nine_in_one->Normal();
    sm->AddBackgroundStyle("header_btn", nine_in_one);

    nine_in_one = new NinePatchBackground;
    nine_in_one->Normal().SetAtals(2, 98, 152, 167).SetMargin(40, 68, 40, 40);
    nine_in_one->Hover() = nine_in_one->Normal();
    nine_in_one->Pressed() = nine_in_one->Normal();
    nine_in_one->Disable() = nine_in_one->Normal();
    sm->AddBackgroundStyle("window_bg", nine_in_one);

    OnePatchBackground *one_patch = new OnePatchBackground;
    one_patch->Normal() .SetAtlas(103, 3, 22, 20);
    one_patch->Hover()  .SetAtlas(103, 23, 22, 20);
    one_patch->Pressed().SetAtlas(103, 43, 22, 20);
    one_patch->Disable() = one_patch->Normal();
    sm->AddBackgroundStyle("min_button", one_patch);

    one_patch = new OnePatchBackground;
    one_patch->Normal() .SetAtlas(125, 3, 22, 20);
    one_patch->Hover()  .SetAtlas(125, 23, 22, 20);
    one_patch->Pressed().SetAtlas(125, 43, 22, 20);
    one_patch->Disable() = one_patch->Normal();
    sm->AddBackgroundStyle("max_button", one_patch);

    one_patch = new OnePatchBackground;
    one_patch->Normal() .SetAtlas(147, 3, 22, 20);
    one_patch->Hover()  .SetAtlas(147, 23, 22, 20);
    one_patch->Pressed().SetAtlas(147, 43, 22, 20);
    one_patch->Disable() = one_patch->Normal();
    sm->AddBackgroundStyle("close_button", one_patch);
}
