
// team_Dlg.cpp: ���� ����

#include "pch.h"
#include "framework.h"
#include "team_.h"
#include "team_Dlg.h"
#include "afxdialogex.h"
#include "csvdata_structure.h"
#include "Universial_Parameter.h"
#include "NewDialog.h"
#include <clocale>
#include <vector>
#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int GetDirFilesNum(CString dirName);

// CteamDlg ��ȭ ����
CteamDlg::CteamDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_TEAM__DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    sheet_num = 1;
    csv_now = 0; // 0~351
    start_ = false;


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
}

BEGIN_MESSAGE_MAP(CteamDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SHEET, &CteamDlg::OnDeltaposSpinSheet)
    ON_BN_CLICKED(IDC_BUTTON_OPEN, &CteamDlg::OnBnClickedButtonOpen)
    ON_STN_CLICKED(IDC_STATIC1, &CteamDlg::OnStnClickedStatic1)
END_MESSAGE_MAP()


//-----------------------��������-----------------------------------------------------------------------------------------------

// ��Ʈ�� ����� ���� ���� ����
vector<SCharInfo> sheet1;
vector<SCharInfo> sheet2;
vector<SCharInfo> sheet3;
vector<CRgn*> rgn1;
vector<CRgn*> rgn2;
vector<CRgn*> rgn3;
CDC* cdc;
CWnd* pWnd;
//-------------------------------------------------------------------------------------------------------------------------------


// CteamDlg �޽��� ó����

BOOL CteamDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
    // �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
    SetIcon(m_hIcon, TRUE);         // ū �������� �����մϴ�.
    SetIcon(m_hIcon, FALSE);      // ���� �������� �����մϴ�.

    // TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
    db.ReadCSVFile(file);

    // ����Ʈ �ʱ�ȭ
    m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_ListCtrl.InsertColumn(0, TEXT("��"), LVCFMT_CENTER, 30);
    m_ListCtrl.InsertColumn(1, TEXT("sx"), LVCFMT_CENTER, 45);
    m_ListCtrl.InsertColumn(2, TEXT("sy"), LVCFMT_CENTER, 45);

    // static �ʱ� ���
    m_static_sheets.SetWindowTextW(_T("/0��"));

    // ���� ��Ʈ�� ����
    m_spin_sheet.SetRange(1, 3);
    m_spin_sheet.SetPos(1);

    // ������ ���� ���� �ʱ�ȭ
    sheet_Ttext = 0;
    sheet_Ttype = 0;

    // ��Ʈ�� ��� ���� �ʱ�ȭ
    CRect rec1, rec2, rec3;

    for (int i = 0, temp_sheet; i < db.m_nChar - 1; i++)
    {
        temp_sheet = db.m_Chars[i].m_sheet;
        if (temp_sheet == 1)      sheet1.push_back(db.m_Chars[i]);
        else if (temp_sheet == 2)  sheet2.push_back(db.m_Chars[i]);
        else if (temp_sheet == 3)  sheet3.push_back(db.m_Chars[i]);
    }


    CStatic* size = (CStatic*)GetDlgItem(IDC_STATIC1);//
    CRect rect;
    size->GetClientRect(rect);
    float ratiox = (float)rect.right / 9912;
    float ratioy = (float)rect.bottom / 7015;


    for (int i = 0; i < sheet1.size() - 1; i++)
    {
        rgn1.push_back(new CRgn);
        rgn1[i]->CreateRectRgn(sheet1[i].m_sx * ratiox, sheet1[i].m_sy * ratioy, (sheet1[i].m_sx + sheet1[i].m_width) * ratiox, (sheet1[i].m_sy + sheet1[i].m_height) * ratioy);

    }

    for (int i = 0; i < sheet2.size() - 1; i++)
    {
        rgn2.push_back(new CRgn);
        rgn2[i]->CreateRectRgn(sheet2[i].m_sx * ratiox, sheet2[i].m_sy * ratioy, (sheet2[i].m_sx + sheet2[i].m_width) * ratiox, (sheet2[i].m_sy + sheet2[i].m_height) * ratioy);

    }

    for (int i = 0; i < sheet3.size() - 1; i++)
    {
        rgn3.push_back(new CRgn);
        rgn3[i]->CreateRectRgn(sheet3[i].m_sx * ratiox, sheet3[i].m_sy * ratioy, (sheet3[i].m_sx + sheet3[i].m_width) * ratiox, (sheet3[i].m_sy + sheet3[i].m_height) * ratioy);
    }

    m_pagebitmap1.Load(_T("C:\\Users\\spc\\Desktop\\����õ������ �ǻ�\\����õ������ �ǻ�\\01_scan\\001.jpg"));
    m_pagebitmap2.Load(_T("C:\\Users\\spc\\Desktop\\����õ������ �ǻ�\\����õ������ �ǻ�\\01_scan\\002.jpg"));
    m_pagebitmap3.Load(_T("C:\\Users\\spc\\Desktop\\����õ������ �ǻ�\\����õ������ �ǻ�\\01_scan\\003.jpg"));


    return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


