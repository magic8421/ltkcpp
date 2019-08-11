#pragma once

namespace ltk {

class AbstractAnimation
{
public:
    virtual ~AbstractAnimation() {}
    virtual bool IsRunning() = 0;
};

class ScrollAnimation : public AbstractAnimation
{
public:
    ScrollAnimation() {}
    ~ScrollAnimation() {}

    enum State
    {
        stStop, stScrollUp, stScrollDown
    };

    void BeginScroll(float delta);
    // return true if you have to call EndAnimation();
    bool UpdateScroll(float height);
    //
    float GetScroll() { return m_scroll; }
    void SetScroll(float pos);
    State GetState() { return m_state; }

    virtual bool IsRunning() override;

private:
    float m_scroll = 0.0f;
    float m_velocity = 0.0f;
    DWORD m_lastTick = 0;
    State m_state = stStop;

    const float ItemHeight = 25.0f;
    const float ScrollVelocity = ItemHeight * 3 / 500.0f;
};

} // namespace ltk
