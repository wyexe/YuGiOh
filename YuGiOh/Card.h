#ifndef __YUGIOH_CORE_CARD_CARD_H__
#define __YUGIOH_CORE_CARD_CARD_H__

#include <Windows.h>
#include <string>

class CCard
{
public:
	CCard();
	~CCard() = default;
	CCard(_In_ UINT_PTR ulCardId);
	CCard(_In_ UINT_PTR ulCardId, _In_ UINT_PTR dwBase);

	//
	DWORD GetPower() CONST ;

	// 
	DWORD GetDefence() CONST ;

	//
	DWORD GetStarCount() CONST ;

	//
	CONST std::wstring& GetCardName();

	//
	CONST std::wstring& GetCardContent();

	//
	UINT_PTR GetCardId() CONST ;

	//
	UINT_PTR GetFullCardId() CONST ;

	//
	bool IsCard() CONST ;

	//
	VOID Print();

	//
	UINT_PTR GetBase() CONST ;

	//
	VOID ResetStar(_In_ DWORD dwStar);
private:
	UINT_PTR     _dwCardId;
	std::wstring _wsCardName;
	std::wstring _wsCardContent;
	UINT_PTR      _dwBase;
};



#endif // !__YUGIOH_CORE_CARD_CARD_H__
