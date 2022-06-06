
// team_Dlg.cpp: 구현 파일

#include "pch.h"
#include "framework.h"
#include "team_.h"
#include "team_Dlg.h"
#include "afxdialogex.h"
#include "csvdata_structure.h"
#include <clocale>
#include <vector>
#include <Windows.h>
#include <vtkLight.h>
#include <vtkSTLReader.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

int GetDirFilesNum(CString dirName);

// CteamDlg 대화 상자
CteamDlg::CteamDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_TEAM__DIALOG, pParent)
    , typeNum(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    sheet_num = 1;
    csv_now = 0; // 0~351
    start_ = false;
    pre_csv_now = 0;

}

void CteamDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_BOOKNAME, m_edit_bookname);
    DDX_Control(pDX, IDC_EDIT_SHEET, m_edit_sheet);
    DDX_Control(pDX, IDC_EDIT_TYPE, m_edit_type);
    DDX_Control(pDX, IDC_STATIC_SHEETS, m_static_sheets);
    DDX_Control(pDX, IDC_SPIN_SHEET, m_spin_sheet);
    DDX_Control(pDX, IDC_STATIC_SHEET_BOTTOM, m_static_sheet_bottom);
    DDX_Control(pDX, IDC_STATIC_NUM_1, m_static_num_1);
    DDX_Control(pDX, IDC_STATIC_NUM_2, m_static_num_2);
    DDX_Control(pDX, IDC_STATIC_NUM_3, m_static_num_3);
    DDX_Control(pDX, IDC_STATIC_NUM_4, m_static_num_4);
    DDX_Control(pDX, IDC_STATIC_NUM_5, m_static_num_5);
    DDX_Control(pDX, IDC_STATIC_NUM_6, m_static_num_6);
    DDX_Control(pDX, IDC_STATIC_CHAR, m_static_char);
    DDX_Control(pDX, IDC_STATIC_TYPE_1, m_static_type1);
    DDX_Control(pDX, IDC_LIST_CHARS, m_ListCtrl);

    DDX_Control(pDX, IDC_STATIC1, m_static1);
    DDX_Control(pDX, IDC_STATIC2, m_static2);
    DDX_Control(pDX, IDC_STATIC3, m_static3);
    DDX_Control(pDX, IDC_STATIC_TYPES, m_static_types);
}

BEGIN_MESSAGE_MAP(CteamDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SHEET, &CteamDlg::OnDeltaposSpinSheet)
    ON_BN_CLICKED(IDC_BUTTON_OPEN, &CteamDlg::OnBnClickedButtonOpen)
    ON_STN_CLICKED(IDC_STATIC1, &CteamDlg::OnStnClickedStatic1)
    ON_NOTIFY(NM_CLICK, IDC_LIST_CHARS, &CteamDlg::OnNMClickListChars)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TYPE, &CteamDlg::OnDeltaposSpinType)
END_MESSAGE_MAP()


//-----------------------전역변수-----------------------------------------------------------------------------------------------

// 비트맵 출력을 위해 벡터 생성
vector<CRgn*> rgn[100];
vector<SCharInfo> sheet123[100];
CDC* cdc;
CWnd* pWnd;
ULONG_PTR m_gdiplusToken;
//-------------------------------------------------------------------------------------------------------------------------------


// CteamDlg 메시지 처리기

// CteamDlg 메시지 처리기

