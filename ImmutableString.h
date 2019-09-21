#pragma once


class ImmutableString
{
public:
	ImmutableString() {}
	ImmutableString(LPCSTR str)
	{
		m_ptr = _strdup(str);
	}
	~ImmutableString()
	{
		free((void *)m_ptr);
	}
	bool operator==(LPCSTR rhs)
	{
		if (m_ptr && rhs) {
			return strcmp(m_ptr, rhs) == 0;
		}
		else {
			return m_ptr == rhs;
		}
	}
	void operator=(LPCSTR str)
	{
		free((void *)m_ptr);
		if (str) {
			m_ptr = _strdup(str);
		}
		else {
			m_ptr = nullptr;
		}
	}
	operator LPCSTR()
	{
		return m_ptr;
	}

private:
	const char *m_ptr = nullptr;
};


