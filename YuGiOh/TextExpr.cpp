#include "stdafx.h"
#include "TextExpr.h"
#include <LogLib/Log.h>
#include <ProcessLib/Memory/Memory.h>
#include "CardExtend.h"
#include "CardAction.h"
#include "GameEnviroment.h"
#include "Card.h"

#define _SELF L"TextExpr.cpp"
std::vector<libTools::ExpressionFunPtr>& CTextExpr::GetVec()
{
	static std::vector<libTools::ExpressionFunPtr> Vec =
	{
		{std::bind(&CTextExpr::Help, this, std::placeholders::_1), L"Help"},
		{ std::bind(&CTextExpr::FindCardByName, this, std::placeholders::_1), L"FindCardByName" },
		{ std::bind(&CTextExpr::FindCardById, this, std::placeholders::_1), L"FindCardById" },
		{ std::bind(&CTextExpr::PrintFullCard, this, std::placeholders::_1), L"PrintFullCard" },
		{ std::bind(&CTextExpr::SwapHandCard, this, std::placeholders::_1), L"SwapHandCard" },
		{ std::bind(&CTextExpr::SwapDeskCard, this, std::placeholders::_1), L"SwapDeskCard" },
		{ std::bind(&CTextExpr::InitializeHandCard, this, std::placeholders::_1), L"InitializeHandCard" },
		{ std::bind(&CTextExpr::PumpingCard, this, std::placeholders::_1), L"PumpingCard" },
		{ std::bind(&CTextExpr::ResetMonsterStar, this, std::placeholders::_1), L"ResetMonsterStar" },
	};

	return Vec;
}

VOID CTextExpr::Release()
{

}

VOID CTextExpr::Help(_In_ CONST std::vector<std::wstring>&)
{
	auto& Vec = GetVec();
	for (auto& itm : Vec)
		LOG_CF_D(L"MethodName[%s]", itm.wsFunName.c_str());
}

VOID CTextExpr::FindCardByName(_In_ CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() == 0)
	{
		LOG_CF_D(L"where's CardName?");
		return;
	}

	CONST std::wstring& wsCardName = VecParam.at(0);

	CCard Card(0);
	if (!CCardExtend().FindCardByName(wsCardName, &Card))
	{
		LOG_CF_D(L"UnExist Card[%s]", wsCardName.c_str());
		return;
	}

	Card.Print();
}

VOID CTextExpr::FindCardById(_In_ CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() == 0)
	{
		LOG_CF_D(L"where's CardId?");
		return;
	}

	DWORD dwCardId = _wtoi(VecParam.at(0).c_str());

	CCard Card(0);
	if (!CCardExtend().FindCardById(dwCardId, &Card))
	{
		LOG_CF_D(L"UnExist Card[%X]", dwCardId);
		return;
	}

	Card.Print();
}

VOID CTextExpr::PrintFullCard(_In_ CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() == 0)
	{
		LOG_CF_D(L"where's Type?");
		return;
	}


	CONST std::wstring& wsType = VecParam.at(0);
	if (wsType == L"ALL")
	{
		std::map<std::wstring, CCard> MapCard;
		CCardExtend().GetFullCard(MapCard);

		LOG_CF_D(L"Size=%d", MapCard.size());
		for (auto& itm : MapCard)
			itm.second.Print();

	}
	else if (wsType == L"Hand")
	{
		std::vector<CCard> VecHand;
		CCardExtend().GetHandCard(VecHand);

		for (auto& itm : VecHand)
			itm.Print();
	}
	else if (wsType == L"Group")
	{
		std::vector<CCard> MapCard;
		CCardExtend().GetGroupCard(MapCard);

		for (auto& itm : MapCard)
			itm.Print();

	}
	else
	{
		LOG_CF_D(L"Invalid Type[%s]", wsType.c_str());
	}
}

VOID CTextExpr::SwapHandCard(_In_ CONST std::vector<std::wstring>& VecParam)
{
	CCardAction Act;
	Act.SwapHandCard(0, L"克里底亚之牙");
	Act.SwapHandCard(1, L"神圣防护罩-反射镜力");
}

VOID CTextExpr::SwapDeskCard(_In_ CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() != 2)
	{
		LOG_CF_D(L"Parm1=Name[Desk] Param2=Name[Hand|Group]");
		return;
	}

	CCardAction Act;
	Act.SwapDeskCard(VecParam.at(0), VecParam.at(1));
}