BOOL CteamDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
    // 프레임워크가 이 작업을 자동으로 수행합니다.
    SetIcon(m_hIcon, TRUE);         // 큰 아이콘을 설정합니다.
    SetIcon(m_hIcon, FALSE);      // 작은 아이콘을 설정합니다.

    // 리스트 초기화

    m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_ListCtrl.InsertColumn(0, TEXT("장"), LVCFMT_CENTER, 30);
    m_ListCtrl.InsertColumn(1, TEXT("sx"), LVCFMT_CENTER, 45);
    m_ListCtrl.InsertColumn(2, TEXT("sy"), LVCFMT_CENTER, 45);

    // static 초기 출력
    m_static_sheets.SetWindowTextW(_T("/0장"));
    m_edit_sheet.SetWindowTextW(_T("0"));
    m_edit_type.SetWindowTextW(_T("0"));

    // 스핀 컨트롤 설정
    m_spin_sheet.SetRange(1, 3);
    m_spin_sheet.SetPos(1);

    // 데이터 저장 변수 초기화
    sheet_Ttext = 0;
    sheet_Ttype = 0;

    if (this->GetDlgItem(IDC_STATIC_FRAME))
    {
        this->InitVtkWindow(this->GetDlgItem(IDC_STATIC_FRAME)->GetSafeHwnd());
        this->ResizeVtkWindow();
    }

    GdiplusStartupInput  gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);


    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CteamDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 아이콘을 그립니다.
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CteamDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CteamDlg::OnDeltaposSpinSheet(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if (!start_) return;

    //--------페이지 번호 바꾸기----------------------------------

    int iVal = (pNMUpDown->iPos) + (pNMUpDown->iDelta); // 값 = 현재 값 + 증감   

    if (iVal <= 0) iVal = 3;
    else if (iVal >= 4) iVal = 1;

    CString str;

    sheet_num = iVal;
    str.Format(_T("%d"), sheet_num);
    m_edit_sheet.SetWindowTextW(str);

    sheet_text.Format(_T("%d장"), sheet_num);
    m_static_sheet_bottom.SetWindowTextW(sheet_text);
    //--------------csv_now바꾸기------------------------------------------------------------
    //if (sheet_num == 1) csv_now = 0;
    for (int i = 0; i < db.m_Chars.size(); i++)
    {
        if ((db.m_Chars[i].m_sheet == sheet_num) && (db.m_Chars[i].m_line == 1) && (db.m_Chars[i].m_order == 1)) csv_now = i;
    }

    //--------비트맵 출력----------------------------------------

    pre_csv_now = 0;

    printRgn();

    //--------활자 정보 출력-------------------------------------

    // 현재 출력하고 있는 활자의 char 출력
    m_static_char.SetWindowTextW(db.m_Chars[csv_now].m_char);

    // 현재 출력하고 있는 활자의 위치 출력
    str.Format(_T("%d장 %d행 %d열"), db.m_Chars[csv_now].m_sheet, db.m_Chars[csv_now].m_line, db.m_Chars[csv_now].m_order);
    m_static_type1.SetWindowTextW(str);

    ///----------------------------------------------------------------------------------------------------------- //////////////////

    CString temp, tmp1;
    temp.Format(_T("\\%d"), db.m_Chars[csv_now].m_type);
    CString address = base_address + "03_type\\" + db.m_Chars[csv_now].m_char + temp + "\\*.*";

    //검색 클래스
    CFileFind finder;
    BOOL bWorking = finder.FindFile(address);
    for (int j = 0; j < 30; j++) list_fileName[j] = _T("");
    int k = 0;

    while (bWorking) // 파일 읽어오기
    {
        bWorking = finder.FindNextFile();   //다음 파일이 존재하면다면 TRUE 반환
        if (finder.IsArchived())
            list_fileName[k] = finder.GetFileName();
        k++;
    }

    CString img;
    CString add;
    tmp1 = _T("_");
    for (int i = 2; i < k; i++)
    {
        if (list_fileName[i].GetAt(1) == tmp1)
            img = list_fileName[i].Left(1);
        else
            img = list_fileName[i].Left(2);
        CString tmp2;
        tmp2.Format(_T("%d"), db.m_Chars[csv_now].m_sheet);
        if (img == tmp2)
        {
            add = base_address + "03_type\\" + db.m_Chars[csv_now].m_char + temp + "\\" + list_fileName[i];
            break;
        }

    }

    // 여기서 address_의 사진을 출력
    PrintBitmap(add, IDC_STATIC3);
    PrintBitmap(add, IDC_STATIC2);

    PrintData();
    PrintList();
    *pResult = 0;
}

