#pragma once

namespace ltk {

template<typename T>
class Delegate;

class IConnection
{
public:
    virtual void Close() = 0;
    virtual ~IConnection() {}
};

class DelegateTracker
{
public:
    DelegateTracker() {}
    explicit DelegateTracker(IConnection *conn) : m_conn(conn) {}
    DelegateTracker(DelegateTracker &&rhs) { m_conn = rhs.m_conn; rhs.m_conn = nullptr; }
    void operator=(DelegateTracker &&rhs) { m_conn = rhs.m_conn; rhs.m_conn = nullptr; }
    DelegateTracker(const DelegateTracker &rhs) = delete;
    void operator=(const DelegateTracker &rhs) = delete;
    ~DelegateTracker() {
        delete m_conn; 
    }

    void Disconnect() { if(m_conn) m_conn->Close(); }

private:
    IConnection * m_conn = nullptr;
};

template<typename T>
struct DelegateNode
{
    DelegateNode<T> *next = nullptr;
    std::function<T> lambda;
};

template<typename T>
class Connection : public IConnection
{
public:
    Connection(Delegate<T> *d, DelegateNode<T> *n) : m_d(d), m_n(n) {}
    ~Connection() {}
    void Close() { m_d->Remove(m_n); }

private:
    Delegate<T> *m_d;
    DelegateNode<T> *m_n;
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

    DelegateTracker Attach(const std::function<T> &cb)
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
        return std::move(DelegateTracker(new Connection<T>(this, node)));
        // TODO use void* instead of DelegateTracker
        // because it's safer to dereference the event source pointer and ->Remove()
    }

    bool Remove(DelegateNode<T> *node)
    {
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
