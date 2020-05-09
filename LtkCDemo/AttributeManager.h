#pragma once

struct AttributeInfo
{
	LPCSTR name = nullptr;
	LPCSTR type = nullptr;
	std::string default_;
	std::string translate;
};

struct WidgetAttribute
{
	LPCSTR base = nullptr;
	std::vector<AttributeInfo> attr_list;
};

class AttributeManager
{
public:
	static AttributeManager* Instance();
	static void Free();

	void LoadXml(LPCWSTR path);

private:
	AttributeManager();
	~AttributeManager() {}

	std::unordered_map<LPCSTR, WidgetAttribute> mapWidgetAttr;
};