void CteamDlg::OnBnClickedButtonOpen()
{
    //--------초기 동작----------------------------------------
    start_ = true;

    CFolderPickerDialog dlg(_T("C:\\"), OFN_FILEMUSTEXIST, NULL, 0);

    if (dlg.DoModal() == IDOK) {
        base_address = dlg.GetPathName();
        base_address += _T("\\");
    }
    BeginWaitCursor();
    ifstream file(base_address + _T("typeDB.csv"));//(base_address+"typeDB.TXT");
    db.ReadCSVFile(file);

    // 비트맵 출력 벡터 초기화
    CRect rec1, rec2, rec3;

    for (int i = 0, temp_sheet; i < db.m_nChar - 1; i++)
    {
        temp_sheet = db.m_Chars[i].m_sheet;
        if (temp_sheet == 1)      sheet123[0].push_back(db.m_Chars[i]);
        else if (temp_sheet == 2)  sheet123[1].push_back(db.m_Chars[i]);
        else if (temp_sheet == 3) sheet123[2].push_back(db.m_Chars[i]);
    }
    CStatic* size = (CStatic*)GetDlgItem(IDC_STATIC1);
    CRect rect;
    size->GetClientRect(rect);
    float ratiox = (float)rect.right / 9912;
    float ratioy = (float)rect.bottom / 7015;

    CString str;

    sheet_num_array[0] = 0;
    sheet_num_array[1] = 0;
    sheet_num_array[2] = 0;

    //--------연산---------------------------------------------

    // 글자 수 계산
        // 글자 수 계산
    for (int i = 0, temp_sheet; i < db.m_nChar - 1; i++)
    {
        temp_sheet = db.m_Chars[i].m_sheet;

        // 페이지당 글자 수   
        sheet_num_array[temp_sheet - 1]++;

        // 글자 종류
        kind.insert(db.m_Chars[i].m_char); // 전체
        kind_array[temp_sheet - 1].insert(db.m_Chars[i].m_char); // 장별     

    }
    // 페이지당 글자 종류 연산 결과를 저장
    sheet_Ttext = kind.size();
    for (int i = 0; i < 3; i++)
        sheet_text_array[i] = kind_array[i].size();

    // 전체 한글 활자 수, 폴더 갯수
    folder_num = 0;
    iter = kind.begin();
    for (int t = 0; t < kind.size(); t++)
    {
        CString address = base_address + _T("03_type\\") + *iter + _T("\\");
        folder_num += GetDirFilesNum(address);
        iter++;
    }

    //--------출력--------------------------------------------

    str.Format(_T("%d"), sheet_num);
    m_edit_sheet.SetWindowTextW(str);

    // 리스트 위쪽 페이지 번호 출력 포멧
    str.Format(_T("/%d장"), 3);
    m_static_sheets.SetWindowTextW(str);

    // 책 사진 아래에 페이지 번호 출력
    str.Format(_T("%d장"), sheet_num);
    m_static_sheet_bottom.SetWindowTextW(str);

    // 전체 한글 글자 수 출력
    str.Format(_T("%d 개"), (sheet_num_array[0] + sheet_num_array[1] + sheet_num_array[2]));
    m_static_num_1.SetWindowText(str);

    // 전체 한글 글자 종류 출력
    str.Format(_T("%d 종"), sheet_Ttext);
    m_static_num_2.SetWindowText(str);

    // 전체 활자 수 출력
    str.Format(_T("%d 개"), folder_num);
    m_static_num_3.SetWindowText(str);

    m_pagebitmap[0].Load(base_address + _T("01_scan\\001.jpg"));
    m_pagebitmap[1].Load(base_address + _T("01_scan\\002.jpg"));
    m_pagebitmap[2].Load(base_address + _T("01_scan\\003.jpg"));

    //--------비트맵 출력----------------------------------------
    CWnd* pWnd1 = (CWnd*)GetDlgItem(IDC_STATIC1);
    CDC* dc1 = pWnd1->GetDC();
    CStatic* size1 = (CStatic*)GetDlgItem(IDC_STATIC1);
    CRect rect1;
    size1->GetClientRect(rect1);
    m_pagebitmap[0].Draw(dc1->m_hDC, rect1);

    PrintData();
    PrintList();
    printRgn();

    CBrush brush(RGB(255, 0, 0));
    dc1->FrameRgn(rgn[sheet_num - 1][csv_now], &brush, 1, 1);
    CString temp;
    temp.Format(_T("03_type\\%s\\%d\\%s"), db.m_Chars[csv_now].m_char, db.m_Chars[csv_now].m_type, list_fileName[list_selected + 2]);
    CString address_ = base_address + temp;

    PrintBitmap(address_, IDC_STATIC2);
    PrintBitmap(address_, IDC_STATIC3);

    CString tmp4;
    tmp4.Format(_T("_%d"), db.m_Chars[csv_now].m_type);
    CString tmp5 = base_address + _T("\\04_3d\\") + db.m_Chars[csv_now].m_char + tmp4 + _T(".stl");

    callVtk(tmp5);

    EndWaitCursor();

    // Edit bookname에 책 제목 띄우기
    str.Format(_T("%s"), base_address);
    str = str.Left(base_address.GetLength() - 1); // 왼쪽 '//' 지우기
    str = str.Mid(str.ReverseFind((TCHAR)_T('\\')) + 1); // 맨 마지막 폴더 이름만 추출하기 
    m_edit_bookname.SetWindowTextW(str);
}

