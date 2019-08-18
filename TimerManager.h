//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

namespace ltk {

struct TimerNode {
    std::function<void()> callback;
    bool isOnceTimer = true;
};

class TimerManager
{
public:
    static TimerManager *Instance();
    static void Free();

    UINT SetTimer(const std::function<void()>&cb, UINT elapse, bool bOnce);

private:
    TimerManager();
    ~TimerManager();
    static TimerManager *m_instance;

    static LRESULT CALLBACK MyWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
    void OnTimer(UINT id);

    HWND m_hwnd = 0;
    std::unordered_map<UINT, TimerNode *> m_mapCallback;
};

UINT SetTimer(const std::function<void()>&cb, UINT elapse);
UINT SetOnceTimer(const std::function<void()>&cb, UINT elapse);

} // namespace ltk
