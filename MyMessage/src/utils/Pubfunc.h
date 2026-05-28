#ifndef COMMON_H
#define COMMON_H

namespace MPubfunc
{
	Gdiplus::Color GetBkColor(UINT32 uPortNum);
	Gdiplus::Color GetBorderColor(UINT32 uPortNum);

	// Ê±¼ä
	CDuiString GetCurrentSystemTime();

	// ×Ö·û´®
	std::wstring String2WString(std::string strvalue);
	std::string WString2String(std::wstring wstrvalue);

	bool Char2Data(void *pdata, size_t len, char *pOutputData);
}


#endif // !COMMON_H