//-----------함수------------------------------------

void CteamDlg::PrintList()
{
    m_ListCtrl.DeleteAllItems();
    for (int i = 0; i < 15; list_fileName[i++].Empty());

    CString temp, tmp1;
    temp.Format(_T("\\%d"), db.m_Chars[csv_now].m_type);

    CString address = base_address + "03_type\\" + db.m_Chars[csv_now].m_char + temp + "\\*.*";

    //검색 클래스
    CFileFind finder;
    BOOL bWorking = finder.FindFile(address);

    list_num = 0;
    while (bWorking) // 파일 읽어오기
    {
        bWorking = finder.FindNextFile();    //다음 파일이 존재하면다면 TRUE 반환

        if (finder.IsArchived())
            list_fileName[list_num] = finder.GetFileName();

        list_num++;
    }

    for (int i = 2; i < list_num; i++)
    {
        int num = m_ListCtrl.GetItemCount();

        tmp1 = _T("_");

        if (list_fileName[i].GetAt(1) == tmp1) {
            m_ListCtrl.InsertItem(num, list_fileName[i].Left(1)); // (n,0)번째 내용
        }
        else
            m_ListCtrl.InsertItem(num, list_fileName[i].Left(2)); // (n,0)번째 내용

        m_ListCtrl.SetItem(num, 1, LVIF_TEXT, list_fileName[i].Right(13).Left(4), NULL, NULL, NULL, NULL); //(n,1)번째 내용
        m_ListCtrl.SetItem(num, 2, LVIF_TEXT, list_fileName[i].Right(8).Left(4), NULL, NULL, NULL, NULL); //(n,2)번째 내용

        if (sheet_num == list_fileName[i].Left(1))
            list_selected = i - 1;
    }
    temp.Format(_T("/%d 개"), list_num - 2);
    m_static_types.SetWindowTextW(temp);
}

void CteamDlg::PrintData()
{
    CString str;
    // 장내 한글 글자 수 출력
    str.Format(_T("%d 개"), (int)sheet_num_array[sheet_num - 1]);
    m_static_num_4.SetWindowText(str);

    // 장내 활자 수 출력
    m_static_num_6.SetWindowText(str);

    // 장내 한글 글자 종류 출력
    str.Format(_T("%d 종"), (int)sheet_text_array[sheet_num - 1]);
    m_static_num_5.SetWindowText(str);


    //--------활자 정보 출력-------------------------------------

    // 현재 출력하고 있는 활자의 char 출력
    m_static_char.SetWindowTextW(db.m_Chars[csv_now].m_char);

    // 현재 출력하고 있는 활자의 위치 출력
    str.Format(_T("%d장 %d행 %d열"), db.m_Chars[csv_now].m_sheet, db.m_Chars[csv_now].m_line, db.m_Chars[csv_now].m_order);
    m_static_type1.SetWindowTextW(str);
}

