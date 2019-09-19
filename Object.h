#pragma once
#include "RTTI.h"

namespace ltk {

class Object;

class ObserverCtrl {
public:
	void AddRef() {
		refCount++;
	}
	void Release() {
		refCount--;
		if (refCount == 0) {
			delete this;
		}
	}
	Object *Get() {
		return pobj;
	}
	void Set(Object *p){
		pobj = p;
	}

private:
	Object *pobj;
	long refCount = 1;
};

template<typename T>
class Observer {
public:
	explicit Observer(ObserverCtrl *ctrl) :m_obctrl(ctrl) {
		m_obctrl->AddRef();
	}
	Observer(const Observer &rhs) {
		m_obctrl = rhs.m_obctrl;
		m_obctrl->AddRef();
	}
	Observer operator=(const Observer &rhs) {
		m_obctrl->Release();
		m_obctrl = rhs.m_obctrl;
		m_obctrl->AddRef();
		return *this;
	}
	~Observer() {
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

class Object : public RTTI
{
public:
	RTTI_DECLARATIONS(Object, RTTI);

	Object();
	virtual ~Object();

	template<typename T>
	Observer<T> GetObserver() {
		assert(this->Is(T::TypeIdClass()));
		return Observer<T>(m_obctrl);
	}

	void SetInvalid();

private:
	ObserverCtrl *m_obctrl = nullptr;
};

} // namespace ltk

