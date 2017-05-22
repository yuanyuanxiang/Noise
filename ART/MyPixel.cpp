#include "stdafx.h"
#include "MyPixel.h"


CMyPixel::CMyPixel()
{
	m_nRow = 0;
	m_nCol = 0;
	m_dLeftBottom = CMyPoint(0, 0);
	m_dRightTop = CMyPoint(0, 0);
	m_dValue = 0;
}

CMyPixel::CMyPixel(CMyPoint LeftBottom, CMyPoint RightTop)
{
	m_nRow = 0;
	m_nCol = 0;
	m_dLeftBottom = LeftBottom;
	m_dRightTop = RightTop;
	m_dValue = 0;
}

CMyPixel::CMyPixel(int Row, int Col, CMyPoint LeftBottom, CMyPoint RightTop)
{
	m_nRow = Row;
	m_nCol = Col;
	m_dLeftBottom = LeftBottom;
	m_dRightTop = RightTop;
	m_dValue = 0;
}

CMyPixel::~CMyPixel()
{

}