void CteamDlg::PrintBitmap(CString address, int nID)
{
    CClientDC dc(GetDlgItem(nID));

    CRect rect;
    GetDlgItem(nID)->GetClientRect(&rect);

    Graphics mainG(dc.GetSafeHdc());

    Bitmap memBmp(rect.Width(), rect.Height());

    Image img(address);

    mainG.DrawImage(&img, rect.left, rect.top, rect.right, rect.bottom);
}

int GetDirFilesNum(CString dirName)
{
    int count = 0;
    CFileFind finder;

    BOOL bWorking = finder.FindFile(dirName + "/*.*");

    while (bWorking)
    {
        bWorking = finder.FindNextFile();
        if (finder.IsDots())
            continue;
        count++;
    }
    finder.Close();

    return count;
}

void CteamDlg::OnNMClickListChars(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (!start_) return;

    CString temp;
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR; // 행클릭시 행 넘버 가져오기
    list_selected = pNMListView->iItem;

    //----선택한 리스트 번호가 list_selected에 저장되어있으니까 그걸 가지고 비트맵 출력해야 함---------
    MessageBox(list_fileName[list_selected + 2], NULL, MB_OK);

    temp.Format(_T("03_type\\%s\\%d\\%s"), db.m_Chars[csv_now].m_char, db.m_Chars[csv_now].m_type, list_fileName[list_selected + 2]);
    CString address_ = base_address + temp;

    MessageBox(address_, NULL, MB_OK);

    // 여기서 address_의 사진을 출력
    PrintBitmap(address_, IDC_STATIC3);

    temp.Format(_T("%d"), list_selected - 1);
    m_edit_type.SetWindowTextW(temp);

    *pResult = 0;
}

void CteamDlg::OnStnClickedStatic1()
{
    if (!start_) return;

    list_selected = 0;
    printRgn();

    CString temp, tmp1;
    temp.Format(_T("\\%d"), db.m_Chars[csv_now].m_type);

    CString address = base_address + "03_type\\" + db.m_Chars[csv_now].m_char + temp + "\\*.*";

    //검색 클래스
    CFileFind finder;
    BOOL bWorking = finder.FindFile(address);
    for (int j = 0; j < 30; j++) list_fileName[j] = _T("");
    int k = 0;

    while (bWorking) // 파일 읽어오기
    {
        bWorking = finder.FindNextFile();   //다음 파일이 존재하면다면 TRUE 반환
        if (finder.IsArchived())
            list_fileName[k] = finder.GetFileName();
        k++;
    }

    CString img;
    CString add;
    tmp1 = _T("_");
    for (int i = 2; i < k; i++)
    {
        if (list_fileName[i].GetAt(1) == tmp1)
            img = list_fileName[i].Left(1);
        else
            img = list_fileName[i].Left(2);
        CString tmp2;
        tmp2.Format(_T("%d"), db.m_Chars[csv_now].m_sheet);
        if (img == tmp2)
        {
            add = base_address + "03_type\\" + db.m_Chars[csv_now].m_char + temp + "\\" + list_fileName[i];
            break;
        }

    }
    PrintBitmap(add, IDC_STATIC2);
    PrintBitmap(add, IDC_STATIC3);

    PrintData();
    PrintList();

    CString tmp2;
    tmp2.Format(_T("_%d"), db.m_Chars[csv_now].m_type);
    CString tmp3 = base_address + _T("\\04_3d\\") + db.m_Chars[csv_now].m_char + tmp2 + _T(".stl");

    callVtk(tmp3);

    CString tmp4;
    tmp4.Format(_T("%d"), list_selected + 1);

    m_edit_type.SetWindowTextW(tmp4);

    // 리스트 박스 안에서 해당 페이지의 글자가 가장 먼저 하이라이트 되게 하는 코드
    CString list_sheet_array[30];   // 현재 리스트 박스에 출력되고 있는 글자들의 sheet값을 추출하여 Cstring 타입으로 저장

    for (int i = 0; i < list_num - 2; i++)
    {
        temp = _T("_");
        if (list_fileName[i + 2].GetAt(1) == temp)
            list_sheet_array[i] = list_fileName[i + 2].Left(1);
        else
            list_sheet_array[i] = list_fileName[i + 2].Left(2);
    }

    for (int i = 0; i < list_num - 2; i++)
    {
        temp.Format(_T("%d"), db.m_Chars[csv_now].m_sheet);
        if (list_sheet_array[i] == temp)
        {
            list_selected = i + 2;
            break;
        }
    }

    temp.Format(_T("%d"), list_selected - 1);
    m_edit_type.SetWindowTextW(temp);

    temp.Format(_T("03_type\\%s\\%d\\%s"), db.m_Chars[csv_now].m_char, (int)db.m_Chars[csv_now].m_type, list_fileName[list_selected]);
    CString address_ = base_address + temp;

    temp.Format(_T("%d"), list_selected - 1);
    m_edit_type.SetWindowTextW(temp);

    // 여기서 address_의 사진을 출력
    PrintBitmap(address_, IDC_STATIC3);
}

