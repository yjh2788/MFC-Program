#pragma once


// NewDialog 대화 상자

class NewDialog : public CDialogEx
{
	DECLARE_DYNAMIC(NewDialog)

public:
	NewDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~NewDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_new_bookname;
	afx_msg void OnBnClickedOk();
	CString temp_string;
};
