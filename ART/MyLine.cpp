#include "stdafx.h"
#include "MyLine.h"

CMyLine::CMyLine()
{
	m_nRow = 0;
	m_nCol = 0;
	m_dLength = 0;
}

CMyLine::CMyLine(int row, int col)
{
	m_nRow = row;
	m_nCol = col;
	m_dLength = 0;
}

CMyLine::~CMyLine()
{

}