void CteamDlg::ResizeVtkWindow()
{
    CRect rc;
    CWnd* pWnd = GetDlgItem(IDC_STATIC_FRAME);
    if (pWnd)
    {
        GetDlgItem(IDC_STATIC_FRAME)->GetClientRect(rc);
        m_vtkWindow->SetSize(rc.Width(), rc.Height());
    }
}

void CteamDlg::InitVtkWindow(void* hWnd)
{
    if (m_vtkWindow == NULL)
    {
        vtkSmartPointer<vtkRenderWindowInteractor> interactor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();

        interactor->SetInteractorStyle(
            vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New());

        vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
        renderer->SetBackground(0.0, 0.0, 0.0);

        m_vtkWindow = vtkSmartPointer<vtkRenderWindow>::New();
        m_vtkWindow->SetParentId(hWnd);
        m_vtkWindow->SetInteractor(interactor);
        m_vtkWindow->AddRenderer(renderer);
        m_vtkWindow->Render();
    }
}

void CteamDlg::callVtk(CString& str)
{
    USES_CONVERSION;
    CHAR* sz = T2A(str);

    std::string temp = CW2A(str);

    vtkSmartPointer<vtkSTLReader> pSTLReader =
        vtkSmartPointer<vtkSTLReader>::New();
    pSTLReader->SetFileName(sz);
    pSTLReader->Update();

    vtkSmartPointer<vtkPolyData>pPolyData =
        pSTLReader->GetOutput();

    vtkSmartPointer<vtkPolyDataMapper>mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(pPolyData);
    vtkSmartPointer<vtkActor>actor =
        vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkRenderer>renderer =
        vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground(.1, .2, .3);
    renderer->ResetCamera();

    vtkSmartPointer<vtkLight>newLight = vtkSmartPointer<vtkLight>::New();
    newLight->SetColor(1, 1, 1);
    newLight->SetFocalPoint(0, 0, 0);
    newLight->SetPosition(-3, 0, 5);

    vtkSmartPointer<vtkLight>newLight2 = vtkSmartPointer<vtkLight>::New();
    newLight2->SetColor(1, 1, 1);
    newLight2->SetFocalPoint(0, 0, 0);
    newLight2->SetPosition(3, 0, 5);

    renderer->AddLight(newLight);
    renderer->AddLight(newLight2);

    m_vtkWindow->AddRenderer(renderer);
    m_vtkWindow->Render();
}