VOID CTextExpr::InitializeHandCard(_In_ CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() == 0)
	{
		LOG_C_E(L"守护者卡组|???");
		return;
	}
	CCardAction Act;
	if (VecParam.at(0) == L"守护者卡组")
	{
		Act.SwapHandCard(0, L"守护者·戴思塞斯");
		Act.SwapHandCard(1, L"守护者·艾托斯");
		Act.SwapHandCard(2, L"女神的圣剑-鹰灵");
		Act.SwapHandCard(3, L"死神的大镰-断魂");
		//Act.SwapHandCard(4, L"赫谟之爪");
	}
}

DWORD WINAPI _HandCardCountChecker(LPVOID)
{
	for (;;)
	{
		WORD wdCount = libTools::CMemory::ReadWORD(0x1410d7fc4);
		if (wdCount != 1)
		{
			::Sleep(100);
			continue;
		}


		if ((libTools::CMemory::ReadWORD(PUMPINGCARD_BASE2) >> 0xF) != 0)
		{
			::Sleep(100);
			continue;
		}

		if (libTools::CMemory::ReadBYTE(PUMPINGCARD_BASE1) > 2)
		{
			::Sleep(100);
			continue;
		}


		LOG_C_D(L"Pulling Card");
		auto ulCardCountPtr = libTools::CMemory::ReadMemValue(0x1410d7fc4);

		// remove low Value
		ulCardCountPtr >>= 0x20;
		ulCardCountPtr <<= 0x20;
		ulCardCountPtr += 2;
		*reinterpret_cast<DWORD64*>(0x1410d7fc4) = ulCardCountPtr;
		::Sleep(2000);
	}
	return 0;
}

VOID CTextExpr::PumpingCard(_In_ CONST std::vector<std::wstring>& VecParam)
{
	//::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_HandCardCountChecker, NULL, NULL, NULL);
	/*
	x64dbg  script:
	mov rax,1410D87D0 // set 1
	mov [rax],1
	mov rax,1410D7FC0 // 0 = self 1 = enemy
	mov [rax],0
	mov r12, 14049BA30
	push r12
	ret
	ret
	*/
	/*if (VecParam.size() != 1)
	{
		LOG_CF_D(L"PumpingCard(CardName)");
		return;
	}


	std::vector<CCard> VecMapCard;
	CCardExtend().GetGroupCard(VecMapCard);
	auto itr = std::find_if(VecMapCard.begin(), VecMapCard.end(), [VecParam](CCard& itm) { return itm.GetCardName() == VecParam.at(0); });
	if (itr == VecMapCard.end())
	{
		LOG_MSG_CF(L"无效的卡:[%s]", VecParam.at(0).c_str());
		return;
	}


	CCard& Card = *itr;
	DWORD* pdwGroup = CCardExtend().GetGrouCardPointr();
	if (*pdwGroup != (DWORD)Card.GetFullCardId())
	{
		DWORD dwTempId = *pdwGroup;
		UINT_PTR dwTempBase = Card.GetBase();
		*pdwGroup = (DWORD)Card.GetFullCardId();
		*(DWORD*)dwTempBase = dwTempId;
	}
	
	
	*reinterpret_cast<UINT_PTR*>(PUMPINGCARD_BASE1) = 0x1;
	*reinterpret_cast<UINT_PTR*>(PUMPINGCARD_BASE2) = 0x0;

	using PumpingCardPtr = VOID(WINAPI*)(VOID);
	PumpingCardPtr Ptr = reinterpret_cast<PumpingCardPtr>(PUMPINGCARD_CALL);
	Ptr();*/

	//::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_HandCardCountChecker, NULL, NULL, NULL);
	std::vector<CCard> VecHand;
	CCardExtend().GetHandCard(VecHand);
	for (UINT i = 0; i < 6 - VecHand.size(); ++i)
	{
		*reinterpret_cast<UINT_PTR*>(PUMPINGCARD_BASE1) = 0x1;
		*reinterpret_cast<UINT_PTR*>(PUMPINGCARD_BASE2) = 0x0;

		using PumpingCardPtr = VOID(WINAPI*)(VOID);
		PumpingCardPtr Ptr = reinterpret_cast<PumpingCardPtr>(PUMPINGCARD_CALL);
		Ptr();
	}
	/*__try
	{
		
		
	}
	__except (1)
	{
		LOG_CF_E(L"PumpingCard Exception");
	}*/
}

VOID CTextExpr::ResetMonsterStar(_In_ CONST std::vector<std::wstring>&)
{
	CCardAction().ResetMonsterStar();
}
