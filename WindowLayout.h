#pragma once

#include "Sprite.h"

namespace ltk { 

class Button;
class Label;

class WindowLayout : public Sprite
{
public:
    RTTI_DECLARATIONS(WindowLayout, Sprite);
    WindowLayout();
    ~WindowLayout();

    Sprite *SetClientSprite(Sprite *sp);
    Sprite *GetClientSprite();

    void DoLayout();
    void UpdateEventHandler();
    RectF GetCaptionRect();
    LPCWSTR GetCaptionText();
    void SetCaptionText(LPCWSTR text);

    virtual bool OnSize(SizeEvent *ev) override;

private:
    Sprite *m_client = nullptr;
    Button *m_minBtn = nullptr;
    Button *m_maxBtn = nullptr;
    Button *m_closeBtn = nullptr;
    DelegateTracker m_minTrack;
    DelegateTracker m_maxTrack;
    DelegateTracker m_closeTrack;
    Label *m_caption = nullptr;
};

} // namespace ltk