void CteamDlg::OnPaint()
{

    if (IsIconic())
    {

        CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // �������� �׸��ϴ�.
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.
        OnDraw(&dc);
        CDialogEx::OnPaint();


    }
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CteamDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CteamDlg::OnDeltaposSpinSheet(NMHDR* pNMHDR, LRESULT* pResult)
{

    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if (!start_) return;

    // ������ ��ȣ �ٲٱ�

    int iVal = (pNMUpDown->iPos) + (pNMUpDown->iDelta); // �� = ���� �� + ����   

    if (iVal <= 0) iVal = 3;
    else if (iVal >= 4) iVal = 1;

    CString str;

    sheet_num = iVal;
    str.Format(_T("%d"), sheet_num);
    m_edit_sheet.SetWindowTextW(str);

    sheet_text.Format(_T("%d��"), sheet_num);
    m_static_sheet_bottom.SetWindowTextW(sheet_text);

    //--------��Ʈ�� ���----------------------------------------
    pWnd = (CWnd*)GetDlgItem(IDC_STATIC1);
    cdc = pWnd->GetDC();
    CStatic* size = (CStatic*)GetDlgItem(IDC_STATIC1);//
    CRect rect;
    size->GetClientRect(rect);
    float ratiox = (float)rect.right / 9912;
    float ratioy = (float)rect.bottom / 7015;
    cdc->SetMapMode(MM_TEXT);

    ///this->SetDlgItemInt(IDC_STATIC_NUM_6, (int)tkind_array[sheet_num].size());
    switch (sheet_num)
    {
    case 1:
    {

        m_pagebitmap1.Draw(cdc->m_hDC, rect);
        //m_pagebitmap1.Detach();
        for (int i = 0; i < sheet1.size() - 1; i++)
        {
            CBrush brush(RGB(0, 255, 0));
            CPen pen(PS_SOLID, 2, RGB(0, 255, 0));
            cdc->FrameRgn(rgn1[i], &brush, 2, 2);//sheet1[i].m_width, sheet1[i].m_height);
        }
        break;
    }

    case 2:
    {

        m_pagebitmap2.Draw(cdc->m_hDC, rect);
        // m_pagebitmap2.Detach();
        for (int i = 0; i < sheet2.size() - 1; i++)
        {
            CBrush brush(RGB(0, 255, 0));
            CPen pen(PS_SOLID, 2, RGB(0, 255, 0));
            cdc->FrameRgn(rgn2[i], &brush, 2, 2);//sheet1[i].m_width, sheet1[i].m_height);
        }
        break;
    }
    case 3:
    {

        m_pagebitmap3.Draw(cdc->m_hDC, rect);
        //m_pagebitmap3.Detach();
        for (int i = 0; i < sheet3.size() - 1; i++)
        {
            CBrush brush(RGB(0, 255, 0));
            CPen pen(PS_SOLID, 2, RGB(0, 255, 0));
            cdc->FrameRgn(rgn3[i], &brush, 2, 2);//sheet1[i].m_width, sheet1[i].m_height);
        }
        break;
    }

    }

    ReleaseDC(cdc);




    //--------���--------------------------------------------


    // �峻 �ѱ� ���� �� ���
    str.Format(_T("%d ��"), sheet_num_array[sheet_num - 1]);
    m_static_num_4.SetWindowText(str);

    // �峻 Ȱ�� �� ���
    m_static_num_6.SetWindowText(str);

    // �峻 �ѱ� ���� ���� ���
    str.Format(_T("%d ��"), sheet_text_array[sheet_num - 1]);
    m_static_num_5.SetWindowText(str);


    //--------Ȱ�� ���� ���-------------------------------------

    // ���� ����ϰ� �ִ� Ȱ���� char ���
    m_static_char.SetWindowTextW(db.m_Chars[csv_now].m_char);

    // ���� ����ϰ� �ִ� Ȱ���� ��ġ ���
    str.Format(_T("%d�� %d�� %d��"), db.m_Chars[csv_now].m_sheet, db.m_Chars[csv_now].m_line, db.m_Chars[csv_now].m_order);
    m_static_type1.SetWindowTextW(str);


    //--------����Ʈ ���----------------------------------------
    st_for_list my_list[6] = { 0, };
    m_ListCtrl.DeleteAllItems();

    int temp_num = 0;
    for (int i = 0; i < db.m_nChar - 1; i++)
        if (db.m_Chars[csv_now].m_char == db.m_Chars[i].m_char && db.m_Chars[csv_now].m_type == db.m_Chars[i].m_type)
        {
            temp_num++;
            my_list[i].l_line = db.m_Chars[i].m_line;
            my_list[i].l_order = db.m_Chars[i].m_order;
            my_list[i].l_sheet = db.m_Chars[i].m_sheet;
        }

    for (int i = 0; i < temp_num; i++)
    {
        int num = m_ListCtrl.GetItemCount();

        CString str1;
        CString str2;
        CString str3;

        str1.Format(_T("%d"), my_list[i].l_sheet);
        str2.Format(_T("%d"), my_list[i].l_line);
        str3.Format(_T("%d"), my_list[i].l_order);


        m_ListCtrl.InsertItem(num, str1); // (n,0)��° ����
        m_ListCtrl.SetItem(num, 1, LVIF_TEXT, str2, NULL, NULL, NULL, NULL); //(n,1)��° ����
        m_ListCtrl.SetItem(num, 2, LVIF_TEXT, str3, NULL, NULL, NULL, NULL); //(n,2)��° ����

    }

    *pResult = 0;

}



void CteamDlg::OnBnClickedButtonOpen()
{
    //--------�ʱ� ����----------------------------------------
    NewDialog dlg;
    dlg.DoModal();

    start_ = true;
    //m_edit_bookname.SetWindowTextW(strstr_);

    sheet_num_array[0] = 0;
    sheet_num_array[1] = 0;
    sheet_num_array[2] = 0;

    //--------����---------------------------------------------

    // ���� �� ���
    for (int i = 0, temp_sheet; i < db.m_nChar - 1; i++)
    {
        temp_sheet = db.m_Chars[i].m_sheet;

        // �������� ���� ��   
        sheet_num_array[temp_sheet - 1] = sheet_num_array[temp_sheet - 1] + 1;

        // ���� ����
        kind.insert(db.m_Chars[i].m_char); // ��ü
        kind_array[temp_sheet - 1].insert(db.m_Chars[i].m_char); // �庰     

    }
    // �������� ���� ���� ���� ����� ����
    sheet_Ttext = kind.size();
    for (int i = 0; i < 3; i++)
        sheet_text_array[i] = kind_array[i].size();

    // ��ü �ѱ� Ȱ�� ��, ���� ����
    folder_num = 0;
    iter = kind.begin();
    for (int t = 0; t < kind.size(); t++)
    {
        CString address = _T("C:\\Users\\spc\\Desktop\\����õ������ �ǻ�\\����õ������ �ǻ�\\03_type\\");
        address += *iter;
        address += "\\";
        folder_num += GetDirFilesNum(address);
        iter++;
    }

    //--------���--------------------------------------------

    CString str;

    str.Format(_T("%d"), sheet_num);
    m_edit_sheet.SetWindowTextW(str);

    // ����Ʈ ���� ������ ��ȣ ��� ����
    str.Format(_T("/%d��"), 3);
    m_static_sheets.SetWindowTextW(str);

    // å ���� �Ʒ��� ������ ��ȣ ���
    str.Format(_T("%d��"), sheet_num);
    m_static_sheet_bottom.SetWindowTextW(str);


    // ��ü �ѱ� ���� �� ���
    str.Format(_T("%d ��"), (sheet_num_array[0] + sheet_num_array[1] + sheet_num_array[2]));
    m_static_num_1.SetWindowText(str);

    // ��ü �ѱ� ���� ���� ���
    str.Format(_T("%d ��"), sheet_Ttext);
    m_static_num_2.SetWindowText(str);

    // ��ü Ȱ�� �� ���
    str.Format(_T("%d ��"), folder_num);
    m_static_num_3.SetWindowText(str);
    //this->SetDlgItemTextW(IDC_STATIC_NUM_3, str);

    int count = 0;
    // ��ü �ѱ� Ȱ�� ��, ���� ����
    int folder_num = 0;
    iter = kind.begin();
    for (int t = 0; t < kind.size(); t++)
    {
        count++;

        CString address = _T("C:\\Users\\spc\\Desktop\\����õ������ �ǻ�\\����õ������ �ǻ�\\03_type\\");
        address += *iter;
        address += "\\";
        folder_num += GetDirFilesNum(address);
        iter++;
    }

    CString str_temptemp;

    str_temptemp.Format(_T("%d ��"), folder_num);

    this->SetDlgItemTextW(IDC_STATIC_NUM_3, str_temptemp);
    this->SetDlgItemInt(IDC_STATIC_NUM_6, tkind_array[sheet_num].size());


    CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC2);
    CDC* dc = pWnd->GetDC();
    CStatic* size = (CStatic*)GetDlgItem(IDC_STATIC2);
    CRect rect;
    size->GetClientRect(rect);
    CString address_str;
    address_str.Format(_T("C:\\Users\\spc\\Desktop\\����õ������ �ǻ�\\����õ������ �ǻ�\\03_type\\"));
    address_str += _T("db.m_Chars[csv_now].m_char\\");

    m_bitmap.Load(_T("C:\\Users\\spc\\Desktop\\����õ������ �ǻ�\\����õ������ �ǻ�\\03_type\\110A11A10000\\8\\2_6190_3705.png"));
    m_bitmap.Draw(dc->m_hDC, rect);
    m_bitmap.Detach();



    //--------��Ʈ�� ���----------------------------------------
    CWnd* pWnd1 = (CWnd*)GetDlgItem(IDC_STATIC1);
    CDC* dc1 = pWnd1->GetDC();
    CStatic* size1 = (CStatic*)GetDlgItem(IDC_STATIC1);
    CRect rect1;
    size1->GetClientRect(rect1);

    m_pagebitmap1.Draw(dc1->m_hDC, rect1);
    //m_pagebitmap1.Detach();
    for (int i = 0; i < sheet1.size() - 1; i++) {
        CBrush brush(RGB(0, 255, 0));
        CPen pen(PS_SOLID, 2, RGB(0, 255, 0));
        dc1->FrameRgn(rgn1[i], &brush, 2, 2);//sheet1[i].m_width, sheet1[i].m_height);
    }

    //--------���--------------------------------------------


    // �峻 �ѱ� ���� �� ���
    str.Format(_T("%d ��"), sheet_num_array[sheet_num - 1]);
    m_static_num_4.SetWindowText(str);

    // �峻 Ȱ�� �� ���
    m_static_num_6.SetWindowText(str);

    // �峻 �ѱ� ���� ���� ���
    str.Format(_T("%d ��"), sheet_text_array[sheet_num - 1]);
    m_static_num_5.SetWindowText(str);


    //--------Ȱ�� ���� ���-------------------------------------

    // ���� ����ϰ� �ִ� Ȱ���� char ���
    m_static_char.SetWindowTextW(db.m_Chars[csv_now].m_char);

    // ���� ����ϰ� �ִ� Ȱ���� ��ġ ���
    str.Format(_T("%d�� %d�� %d��"), db.m_Chars[csv_now].m_sheet, db.m_Chars[csv_now].m_line, db.m_Chars[csv_now].m_order);
    m_static_type1.SetWindowTextW(str);


    //--------����Ʈ ���----------------------------------------
    m_ListCtrl.DeleteAllItems();
    CString fileName[6] = { 0, };

    CString address_before = _T("C:\\Users\\spc\\Desktop\\����õ������ �ǻ�\\����õ������ �ǻ�\\03_type\\");

    CString temp;
    temp.Format(_T("\\%d"), db.m_Chars[csv_now].m_type);

    CString address_after = address_before + db.m_Chars[csv_now].m_char + temp + "\\*.*";

    MessageBox(temp, NULL, MB_OK);
    MessageBox(address_after, NULL, MB_OK);

    //�˻� Ŭ����
    CFileFind finder;
    BOOL bWorking = finder.FindFile(address_after);

    int k = 0;
    while (bWorking)
    {
        bWorking = finder.FindNextFile();    //���� ������ �����ϸ�ٸ� TRUE ��ȯ

        if (finder.IsArchived())
            fileName[k] = finder.GetFileName();

        k++;
    }

    for (int i = 2; i < k; i++)
    {
        int num = m_ListCtrl.GetItemCount();

        CString temp = _T("_");

        if (fileName[i].GetAt(1) == temp)
            m_ListCtrl.InsertItem(num, fileName[i].Left(1)); // (n,0)��° ����
        else
            m_ListCtrl.InsertItem(num, fileName[i].Left(2)); // (n,0)��° ����
        m_ListCtrl.SetItem(num, 1, LVIF_TEXT, fileName[i].Right(13).Left(4), NULL, NULL, NULL, NULL); //(n,1)��° ����
        m_ListCtrl.SetItem(num, 2, LVIF_TEXT, fileName[i].Right(8).Left(4), NULL, NULL, NULL, NULL); //(n,2)��° ����
    }

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
        {
            continue;
        }
        count++;
    }
    finder.Close();

    return count;
}


