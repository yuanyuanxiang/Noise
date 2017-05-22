#pragma once

//像素类：一个像素占用48

class CMyPixel
{
public:
	int                                   m_nRow;  //像素所在行(y坐标)
	int                                   m_nCol;  //像素所在列(x坐标)
	CMyPoint                       m_dLeftBottom;  //左下角坐标
	CMyPoint                         m_dRightTop;  //右上角坐标
	TYPE                                m_dValue;  //待求解的值

public:
	CMyPixel();
	CMyPixel(CMyPoint LeftBottom, CMyPoint RightTop);
	CMyPixel(int Row, int Col, CMyPoint LeftBottom, CMyPoint RightTop);
	~CMyPixel();
};