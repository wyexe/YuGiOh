#include "stdafx.h"
#include "TextExpr.h"
#include <MyTools/Log.h>
#include "CardExtend.h"
#include "CardAction.h"
#include "GameEnviroment.h"
#include "Card.h"

#define _SELF L"TextExpr.cpp"
std::vector<MyTools::ExpressionFunPtr>& CTextExpr::GetVec()
{
	static std::vector<MyTools::ExpressionFunPtr> Vec = 
	{
		{std::bind(&CTextExpr::Help, this, std::placeholders::_1), L"Help"},
		{ std::bind(&CTextExpr::FindCardByName, this, std::placeholders::_1), L"FindCardByName" },
		{ std::bind(&CTextExpr::FindCardById, this, std::placeholders::_1), L"FindCardById" },
		{ std::bind(&CTextExpr::PrintFullCard, this, std::placeholders::_1), L"PrintFullCard" },
		{ std::bind(&CTextExpr::SwapHandCard, this, std::placeholders::_1), L"SwapHandCard" },
		{ std::bind(&CTextExpr::SwapDeskCard, this, std::placeholders::_1), L"SwapDeskCard" },
		{ std::bind(&CTextExpr::InitializeHandCard, this, std::placeholders::_1), L"InitializeHandCard" },
		{ std::bind(&CTextExpr::PumpingCard, this, std::placeholders::_1), L"PumpingCard" },
		
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
		std::map<std::wstring, CCard> MapCard;
		CCardExtend().GetGroupCard(MapCard);

		for (auto& itm : MapCard)
			itm.second.Print();

	}
	else
	{
		LOG_CF_D(L"Invalid Type[%s]", wsType.c_str());
	}
}

VOID CTextExpr::SwapHandCard(_In_ CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() != 2)
	{
		LOG_CF_D(L"Parm1=Name[Hand] Param2=Name[Group]");
		return;
	}

	CCardAction Act;
	Act.SwapHandCard(_wtoi(VecParam.at(0).c_str()), VecParam.at(1));
}

VOID CTextExpr::SwapDeskCard(_In_ CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() != 2)
	{
		LOG_CF_D(L"Parm1=Name[Hand] Param2=Name[Group]");
		return;
	}

	CCardAction Act;
	Act.SwapDeskCard(VecParam.at(0), VecParam.at(1));
}

VOID CTextExpr::InitializeHandCard(_In_ CONST std::vector<std::wstring>& VecParam)
{

}

VOID CTextExpr::PumpingCard(_In_ CONST std::vector<std::wstring>&)
{
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

	__try
	{
		*reinterpret_cast<UINT_PTR*>(PUMPINGCARD_BASE1) = 0x1;
		*reinterpret_cast<UINT_PTR*>(PUMPINGCARD_BASE2) = 0x0;

		using PumpingCardPtr = VOID(WINAPI*)(VOID);
		PumpingCardPtr Ptr = reinterpret_cast<PumpingCardPtr>(PUMPINGCARD_CALL);
		Ptr();
	}
	__except (1)
	{
		LOG_CF_E(L"PumpingCard Exception");
	}
}
