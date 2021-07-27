#include "stdafx.h"
#include "Builder.h"
#include "ltk.h"
#include "Sprite.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

namespace ltk {

Builder* Builder::m_sInst = nullptr;

Builder* Builder::Instance()
{
	if (!m_sInst) {
		m_sInst = new Builder;
	}
	return m_sInst;
}

void Builder::Free()
{
	delete m_sInst;
}

void Builder::RegisterType(LPCSTR xml_tag, FactoryMethod func)
{
	//auto tag_intern = ltk::InternString(xml_tag);
	// https://stackoverflow.com/questions/50608392/using-const-char-as-key-for-map-unordered-map
	m_mapFactory[xml_tag] = func;
}

Ptr<Widget> Builder::WidgetFromXml(LPCSTR path)
{
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(path) != XML_SUCCESS) return Ptr<Widget>();
	m_buildingPath.clear();
	auto elm = doc.FirstChildElement();
	return WidgetFromXmlRec(elm, nullptr);
}

Ptr<Widget> Builder::WidgetFromXmlRec(tinyxml2::XMLElement* elm, Widget* parent)
{
	Ptr<Widget> obj;
	auto name = elm->Name();
	auto iter = m_mapFactory.find(name);
	if (iter != m_mapFactory.end()) {
		m_buildingPath.push_back(name);
		obj = iter->second();
		//LTK_ASSERT(Object::CheckValid((Object*)hltk));
		//obj = ((Object*)hltk)->As<Widget>();
		if (parent) {
			parent->AddChild(obj); // 要先有父级才能设置属性 因为有的属性要传递给父级
		}
		auto attr = elm->FirstAttribute();
		while (attr) {
			obj->SetAttribute(ltk::InternString(attr->Name()), attr->Value());
			attr = attr->Next();
		}
		auto child_elm = elm->FirstChildElement();
		while (child_elm) {
			WidgetFromXmlRec(child_elm, obj.Get());
			child_elm = child_elm->NextSiblingElement();
		}
		m_buildingPath.pop_back();
	}
	return obj;
}

} // namespace ltk
