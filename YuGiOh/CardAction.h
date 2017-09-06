#ifndef __YUGIOH_CORE_CARD_CARDACTION_H__
#define __YUGIOH_CORE_CARD_CARDACTION_H__

#include <MyTools/ToolsPublic.h>

class CCardAction
{
public:
	CCardAction() = default;
	~CCardAction() = default;

	//
	VOID SwapHandCard(_In_ int Index, _In_ CONST std::wstring& wsCardName2) CONST;

	//
	VOID SwapDeskCard(_In_ CONST std::wstring& wsCardName1, _In_ CONST std::wstring& wsCardName2) CONST;

	//
	VOID InitializeHandCard(_In_ int nIndex) CONST;
private:

};


#endif // !__YUGIOH_CORE_CARD_CARDACTION_H__
