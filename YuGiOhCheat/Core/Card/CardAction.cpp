#include <pch.h>
#include "CardAction.h"
#include <LogLib/Log.h>
#include <ProcessLib/Memory/Memory.h>
#include "CardExtend.h"
#include "Card.h"

#define _SELF L"CardAction.cpp"

VOID CCardAction::SwapHandCard(_In_ int Index, _In_ CONST std::wstring& wsCardName2) CONST
{
	CCardExtend Ext;
	// Find Card in Hand
	CCard HandCard(0, 0);
	if (!Ext.FindHandCardByIndex(Index, &HandCard))
	{
		LOG_CF_E(L"UnExist Card[%d] in Hand", Index);
		return;
	}

	CCard GroupCard(0, 0);
	// Find Card in Group
	if (!Ext.FindGroupCardByName(wsCardName2, &GroupCard))
	{
		LOG_CF_E(L"UnExist Card[%s] in Group", wsCardName2.c_str());
		return;
	}

	auto  WriteValuePtr = [](_In_ DWORD64 ulAddr, _In_ DWORD dwValue)
	{
		auto dwCardValue = libTools::CMemory::ReadMemValue(ulAddr);
		LOG_CF_D(L"ulAddr=%I64X, dwCardValue=%I64X, dwValue=%X", ulAddr, dwCardValue, dwValue);

		// remove low Value
		dwCardValue >>= 0x20;
		dwCardValue <<= 0x20;
		dwCardValue += dwValue;
		*reinterpret_cast<DWORD64*>(ulAddr) = dwCardValue;
	};

	// swap
	DWORD dwGroupCardId = static_cast<DWORD>(GroupCard.GetFullCardId());
	DWORD dwHandCardId = static_cast<DWORD>(HandCard.GetFullCardId());
	auto ulHandAddr = HandCard.GetBase();
	auto ulGroupAddr = GroupCard.GetBase();

	*reinterpret_cast<DWORD*>(ulHandAddr) = dwGroupCardId;
	*reinterpret_cast<DWORD*>(ulGroupAddr) = dwHandCardId;

	//WriteValuePtr(GroupCard.GetBase(), static_cast<DWORD>(HandCard.GetFullCardId()));
	//WriteValuePtr(ulHandAddr, dwGroupCardId);
}

VOID CCardAction::SwapDeskCard(_In_ CONST std::wstring& wsCardName1, _In_ CONST std::wstring& wsCardName2) CONST
{
	CCardExtend Ext;
	// Find Card in Desk
	CCard Card(0, 0);
	if (!Ext.FindCardByName(wsCardName1, &Card))
	{
		LOG_CF_E(L"UnExist Card[%s] in Card", wsCardName1.c_str());
		return;
	}

	auto ulAddr = Ext.FindBaseInDesk(static_cast<DWORD>(Card.GetCardId()));
	if (ulAddr == NULL)
	{
		LOG_CF_E(L"UnExist Card[%s:%X] in Desk", wsCardName1.c_str(), static_cast<DWORD>(Card.GetCardId()));
		return;
	}



	// Find Card in Hand
	CCard HandCard(0, 0);
	if (!Ext.FindHandCardByName(wsCardName2, &HandCard))
	{
		// Find Card in Group
		if (!Ext.FindGroupCardByName(wsCardName2, &HandCard))
		{
			LOG_CF_E(L"UnExist Card[%s] in Group or Hand", wsCardName2.c_str());
			return;
		}
	}


	auto  WriteValuePtr = [](_In_ DWORD64 ulAddr, _In_ DWORD dwValue)
	{
		auto dwCardValue = libTools::CMemory::ReadMemValue(ulAddr);
		LOG_CF_D(L"ulAddr=%I64X, dwCardValue=%I64X, dwValue=%X", ulAddr, dwCardValue, dwValue);

		// remove low Value
		dwCardValue >>= 0x20;
		dwCardValue <<= 0x20;
		dwCardValue += dwValue;
		*reinterpret_cast<DWORD64*>(ulAddr) = dwCardValue;
	};

	// swap
	DWORD dwDeskCardId = libTools::CMemory::ReadDWORD(ulAddr);

	auto ulHandAddr = HandCard.GetBase();
	//LOG_MSG_CF(L"DeskAddr=%I64X, DeskId=%X, HandCardBase=%I64X, HandCardId=%X", ulAddr, dwDeskCardId, ulHandAddr, static_cast<DWORD>(HandCard.GetFullCardId()));

	// write handcard to desk
	WriteValuePtr(ulAddr, static_cast<DWORD>(HandCard.GetFullCardId()));
	//LOG_MSG_CF(L"Ready Write two");
	// write desk card to hand
	WriteValuePtr(ulHandAddr, dwDeskCardId);
}

VOID CCardAction::InitializeHandCard(_In_ int) CONST
{

}

VOID CCardAction::ResetMonsterStar()
{
	std::vector<CCard> VecHand;
	CCardExtend().GetHandCard(VecHand);
	for (auto& itm : VecHand)
		itm.ResetStar(4);


	std::vector<CCard> VecMapCard;
	CCardExtend().GetGroupCard(VecMapCard);
	for (auto& itm : VecMapCard)
		itm.ResetStar(4);
}
