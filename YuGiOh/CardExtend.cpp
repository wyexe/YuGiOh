#include "stdafx.h"
#include "CardExtend.h"
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Log.h>
#include "GameEnviroment.h"
#include "Card.h"

#define _SELF L"CardExtend.cpp"
UINT_PTR CCardExtend::GetFullCard(_Out_ std::map<std::wstring, CCard>& MapCard) CONST
{
	for (DWORD64 ulCardId = MIN_CARDID; ulCardId <= MAX_CARDID; ++ulCardId)
	{
		CCard Card(ulCardId);
		if (!Card.GetCardName().empty())
		{
			MapCard.emplace(std::make_pair(Card.GetCardName(), Card));
		}
	}

	return MapCard.size();
}

UINT_PTR CCardExtend::GetGroupCard(_Out_ std::map<std::wstring, CCard>& MapCard) CONST
{
	auto pGroupCard = GetGrouCardPointr();
	for (int i = 0; i < 60; ++i, pGroupCard)
	{
		if (pGroupCard[i] == NULL)
			continue;

		CCard Card(static_cast<UINT_PTR>(pGroupCard[i]), reinterpret_cast<UINT_PTR>(pGroupCard + i * 4));
		if (!Card.GetCardName().empty())
		{
			MapCard.emplace(std::make_pair(Card.GetCardName(), Card));
		}
	}
	return MapCard.size();
}

UINT_PTR CCardExtend::GetHandCard(_Out_ std::vector<CCard>& VecCard) CONST
{
	for (UINT i = 0;i < 60; ++i)
	{
		UINT_PTR ulAddr = CURRENT_HAND_CARD_BASE + i * 4;
		UINT_PTR ulCardId = MyTools::CCharacter::ReadMemValue(ulAddr) & 0xFFFFFFFF;
		if (ulCardId == 0)
			break;

		//LOG_CF_D(L"CardID=%X", static_cast<DWORD>(ulCardId));
		CCard Card(MyTools::CCharacter::ReadMemValue(ulAddr) & 0xFFFFFFFF, ulAddr);
		VecCard.push_back(std::move(Card));
	}

	return VecCard.size();
}

BOOL CCardExtend::FindGroupCardByName(_In_ CONST std::wstring& wsCardName, _Out_ CCard* pCard) CONST
{
	std::map<std::wstring, CCard> VecCard;
	GetGroupCard(VecCard);

	for (auto& Card : VecCard)
	{
		if (Card.second.GetCardName() == wsCardName)
		{
			if (pCard != nullptr)
				*pCard = Card.second;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCardExtend::FindHandCardByName(_In_ CONST std::wstring& wsCardName, _Out_ CCard* pCard) CONST
{
	std::vector<CCard> Vec;
	GetHandCard(Vec);

	for (auto& Card : Vec)
	{
		if (Card.GetCardName() == wsCardName)
		{
			if (pCard != nullptr)
				*pCard = Card;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCardExtend::FindHandCardByIndex(_In_ int Index, _Out_ CCard* pCard) CONST
{
	std::vector<CCard> Vec;
	GetHandCard(Vec);
	*pCard = Vec.at(Index);
	return TRUE;
}

BOOL CCardExtend::FindCardByName(_In_ CONST std::wstring& wsCardName, _Out_ CCard* pCard) CONST
{
	std::map<std::wstring, CCard> MapCard;
	GetFullCard(MapCard);

	auto itr = MapCard.find(wsCardName);
	if (itr == MapCard.end())
		return FALSE;

	// Copy
	if (pCard != nullptr)
		*pCard = itr->second;

	return TRUE;
}

BOOL CCardExtend::FindCardById(_In_ DWORD dwCardId, _Out_ CCard* pCard) CONST
{
	std::map<std::wstring, CCard> MapCard;
	GetFullCard(MapCard);

	for (auto& itm : MapCard)
	{
		if (static_cast<decltype(dwCardId)>(itm.second.GetCardId()) == dwCardId)
		{
			// Copy
			if (pCard != nullptr)
				*pCard = itm.second;

			return TRUE;
		}
	}

	return FALSE;
}

DWORD64 CCardExtend::FindBaseInDesk(_In_ DWORD dwCardId) CONST
{
	auto ulAddr = CURRENT_CARD_DESK_BASE + 0x30 + 0x18;
	for (DWORD64 i = 0; i <= 20; ++i)
	{
		DWORD64 ulCardAddr = ulAddr + i * 4 * 6;
		if ((MyTools::CCharacter::ReadDWORD(ulCardAddr) & 0xFFFF) == dwCardId)
		{
			return ulCardAddr;
		}
	}

	return NULL;
}

DWORD* CCardExtend::GetGrouCardPointr() CONST _NOEXCEPT
{
	return reinterpret_cast<DWORD*>(CURRENT_CARD_GROUP_BASE - CURRENT_CARD_GROUP_OFFSET);
}
