#ifndef __YUGIOH_CORE_OUTPUT_TEXTEXPR_H__
#define __YUGIOH_CORE_OUTPUT_TEXTEXPR_H__

#include <MyTools/ClassInstance.h>
#include <MyTools/CLExpression.h>

class CTextExpr : public MyTools::CExprFunBase, public MyTools::CClassInstance<CTextExpr>
{
public:
	CTextExpr() = default;
	virtual ~CTextExpr() = default;

	virtual std::vector<MyTools::ExpressionFunPtr>& GetVec();
private:
	virtual VOID Release();

	virtual VOID Help(_In_ CONST std::vector<std::wstring>&);

	// FindCardByName(XXXX)
	VOID FindCardByName(_In_ CONST std::vector<std::wstring>& VecParam);

	//
	VOID FindCardById(_In_ CONST std::vector<std::wstring>& VecParam);

	// PrintFullCard(ALL,Hand,Group)
	VOID PrintFullCard(_In_ CONST std::vector<std::wstring>& VecParam);

	// SwapHandCard(Name[in CardGroup],Name[in HandCard])
	VOID SwapHandCard(_In_ CONST std::vector<std::wstring>& VecParam);

	// SwapDeskCard(Name[in CardGroup|HandCard],Name[in DeskCard])
	VOID SwapDeskCard(_In_ CONST std::vector<std::wstring>& VecParam);

	// The Default Scheme, InitializeHandCard(Index)
	VOID InitializeHandCard(_In_ CONST std::vector<std::wstring>& VecParam);

	//
	VOID PumpingCard(_In_ CONST std::vector<std::wstring>&);
private:

};



#endif // !__YUGIOH_CORE_OUTPUT_TEXTEXPR_H__
