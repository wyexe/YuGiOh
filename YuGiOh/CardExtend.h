#ifndef __YUGIOH_CORE_CARD_CARDEXTEND_H__
#define __YUGIOH_CORE_CARD_CARDEXTEND_H__

#include <map>
#include <vector>

class CCard;
class CCardExtend
{
public:
	CCardExtend() = default;
	~CCardExtend() = default;

	//
	UINT_PTR GetFullCard(_Out_ std::map<std::wstring,CCard>& MapCard) CONST;

	//
	UINT_PTR GetGroupCard(_Out_ std::map<std::wstring, CCard>& MapCard) CONST;

	//
	UINT_PTR GetHandCard(_Out_ std::vector<CCard>& VecCard) CONST;

	//
	BOOL	 FindGroupCardByName(_In_ CONST std::wstring& wsCardName, _Out_ CCard* pCard) CONST;

	// 
	BOOL	 FindHandCardByName(_In_ CONST std::wstring& wsCardName, _Out_ CCard* pCard) CONST;

	//
	BOOL	 FindHandCardByIndex(_In_ int Index, _Out_ CCard* pCard) CONST;

	//
	BOOL	 FindCardByName(_In_ CONST std::wstring& wsCardName, _Out_ CCard* pCard) CONST;

	//
	BOOL	 FindCardById(_In_ DWORD dwCardId, _Out_ CCard* pCard) CONST;

	//
	DWORD64  FindBaseInDesk(_In_ DWORD dwCardId) CONST;
private:
	DWORD* GetGrouCardPointr() CONST _NOEXCEPT;
private:

};



#endif // !__YUGIOH_CORE_CARD_CARDEXTEND_H__
