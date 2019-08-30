#pragma once

#include "RTTI.h"
#include "Common.h"
#include "stdafx.h"

namespace ltk {

class RefCounted : public RTTI
{
public:
    RefCounted()
    {
        m_refCount = 1;
    }

    void AddRef()
    {
        m_refCount++;
    }
    void Release()
    {
        m_refCount--;
        if (m_refCount < 0)
        {
			LTK_ASSERT(false);
        }
        if (m_refCount == 0)
        {
            delete this;
        }
    }
    int RefCount()
    {
        return m_refCount;
    }
    bool HasOneRef()
    {
        return m_refCount == 1;
    }

    RTTI_DECLARATIONS(RefCounted, RTTI);

protected:
	virtual ~RefCounted() {}

private:
    DISALLOW_COPY_AND_ASSIGN(RefCounted);
    int m_refCount;
};

template<typename T>
class RefPtr
{
public:
    RefPtr() : m_ptr(nullptr) {
        //static_assert(std::is_convertible<T*, RefCounted*>::value,
        //    "RefCounted * required!");
    }
    explicit RefPtr(T *ptr) : m_ptr(ptr) {
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    RefPtr(const RefPtr &rhs) {
        m_ptr = rhs.m_ptr;
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    RefPtr(RefPtr &&rhs) {
        m_ptr = rhs.m_ptr;
        rhs.m_ptr = nullptr;
    }

    ~RefPtr() {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = reinterpret_cast<T*>(0xbaadf00d);
    }
    //Attach to an existing RefCounted (does not AddRef)
    void Attach(T *p) {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = p;
    }

    T *Detach() {
        T *tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }

    T *operator->() {
        return m_ptr;
    }

    operator bool() {
        return m_ptr != nullptr;
    }

    void operator=(const RefPtr &rhs) {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = rhs.m_ptr;
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    void operator=(RefPtr &&rhs) {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = rhs.m_ptr;
        rhs.m_ptr = nullptr;
    }

    void operator=(T *p)
    {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = p;
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    template <typename Q>
    T* operator=(const RefPtr<Q>& ptr) throw()
    {
        if (ptr->Is<T>()) {
            if (m_ptr) {
                m_ptr->Release();
            }
            m_ptr = ptr->As<T>();
        }
    }

    template <typename Q>
    operator RefPtr<Q>()
    {
        Q *q = static_cast<Q*>(m_ptr);
        return RefPtr<Q>(q);
    }

    T *Get() {
        return m_ptr;
    }

    void Reset(T *p)
    {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = p;
        if (m_ptr) {
            m_ptr->AddRef();
        }
    }

    void Reset()
    {
        if (m_ptr) {
            m_ptr->Release();
        }
        m_ptr = nullptr;
    }

    T **GetAddress()
    {
        return &m_ptr;
    }

private:
    T *m_ptr;
};

} // namespace ltk