
// team_Dlg.h: 헤더 파일
//

#pragma once
#include <set>

// CteamDlg 대화 상자
class CteamDlg : public CDialogEx
{
	// 생성입니다.
public:
	CteamDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	vtkSmartPointer<vtkRenderWindow> m_vtkWindow;
	void InitVtkWindow(void* hWnd);
	void ResizeVtkWindow();
	void callVtk(CString& str);

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEAM__DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDeltaposSpinSheet(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnStnClickedStatic1();
	afx_msg void OnNMClickListChars(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit_bookname;
	CEdit m_edit_sheet;
	CEdit m_edit_type;
	CStatic m_static_sheets;

	CSpinButtonCtrl m_spin_sheet;
	CSpinButtonCtrl m_spin_type;
	CComboBox m_new_bookname;

	// 사용자 지정 변수들
public:
	CString base_address;

	CStatic m_static_sheet_bottom;
	CStatic m_static_num_1;
	CStatic m_static_num_2;
	CStatic m_static_num_3;
	CStatic m_static_num_4;
	CStatic m_static_num_5;
	CStatic m_static_num_6;
	CStatic m_static_char;

	CStatic m_static_type1;

	CString sheet_text;
	CString bookname_text;
	CString str_name;
	CString strstr_; //리스트박스에서 선택한 값을 가져올 변수

	CListCtrl m_ListCtrl;
	CString list_fileName[30];

	int sheet_num_array[3] = { 0, 0, 0 };	// 페이지별 글자 수

	int sheet_Ttext;						// 
	int sheet_text_array[3] = { 0, 0, 0 };	// 

	int sheet_Ttype;						// 
	int sheet_type_array[3] = { 0, 0, 0 };	// 

	int sheet_num;	// 현재 장 번호
	int csv_now;	// 0~351
	int pre_csv_now;
	bool start_;	// 버튼이 눌렸는지 확인하는 변수

	int folder_num;

	int list_selected;

	int list_num;

	std::set<CString> kind;				// 전체 글자 종류 정렬(중복 제거)
	std::set<CString> kind_array[3];	// 페이지별 글자 종류 정렬 (중복 제거)
	std::set<CString> tkind_array[3];	// 
	std::set<CString> tkind3;			// 이거 왜 못없애는지 모르겠음
	std::set<CString>::iterator iter;	// kind의 값에 접근하기 위한 이터레이터

	CStatic m_static1;
	CStatic m_static2;
	CStatic m_static3;

	CImage m_bitmap;

	CImage m_pagebitmap[3];

public:
	void PrintList();
	void PrintData();
	void PrintBitmap(CString address, int nID);
	void printRgn(void);

	CStatic m_static_types;
	afx_msg void OnDeltaposSpinType(NMHDR* pNMHDR, LRESULT* pResult);
	CString typeNum;
};

struct st_for_list
{
	int l_order;
	int l_line;
	int l_sheet;
};

