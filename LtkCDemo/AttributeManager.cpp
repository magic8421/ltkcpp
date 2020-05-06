#include "pch.h"
#include "AttributeManager.h"
#include "3rd/tinyxml2.h"
#include "../ScopeGuard.h"
#include "LtkInterface.h"

static AttributeManager* sInst = nullptr;

AttributeManager* AttributeManager::Instance()
{
	if (!sInst) {
		sInst = new AttributeManager;
	}
	return sInst;
}

void AttributeManager::Free()
{
	delete sInst;
}

AttributeManager::AttributeManager() 
{
	this->LoadXml(L"res\\attributes.xml");
}

void AttributeManager::LoadXml(LPCWSTR path)
{
	this->mapWidgetAttr.clear();

	using namespace tinyxml2;
	XMLDocument doc;
	XMLError err = XMLError::XML_SUCCESS;
	XMLElement* elm_widgets = nullptr;
	XMLElement* elm_widget = nullptr;

	FILE* fp = nullptr;
	_wfopen_s(&fp, path, L"rb");
	auto deffer1 = ltk::LtkScopeGuard([&](){
		fclose(fp);
	});
	if (!fp) return;

	err = doc.LoadFile(fp);
	if (err != XMLError::XML_SUCCESS) return;
	elm_widgets = doc.FirstChildElement("Widgets");
	if (!elm_widgets) return;

	WidgetAttribute widget_attr;

	elm_widget = elm_widgets->FirstChildElement();
	while (elm_widget) {
		LPCSTR widget_type = ::LtkInternString(elm_widget->Name());
		LPCSTR base = elm_widget->Attribute("base");
		widget_attr.base = ::LtkInternString(base);
		
		AttributeInfo attr_info;
		XMLElement* elm_attr = elm_widget->FirstChildElement("Attribute");
		while (elm_attr) {
			attr_info.name = ::LtkInternString(elm_attr->Attribute("name"));
			attr_info.type = ::LtkInternString(elm_attr->Attribute("type"));
			attr_info.default_ = elm_attr->Attribute("default");
			attr_info.translate = elm_attr->Attribute("translate");
			widget_attr.attr_list.push_back(std::move(attr_info));

			elm_attr->NextSiblingElement("Attribute");
		}
		this->mapWidgetAttr[widget_type] = std::move(widget_attr);

		elm_widget->NextSiblingElement();
	}
}

