#ifndef __YUGIOH_CORE_CARD_CARD_H__
#define __YUGIOH_CORE_CARD_CARD_H__

#include <MyTools/ToolsPublic.h>

class CCard
{
public:
	CCard();
	~CCard() = default;
	CCard(_In_ UINT_PTR ulCardId);
	CCard(_In_ UINT_PTR ulCardId, _In_ UINT_PTR dwBase);

	//
	DWORD GetPower() CONST _NOEXCEPT;

	// 
	DWORD GetDefence() CONST _NOEXCEPT;

	//
	DWORD GetStarCount() CONST _NOEXCEPT;

	//
	CONST std::wstring& GetCardName();

	//
	CONST std::wstring& GetCardContent();

	//
	UINT_PTR GetCardId() CONST _NOEXCEPT;

	//
	UINT_PTR GetFullCardId() CONST _NOEXCEPT;

	//
	bool IsCard() CONST _NOEXCEPT;

	//
	VOID Print();

	//
	UINT_PTR GetBase() CONST _NOEXCEPT;
private:
	UINT_PTR     _dwCardId;
	std::wstring _wsCardName;
	std::wstring _wsCardContent;
	UINT_PTR      _dwBase;
};



#endif // !__YUGIOH_CORE_CARD_CARD_H__
