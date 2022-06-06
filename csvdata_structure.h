#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <istream>

#include "afxwinappex.h"
#include "afxdialogex.h"
using namespace std;



struct SCharInfo {
    CString m_char;
    int m_type,
        m_sheet,
        m_sx,
        m_sy,
        m_line,
        m_order,
        m_width,
        m_height;
};

SCharInfo get1line(istream& file);
class CTypeDB {
public:
    int m_nSheet;
    int m_nChar;
    vector<SCharInfo> m_Chars;
    BOOL ReadCSVFile(istream& file);
    CTypeDB();
};

CTypeDB::CTypeDB()
{
    m_nSheet = 0;
    m_nChar = 353;
}
CTypeDB db;

BOOL CTypeDB::ReadCSVFile(istream& file)
{

    int count = 0;
    if (file.fail())  //만약 bad() 함수가 실패 하면..
    {
        return FALSE;//(cout << "해당 경로에 위치하는 파일이 존재하지 않습니다." << endl) && 0;
    }

    while (file.good()) //eof, bad, fail 함수가 거짓의 참을 반환할 때까지.
    {
        count++;
        if (count == 1)
        {
            get1line(file);
        }
        else
        {
            db.m_Chars.push_back(get1line(file));    //csv_read_row(file, ',');
        }
    }
    return TRUE;
}

SCharInfo get1line(istream& file)
{
    SCharInfo obj;
    vector<string> row;
    stringstream ss;
    int count = 0;
    bool inquotes = false;
    char delimiter = ',';

    while (file.good())
    {
        count++;
        char c = file.get();
        if (!inquotes && c == delimiter)//else
        {
            row.push_back(ss.str());
            ss.str("");

        }
        else if (!inquotes && (c == '\r' || c == '\n'))
        {
            if (file.peek() == '\n') { file.get(); }
            row.push_back(ss.str());
            break;
        }
        else
        {
            ss << c;
        }
    }
   
    obj.m_char = row[0].c_str();
    obj.m_type = (row[1] == (string)("type")) ? 0 : stoi(row[1]);
    obj.m_sheet = (row[2] == (string)("sheet")) ? 0 : stoi(row[2]);
    obj.m_sx = (row[3] == (string)("sx")) ? 0 : stoi(row[3]);
    obj.m_sy = (row[4] == (string)("sy")) ? 0 : stoi(row[4]);
    obj.m_line = (row[5] == (string)("line")) ? 0 : stoi(row[5]);
    obj.m_order = (row[6] == (string)("order")) ? 0 : stoi(row[6]);
    obj.m_width = (row[7] == (string)("width")) ? 0 : stoi(row[7]);
    obj.m_height = (row[8] == (string)("height")) ? 0 : stoi(row[8]);

    return obj;
}