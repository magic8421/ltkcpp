//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once

namespace ltk {

typedef void* Cookie;

template<typename T>
struct DelegateNode
{
    DelegateNode<T> *next = nullptr;
    std::function<T> lambda;
};

template<typename T>
class Delegate
{
public:
    ~Delegate()
    {
        auto node = m_head;
        while (node) {
            auto tmp = node->next;
            delete node;
            node = tmp;
        }
    }

    Cookie Attach(const std::function<T> &cb)
    {
        auto node = new DelegateNode<T>;
        node->lambda = cb;
        if (m_head == nullptr) {
            m_head = node;
        }   
        else {
            auto p = m_head;
            while (p->next) {
                p = p->next;
            }
            p->next = node;
        }
        return node;
        // use void* instead of DelegateTracker
        // because it's safer to dereference the event source pointer and ->Remove()
    }

    void Remove(Cookie cookie)
    {
        if (cookie && !TryRemove(cookie)) {
            __debugbreak();
        }
    }

    bool TryRemove(Cookie cookie)
    {
        DelegateNode<T> *node = (DelegateNode<T> *)(cookie);
        if (!node) {
            return false;
        }
        if (!m_head) {
            return false;
        }
        if (m_head == node) {
            auto tmp = m_head->next;
            delete m_head;
            m_head = tmp;
        }
        auto p = m_head;
        while (p) {
            if (p->next == node) {
                p->next = p->next->next;
                delete node;
                return true;
            }
            p = p->next;
        }
        return false; // TODO FIXME
    }

    template<typename... Params>
    void Invoke(Params... params)
    {
        for (auto node = m_head; node; node = node->next) {
            (node->lambda)(std::forward<Params>(params)...);
        }
    }

private:
    DelegateNode<T> *m_head = nullptr;
};

} // namespace ltk
