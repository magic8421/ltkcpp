#pragma once

template<typename T>
class SinglyLinkedList
{
private:
    struct Node {
        Node(const T& _data) : next(nullptr), data(_data) {}
        Node(T&& _data) : next(nullptr), data(std::move(_data)) {}
        Node *next;
        T data;
    };
public:
    typedef void* Cookie;
    SinglyLinkedList() {}
    SinglyLinkedList(const SinglyLinkedList &) = delete;
    void operator=(const SinglyLinkedList &) = delete;

    ~SinglyLinkedList() {
        Clear();
        m_head = INVALID_POINTER(Node);
    }

    void Clear()
    {
        auto node = m_head;
        while (node) {
            auto next = node->next;
            delete node;
            node = next;
        }
        m_head = nullptr;
    }

    // add item to the end of list, O(n).
    Cookie PushBack(const T& data)
    {
        auto node = new Node(data);
        return PushBackRaw(node);
    }

    // add item to the end of list, O(n).
    Cookie PushBack(T&& data)
    {
        auto node = new Node(std::move(data));
        return PushBackRaw(node);
    }

    Cookie GetHead()
    {
        return reinterpret_cast<Cookie>(m_head);
    }

    bool Remove(Cookie cookie)
    {
        auto node = reinterpret_cast<Node*>(cookie);
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
            return true;
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
        return false;
    }

    bool IsEmpty()
    {
        return m_head == nullptr;
    }

    void ForEach(std::function<void(T)> visitor)
    {
        auto node = m_head;
        while (node) {
            visitor(node->data);
            node = node->next;
        }
    }

    T& GetData(Cookie cookie)
    {
        auto node = reinterpret_cast<Node*>(cookie);
        return node->data;
    }

    Cookie Next(Cookie cookie)
    {
        auto node = reinterpret_cast<Node*>(cookie);
        return reinterpret_cast<Cookie>(node->next);
    }

private:
    Cookie PushBackRaw(Node *node)
    {
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
    }

private:
    Node *m_head = nullptr;
};

template<typename T>
class ArrayList
{
public:
    ArrayList() : m_d(nullptr) {}
    ~ArrayList() 
    {
        Clear();
    }

    ArrayList(const ArrayList& rhs)
    {
        if (rhs.m_d) {
            size_t size = rhs.m_d->size;
            m_d = (Data *)malloc(
                sizeof(Data) + sizeof(T) * (size - 1));
            m_d->capacity = rhs.m_d->capacity;
            m_d->size = size;
            for (size_t i = 0; i < size; i++) {
                new (&m_d->data[i]) T(rhs.m_d->data[i]);
            }
        } else {
            m_d = nullptr;
        }
    }

    ArrayList(ArrayList &&rhs)
    {
        m_d = rhs.m_d;
        rhs.m_d = nullptr;
    }

    void PushBack(const T& v)
    {
        if (!m_d) {
            m_d = (Data *)malloc(sizeof(Data));
            m_d->capacity = 1;
            m_d->size = 1;
            new (&m_d->data[0]) T(v);
        }
        else {
            EnsureSpace();
            new (&m_d->data[m_d->size]) T(v);
            m_d->size++;
        }
    }

    void PopBack()
    {
        if (!m_d || m_d->size == 0) {
            __debugbreak();
        }
        m_d->data[m_d->size - 1].~T();
        m_d->size--;
    }

    void Insert(UINT idx, const T &v)
    {}

    void Erase(UINT idx)
    {}

    void Clear()
    {
        if (m_d) {
            for (size_t i = 0; i < m_d->size; i++) {
                m_d->data[i].~T();
            }
            free(m_d);
            m_d = nullptr;
        }
    }

    UINT Length()
    {
        if (m_d) {
            return m_d->size;
        }
        else {
            return 0;
        }
    }

    T &operator[](UINT i)
    {
        if (i >= m_d->size) {
            __debugbreak();
        }
        return m_d->data[i];
    }

private:
    struct Data {
        UINT size;
        UINT capacity;
        T data[1];
    };

    void EnsureSpace()
    {
        if (m_d->size == m_d->capacity) {
            UINT capacity = m_d->size + 1;
            capacity += capacity / 2;
            Data *new_d = (Data *)malloc(sizeof(Data) + sizeof(T) * (capacity - 1));
            for (size_t i = 0; i < m_d->size; i++) {
                new (&new_d->data[i]) T(std::move(m_d->data[i]));
            }
            // memcpy(new_d, m_d, sizeof(Data) + sizeof(T) * (m_d->size - 1));
            new_d->capacity = capacity;
            new_d->size = m_d->size;
            free(m_d);
            m_d = new_d;
        }
    }

    Data *m_d;
};