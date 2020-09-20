#pragma once

namespace ltk {

class Object;
class Widget;

typedef HRESULT (CALLBACK *FactoryMethod)(ILtkWidget **ppOut);

class Builder
{
protected:
	Builder() {}
	~Builder() {}

public:
	static Builder* Instance();
	static void Free();

	void RegisterType(LPCSTR xml_tag, FactoryMethod func);
	Object* WidgetFromXml(LPCSTR path);

private:
	ILtkWidget* WidgetFromXmlRec(tinyxml2::XMLElement* elm, Object* parent);

	static Builder* m_sInst;

	std::unordered_map<std::string, FactoryMethod> m_mapFactory;
	std::vector<std::string> m_buildingPath;
};

} // namespace ltk
