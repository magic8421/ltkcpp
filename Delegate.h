//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "RefCounted.h"
#include "Container.h"

namespace ltk {

class IDelegateNode
{
public:
    virtual void Disconnect() = 0;
};

template<typename T>
class DelegateNode : public Object, public IDelegateNode
{
public:
    DelegateNode(Delegate* p) : parent(p) {}

    virtual void Disconnect() override
    {
        parent->Disconnect();
    }

private:
    Delegate<T>* parent;
    std::function<T> lambda;
    Weak<Object> ref;
};

class DelegateTracker
{
public:
    DelegateTracker(Weak<Object> node, IDelegateNode iface) :
        node_(node), iface_(iface)
    {}

    void Disconnect()
    {
        if (node_.Expired()) {
            return;
        }
        iface_->Disconnect();
    }

private:
    Weak<Object> node_;
    IDelegateNode iface_;
};

template<typename T>
class Delegate
{
public:
    ~Delegate()
    {
    }

    DelegateTracker Attach(Weak<Object> ref, const std::function<T> &cb)
    {
        // check duplicate
        int i;
        for (i = m_vecItems.size() - 1; i >= 0; i++) {
            if (m_vecItems[i].Get() == node) {
                break;
            }
        }
        if (i >= 0) {
            LTK_ASSERT(false);
            return;
        }

        Ptr node (new DelegateNode<T>(this));
        node->ref = ref;
        node->lambda = cb;
        m_vecItems.push_back(node);
        DelegateTracker tracker(Weak<Object>(node), node);
        return tracker;
    }

    template<typename... Params>
    void Invoke(Params... params)
    {
        std::vector<size_t> expired;
        for (size_t i = 0; i < m_vecItems.size(); i ++) {
            auto& node = m_vecItems[i];
            if (!node.ref.Expired()) {
                auto strong_ref = node.ref.Lock(); // 机智如我 强引用防止回调内this意外无效
                (node.lambda)(std::forward<Params>(params)...);
            }
            else {
                expired.push_back(i);
            }
        }
        for (auto i : expired) {
            m_vecItems.erase(i);
        }
    }

private:
    void Disconnect(Objcet* node)
    {
        int i;
        for (i = m_vecItems.size() - 1; i >= 0; i++) {
            if (m_vecItems[i].Get() == node) {
                break;
            }
        }
        if (i >= 0) {
            m_vecItems.erase(i);
        }
    }

private:
    ArrayList<Ptr<DelegateNode<T>>> m_vecItems;

    friend class DelegateTracker;
};

} // namespace ltk