void CteamDlg::OnDeltaposSpinType(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (!start_) return;
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    list_selected -= (pNMUpDown->iDelta); // 값 = 현재 값 + 증감   
    if (list_selected < 0) list_selected = list_selected + list_num - 2;
    list_selected = list_selected % (list_num - 2);

    CString temp;
    temp.Format(_T("%d"), list_selected + 1);
    m_edit_type.SetWindowTextW(temp);

    temp.Format(_T("03_type\\%s\\%d\\%s"), (CString)db.m_Chars[csv_now].m_char, (int)db.m_Chars[csv_now].m_type, (CString)list_fileName[list_selected + 2]);
    CString address_ = base_address + temp;
    // 여기서 address_의 사진을 출력
    PrintBitmap(address_, IDC_STATIC3);

    *pResult = 0;
}

void CteamDlg::printRgn(void)
{
    //------------리전 백터 초기화-------------------------------------------
    for (int i = 0; i < sheet_num; i++)
    {

        rgn[i].clear();
        vector<CRgn*>().swap(rgn[i]);
    }
    //-------------마우스 위치 가져오기--------------------------------------
    int count = 0;
    CPoint point;
    ::GetCursorPos(&point);
    ScreenToClient(&point);

    pWnd = (CWnd*)GetDlgItem(IDC_STATIC1);
    cdc = pWnd->GetDC();
    CStatic* size = (CStatic*)GetDlgItem(IDC_STATIC1);
    CStatic* pos = size;

    CRect rect, posRect;
    size->GetClientRect(rect);
    pos->GetWindowRect(posRect);
    ScreenToClient(posRect);
    //-------------마우스 위치 픽쳐 컨트롤에서의 좌표로 변환------------------
    POINT cursor = { point.x - posRect.left ,point.y - posRect.top };
    float ratiox = (float)rect.right / 9912;        //원본 파일의 폭에 대한 비율
    float ratioy = (float)rect.bottom / 7015;       //원본 파일의 높이에 대한 비율
    cdc->SetMapMode(MM_TEXT);
    //-----------------------페이지 위치에 맞는 리전 생성--------------------
    for (int i = 0; i < sheet123[sheet_num - 1].size(); i++)
    {
        rgn[sheet_num - 1].push_back(new CRgn);
        rgn[sheet_num - 1][i]->CreateRectRgn(sheet123[sheet_num - 1][i].m_sx * ratiox, sheet123[sheet_num - 1][i].m_sy * ratioy, (sheet123[sheet_num - 1][i].m_sx + sheet123[sheet_num - 1][i].m_width) * ratiox, (sheet123[sheet_num - 1][i].m_sy + sheet123[sheet_num - 1][i].m_height) * ratioy);
    }

    m_pagebitmap[sheet_num - 1].Draw(cdc->m_hDC, rect);//페이지 출력
    //----------------리전출력------------------------------------------------------
    for (int i = 0; i < sheet123[sheet_num - 1].size(); i++)
    {
        ///////------------------------마우스로 선택된 리전
        if (rgn[sheet_num - 1][i]->PtInRegion(cursor))//마우스 위치가 리전 안에 있으면
        {
            CBrush brush(RGB(255, 0, 0));
            pre_csv_now = i;//이전 출력 csv 위치
            cdc->FrameRgn(rgn[sheet_num - 1][i], &brush, 1, 1);//빨간색으로 설정
            for (int j = 0; j < db.m_Chars.size() - 1; j++)
            {
                if ((db.m_Chars[j].m_char == sheet123[sheet_num - 1][i].m_char) && (db.m_Chars[j].m_sheet == sheet123[sheet_num - 1][i].m_sheet) && (db.m_Chars[j].m_type == sheet123[sheet_num - 1][i].m_type))csv_now = j;
                count++;
            }
        }
        else//--------------------------마우스로 선택 안된 리전
        {
            CBrush brush(RGB(0, 255, 0));
            cdc->FrameRgn(rgn[sheet_num - 1][i], &brush, 1, 1);///초록색으로 설정
        }
    }
    if (!count)//리전이 아무것도 선택이 안되면
    {
        CBrush brush(RGB(255, 0, 0));
        cdc->FrameRgn(rgn[sheet_num - 1][pre_csv_now], &brush, 1, 1);//이전에 선택된 리전을 빨간색으로 설정
    }
    ReleaseDC(cdc);
}