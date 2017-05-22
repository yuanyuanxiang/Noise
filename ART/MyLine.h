#pragma once

//交线类：一条交线占用16

class CMyLine
{
public:
	int                                   m_nRow;  //交线行下标(y坐标)
	int                                   m_nCol;  //交线列下标(x坐标)
	TYPE                               m_dLength;  //交线长度

public:
	CMyLine();
	CMyLine(int row, int col);
	~CMyLine();
};