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

    UINT SetTimer(UINT id, const std::function<void()>&cb, UINT elapse, bool bOnce);
    void KillTimer(UINT id);

private:
    TimerManager();
    ~TimerManager();
    static TimerManager *m_instance;

    static LRESULT CALLBACK MyWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
    void OnTimer(UINT id);

    HWND m_hwnd = 0;
    std::unordered_map<UINT, TimerNode *> m_mapCallback;
};

UINT SetTimer(UINT elapse, UINT id, const std::function<void()>&cb);
UINT SetOnceTimer(UINT elapse, UINT id, const std::function<void()>&cb);
void KillTimer(UINT id);

} // namespace ltk
