#pragma once

#include "Sprite.h"

namespace ltk { 

class Button;
class Label;

class WindowLayout : public Sprite
{
public:
    RTTI_DECLARATIONS(WindowLayout, Sprite);
    WindowLayout() {}
    ~WindowLayout();

    Sprite *SetClientSprite(Sprite *sp);
    Sprite *GetClientSprite();

    void DoLayout();

    virtual bool OnSize(SizeEvent *ev) override;
    virtual void OnParentChanged(Sprite *old, Sprite *new_) override;

private:
    Sprite *m_client = nullptr;
    Button *m_minBtn = nullptr;
    Button *m_maxBtn = nullptr;
    Button *m_closeBtn = nullptr;
    DelegateTracker m_minTrack;
    DelegateTracker m_maxTrack;
    DelegateTracker m_closeTrack;
    Label *m_title = nullptr;
};

} // namespace ltk