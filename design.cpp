int m_currentHandler = -1;
vector<CallbackInfo> m_handlers;


BOOL LtkCallNextEventHandler(LtkEvent *ev);

#defind LTK_HANDLE_CLICK(func) \
	case LTK_CLICK: \
		return self->func((LtkClickEvent*)ev);


// 放头文件里

#defind LTK_EVENT_HANDLER_BEGIN(klass, name) \
	static BOOL MyClass::name(void *userdata, LtkEvent *ev) \
		klass *self = (klass *)userdata; \
		switch(ev->id) { 
		
		
#defind LTK_EVENT_HANDLER_END() \
		} \
		return FALSE; \
	}
	
