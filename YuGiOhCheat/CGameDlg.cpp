// CGameDlg.cpp: 实现文件
//

#include "pch.h"
#include "YuGiOhCheat.h"
#include "CGameDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <Core/Card/CardExtend.h>
#include <Core/Card/Card.h>
#include <Core/Card/CardAction.h>
#include <CharacterLib/Character.h>
#include <Common/GameEnviroment.h>

// CGameDlg 对话框

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CGameDlg::OnInitDialog()
{
	CListCtrl* pList = reinterpret_cast<CListCtrl*>(this->GetDlgItem(IDC_LIST1));
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	pList->InsertColumn(0, L"Name", LVCFMT_LEFT, 100);
	pList->InsertColumn(1, L"ID", LVCFMT_LEFT, 60);
	pList->InsertColumn(2, L"Base", LVCFMT_LEFT, 80);
	pList->InsertColumn(3, L"Text", LVCFMT_LEFT, 400);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CGameDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CGameDlg::OnNMClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CGameDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGameDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CGameDlg 消息处理程序


void CGameDlg::OnBnClickedButton1()
{
	std::vector<CCard> VecGroupCard;
	CCardExtend().GetGroupCard(VecGroupCard);

	CListCtrl* pList = reinterpret_cast<CListCtrl*>(this->GetDlgItem(IDC_LIST1));
	pList->DeleteAllItems();

	for (auto& itm : VecGroupCard)
	{
		auto nRow = pList->InsertItem(pList->GetItemCount(), itm.GetCardName().c_str());
		pList->SetItemText(nRow, 1, libTools::CCharacter::MakeFormatText(L"%X", itm.GetFullCardId()).c_str());
		pList->SetItemText(nRow, 2, libTools::CCharacter::MakeFormatText(L"%llX", itm.GetBase()).c_str());
		pList->SetItemText(nRow, 3, itm.GetCardContent().c_str());
	}
}


void CGameDlg::OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem != -1)
	{
		//CListCtrl* pList = reinterpret_cast<CListCtrl*>(this->GetDlgItem(IDC_LIST1));
		//(reinterpret_cast<CEdit*>(this->GetDlgItem(IDC_EDIT1)))->SetWindowTextW(pList->GetItemText(pNMItemActivate->iItem, 3));
	}
	*pResult = 0;
}


void CGameDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	if (pNMItemActivate->iItem != -1)
	{
		CListCtrl* pList = reinterpret_cast<CListCtrl*>(this->GetDlgItem(IDC_LIST1));
		DWORD64 CardId = std::stoull(std::wstring(pList->GetItemText(pNMItemActivate->iItem, 1).GetBuffer()), nullptr, 16);

		std::vector<CCard> VecGroupCard;
		CCardExtend().GetGroupCard(VecGroupCard);
		auto itr = std::find_if(VecGroupCard.begin(), VecGroupCard.end(), [CardId](CONST CCard& Card) { return Card.GetFullCardId() == CardId; });
		if (itr == VecGroupCard.end())
		{
			AfxMessageBox(L"Card UnExist!");
			return;
		}


		DWORD64 dwSwapCardID = VecGroupCard.begin()->GetFullCardId();


		DWORD64 dwFirstCardAddr = VecGroupCard.begin()->GetBase();
		DWORD64 dwSwapCardAddr = itr->GetBase();
		*reinterpret_cast<DWORD*>(dwSwapCardAddr) = static_cast<DWORD>(dwSwapCardID);
		*reinterpret_cast<DWORD*>(dwFirstCardAddr) = static_cast<DWORD>(CardId);

		*reinterpret_cast<UINT_PTR*>(PUMPINGCARD_BASE1) = 0x1;
		*reinterpret_cast<UINT_PTR*>(PUMPINGCARD_BASE2) = 0x0;

		using PumpingCardPtr = VOID(WINAPI*)(VOID);
		PumpingCardPtr Ptr = reinterpret_cast<PumpingCardPtr>(PUMPINGCARD_CALL);
		Ptr();

		//AfxMessageBox(libTools::CCharacter::MakeFormatText(L"dwFirstCardAddr[%llX]=>[%X], dwSwapCardAddr[%llX]=>[%X]", dwFirstCardAddr, CardId, dwSwapCardAddr, dwSwapCardID).c_str());
		OnBnClickedButton1();
	}

	*pResult = 0;
}


void CGameDlg::OnBnClickedButton2()
{
	CONST static std::vector<std::wstring> Vec = 
	{
		L"被封印者的左腕",L"被封印者的右腕",L"被封印者的左足",L"被封印者的右足",L"被封印的艾克佐迪亚"
	};


	
	for (auto& itm : Vec)
	{
		std::vector<CCard> VecGroupCard;
		CCardExtend().GetGroupCard(VecGroupCard);
		auto itr = std::find_if(VecGroupCard.begin(), VecGroupCard.end(), [itm](CCard& Card) { return Card.GetCardName() == itm; });
		if (itr == VecGroupCard.end())
			continue;


		DWORD64 CardId = itr->GetFullCardId();
		DWORD64 dwSwapCardID = VecGroupCard.begin()->GetFullCardId();


		DWORD64 dwFirstCardAddr = VecGroupCard.begin()->GetBase();
		DWORD64 dwSwapCardAddr = itr->GetBase();
		*reinterpret_cast<DWORD*>(dwSwapCardAddr) = static_cast<DWORD>(dwSwapCardID);
		*reinterpret_cast<DWORD*>(dwFirstCardAddr) = static_cast<DWORD>(CardId);

		*reinterpret_cast<UINT_PTR*>(PUMPINGCARD_BASE1) = 0x1;
		*reinterpret_cast<UINT_PTR*>(PUMPINGCARD_BASE2) = 0x0;

		using PumpingCardPtr = VOID(WINAPI*)(VOID);
		PumpingCardPtr Ptr = reinterpret_cast<PumpingCardPtr>(PUMPINGCARD_CALL);
		Ptr();
	}
}