void CteamDlg::OnStnClickedStatic1()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CPoint point;
    ::GetCursorPos(&point);
    ScreenToClient(&point);
    AfxMessageBox(_T("�� �ǳ�"));
    // CDialogEx::OnLButtonUp(nFlags, point);
    CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC1);
    CDC* dc = pWnd->GetDC();
    CStatic* size = (CStatic*)GetDlgItem(IDC_STATIC1);//
    CStatic* pos = size;

    CRect rect, posRect;
    size->GetClientRect(rect);
    pos->GetWindowRect(posRect);
    ScreenToClient(posRect);
    POINT cursor = { cursor.x - posRect.left ,cursor.y - posRect.top, }
    float ratiox = (float)rect.right / 9912;
    float ratioy = (float)rect.bottom / 7015;
    dc->SetMapMode(MM_TEXT);

    ///this->SetDlgItemInt(IDC_STATIC_NUM_6, (int)tkind_array[sheet_num].size());
    switch (sheet_num)
    {
    case 1:
    {
        for (int i = 0; i < sheet1.size() - 1; i++)
        {
            if (rgn1[i]->PtInRegion(point))
            {
                CBrush brush(RGB(0, 255, 0));
                CPen pen(PS_SOLID, 2, RGB(0, 255, 0));
                dc->FrameRgn(rgn1[i], &brush, 2, 2);
            }
        }
        break;
    }

    case 2:
    {
        for (int i = 0; i < sheet2.size() - 1; i++)
        {
            if (rgn2[i]->PtInRegion(point))
            {
                CBrush brush(RGB(0, 255, 0));
                CPen pen(PS_SOLID, 2, RGB(0, 255, 0));
                dc->FrameRgn(rgn2[i], &brush, 2, 2);
            }
        }
        break;
    }
    case 3:
    {
        for (int i = 0; i < sheet3.size() - 1; i++)
        {
            if (rgn3[i]->PtInRegion(point))
            {
                CBrush brush(RGB(0, 255, 0));
                CPen pen(PS_SOLID, 2, RGB(0, 255, 0));
                dc->FrameRgn(rgn3[i], &brush, 2, 2);
            }
        }
        break;
    }

    }
}


