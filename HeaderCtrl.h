#pragma once

#include "Sprite.h"
#include "Button.h"

namespace ltk {

struct ComlunData {
    std::wstring name;
    float width = 100.0f;
    UINT index = (UINT)-1;
    Button *button = nullptr;
};

class HeaderCtrl : public Sprite
{
public:
    RTTI_DECLARATIONS(HeaderCtrl, Sprite);
    HeaderCtrl();
    virtual ~HeaderCtrl();

    void GetColumnWidth(std::vector<float> &vecColumns);
    void AddColumn(LPCWSTR name, float size);
    void DoLayout();

    virtual bool OnEvent(Event *ev) override;
    virtual bool OnSize(SizeEvent *ev) override;

private:
    std::vector<ComlunData> m_vecColumns;
};

}