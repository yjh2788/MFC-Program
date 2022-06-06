// NewDialog.cpp: 구현 파일
//

#include "pch.h"
#include "team_.h"
#include "NewDialog.h"
#include "afxdialogex.h"
#include "Universial_Parameter.h"


// NewDialog 대화 상자

IMPLEMENT_DYNAMIC(NewDialog, CDialogEx)

NewDialog::NewDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

NewDialog::~NewDialog()
{
}

void NewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_BOOKNAME, m_new_bookname);
}


BEGIN_MESSAGE_MAP(NewDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &NewDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// NewDialog 메시지 처리기


void NewDialog::OnBnClickedOk()
{
	CComboBox* p_list = (CComboBox*)GetDlgItem(IDC_NEW_BOOKNAME);

	int index = p_list->GetCurSel();
	if (index != LB_ERR)
		p_list->GetLBText(index, temp_string);	// str에 원하는 문자열이 들어있습니다...

	CDialogEx::OnOK();
}