void CteamDlg::OnDraw(CDC* pdc)
{
    if (!start_) return;

    CString str_temp;
    sheet_text.Format(_T("%d��"), sheet_num);
    m_static_sheet_bottom.SetWindowTextW(sheet_text);


    //--------���--------------------------------------------

    CString str;

    // �峻 �ѱ� ���� �� ���
    str.Format(_T("%d ��"), sheet_num_array[sheet_num - 1]);
    m_static_num_4.SetWindowText(str);

    // �峻 Ȱ�� �� ���
    m_static_num_6.SetWindowText(str);

    // �峻 �ѱ� ���� ���� ���
    str.Format(_T("%d ��"), sheet_text_array[sheet_num - 1]);
    m_static_num_5.SetWindowText(str);


    //--------Ȱ�� ���� ���-------------------------------------

    // ���� ����ϰ� �ִ� Ȱ���� char ���
    m_static_char.SetWindowTextW(db.m_Chars[csv_now].m_char);

    // ���� ����ϰ� �ִ� Ȱ���� ��ġ ���
    str.Format(_T("%d�� %d�� %d��"), db.m_Chars[csv_now].m_sheet, db.m_Chars[csv_now].m_line, db.m_Chars[csv_now].m_order);
    m_static_type1.SetWindowTextW(str);


    //--------����Ʈ ���----------------------------------------
    st_for_list my_list[6] = { 0, };
    m_ListCtrl.DeleteAllItems();

    int temp_num = 0;
    for (int i = 0; i < db.m_nChar - 1; i++)
        if (db.m_Chars[csv_now].m_char == db.m_Chars[i].m_char)
        {
            temp_num++;
            my_list[i].l_line = db.m_Chars[i].m_line;
            my_list[i].l_order = db.m_Chars[i].m_order;
            my_list[i].l_sheet = db.m_Chars[i].m_sheet;
        }

    for (int i = 0; i < temp_num; i++)
    {
        int num = m_ListCtrl.GetItemCount();

        CString str1;
        CString str2;
        CString str3;

        str1.Format(_T("%d"), my_list[i].l_sheet);
        str2.Format(_T("%d"), my_list[i].l_order);
        str3.Format(_T("%d"), my_list[i].l_sheet);

        m_ListCtrl.InsertItem(num, str1); // (n,0)��° ����
        m_ListCtrl.SetItem(num, 1, LVIF_TEXT, str2, NULL, NULL, NULL, NULL); //(n,1)��° ����
        m_ListCtrl.SetItem(num, 2, LVIF_TEXT, str3, NULL, NULL, NULL, NULL); //(n,2)��° ����

    }


    pWnd = (CWnd*)GetDlgItem(IDC_STATIC1);
    cdc = pWnd->GetDC();
    CStatic* size = (CStatic*)GetDlgItem(IDC_STATIC1);//
    CRect rect;
    size->GetClientRect(rect);
    float ratiox = (float)rect.right / 9912;
    float ratioy = (float)rect.bottom / 7015;
    cdc->SetMapMode(MM_TEXT);

    ///this->SetDlgItemInt(IDC_STATIC_NUM_6, (int)tkind_array[sheet_num].size());
    switch (sheet_num)
    {
    case 1:
    {

        m_pagebitmap1.Draw(cdc->m_hDC, rect);
        //m_pagebitmap1.Detach();
        for (int i = 0; i < sheet1.size() - 1; i++)
        {
            CBrush brush(RGB(0, 255, 0));
            CPen pen(PS_SOLID, 2, RGB(0, 255, 0));
            cdc->FrameRgn(rgn1[i], &brush, 2, 2);//sheet1[i].m_width, sheet1[i].m_height);
        }
        break;
    }

    case 2:
    {

        m_pagebitmap2.Draw(cdc->m_hDC, rect);
        // m_pagebitmap2.Detach();
        for (int i = 0; i < sheet2.size() - 1; i++)
        {
            CBrush brush(RGB(0, 255, 0));
            CPen pen(PS_SOLID, 2, RGB(0, 255, 0));
            cdc->FrameRgn(rgn2[i], &brush, 2, 2);//sheet1[i].m_width, sheet1[i].m_height);
        }
        break;
    }
    case 3:
    {

        m_pagebitmap3.Draw(cdc->m_hDC, rect);
        //m_pagebitmap3.Detach();
        for (int i = 0; i < sheet3.size() - 1; i++)
        {
            CBrush brush(RGB(0, 255, 0));
            CPen pen(PS_SOLID, 2, RGB(0, 255, 0));
            cdc->FrameRgn(rgn3[i], &brush, 2, 2);//sheet1[i].m_width, sheet1[i].m_height);
        }
        break;
    }

    }

    ReleaseDC(cdc);
}