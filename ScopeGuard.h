#pragma once

namespace ltk {
    
template <typename F> class ScopeGuard;
template <typename F> ScopeGuard<F> LtkScopeGuard(F f);

template <typename F>
class ScopeGuard
{
public:
    ScopeGuard(ScopeGuard &&other) noexcept
        : m_func(std::move(other.m_func))
        , m_invoke(other.m_invoke)
    {
        other.m_invoke = false;
    }
    ~ScopeGuard()
    {
        if (m_invoke)
            m_func();
    }
    void dismiss() noexcept
    {
        m_invoke = false;
    }
private:
    explicit ScopeGuard(F &&f) noexcept
        : m_func(std::move(f))
    {
    }
    ScopeGuard(const ScopeGuard&) = delete;
    void operator=(const ScopeGuard&) = delete;

    F m_func;
    bool m_invoke = true;
    friend ScopeGuard LtkScopeGuard<F>(F);
};

template <typename F>
ScopeGuard<F> LtkScopeGuard(F f)
{
    return ScopeGuard<F>(std::move(f));
}

} //namespace ltk
