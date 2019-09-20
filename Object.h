#pragma once
#include "RTTI.h"

namespace ltk {

class Object;

class ObserverCtrl {
public:
	explicit ObserverCtrl(Object *p) : pobj(p) {}
	void AddRef() {
		InterlockedIncrement(&refCount);
	}
	void Release() {
		long ref = InterlockedDecrement(&refCount);
		if (ref == 0) {
			delete this;
		}
	}
	Object *Get() {
		return pobj;
	}
	void Set(Object *p){
		InterlockedExchangePointer((void**)&pobj, p);
	}

private:
	Object *pobj;
	long refCount = 1;
};

template<class T>
class Ptr {
public:
	explicit Ptr(ObserverCtrl *ctrl) :m_obctrl(ctrl) {
		m_obctrl->AddRef();
	}
	Ptr(const Ptr &rhs) {
		m_obctrl = rhs.m_obctrl;
		m_obctrl->AddRef();
	}
	Ptr operator=(const Ptr &rhs) {
		m_obctrl->Release();
		m_obctrl = rhs.m_obctrl;
		m_obctrl->AddRef();
		return *this;
	}
	template<class T2, 
	class = typename std::enable_if<std::is_convertible<T2 *, T *>::value, void>::type>
	Ptr(const Ptr<T2> &rhs) {
		Ptr *prhs = (Ptr<T> *)&rhs;
		m_obctrl = prhs->m_obctrl;
		m_obctrl->AddRef();
	}
	~Ptr() {
		m_obctrl->Release();
	}
	T *operator->() {
		return (T*)(m_obctrl->Get());
	}
	T *Get() {
		return (T*)(m_obctrl->Get());
	}
private:
	ObserverCtrl *m_obctrl;
};

template<class T>
class Owner {
public:
	~Owner() { delete m_ptr; }
	Owner() : m_ptr(nullptr) {}
	Owner(T* p) : m_ptr(p) {}
	Owner(const Owner& rhs) = delete;
	void operator=(const Owner& rhs) = delete;

	Owner(Owner &&rhs) {
		m_ptr = rhs.m_ptr;
		rhs.m_ptr = nullptr;
	}
	void operator=(Owner &&rhs) {
		m_ptr = rhs.m_ptr;
		rhs.m_ptr = nullptr;
	}
	T* operator->() {
		return m_ptr;
	}
	operator bool() {
		return m_ptr ? true : false;
	}
	bool operator==(const Owner& rhs) {
		return m_ptr == rhs.m_ptr;
	}
	T* reset(T *p) {
		T* old = m_ptr;
		m_ptr = p;
		return old;
	}
	T* reset() {
		delete m_ptr;
	}
	T* get() {
		return m_ptr;
	}
	Ptr<T> ToPtr(){
		return m_ptr->GetPtr<T>();
	}
private:
	T *m_ptr;
};

class LTK_API Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	Object();
	virtual ~Object();

	template<typename T>
	Ptr<T> GetPtr() {
		if (!this->Is(T::TypeIdClass())) {
			__debugbreak();
		}
		return Ptr<T>(m_obctrl);
	}

	void SetInvalid();

private:
	ObserverCtrl *m_obctrl = nullptr;
};

} // namespace ltk

