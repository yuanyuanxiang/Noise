#include "stdafx.h"
#include "SpaceFilter.h"

// 构造
CSpaceFilter::CSpaceFilter()
{
	m_nSize = 0;
	m_nRow = 0;
	m_nCol = 0;
	m_nSizeIsEven = 0;
	m_Sector = NULL;
}

// 析构
CSpaceFilter::~CSpaceFilter()
{
	if (m_Sector != NULL)
	{
		delete m_Sector;
	}
}

// image- 图像,bmpInfo-图像信息,curChannel-通道,row-正在处理的中心点行坐标,col-列坐标,size-滤波器大小
BOOL CSpaceFilter::Create(BYTE* dataSrc, BITMAP bmpInfo, int curChannel, int row, int col, int size)
{
	m_nSize = size;
	m_nLength = size*size;
	m_nSizeIsEven = (m_nSize+1)%2;
	m_Sector = new int[m_nLength];
	int R = m_nSize/2;
	m_nRow = R-m_nSizeIsEven;
	m_nCol = R-m_nSizeIsEven;

	int nChannel = bmpInfo.bmWidthBytes/bmpInfo.bmWidth;
	for(int k = 0, y = row-R+m_nSizeIsEven; y<=row+R; y++)
	{
		for(int x = col-R+m_nSizeIsEven; x<=col+R; x++)
		{
			if (0<=x && x<bmpInfo.bmWidth && 0<=y && y<bmpInfo.bmHeight)
			{
				BYTE* pixel = dataSrc + x*nChannel + y*bmpInfo.bmWidthBytes;
				m_Sector[k] = (int)*(pixel+curChannel);
			}
			else
			{
				m_Sector[k] = 0;
			}
			k++;
		}
	}
	return TRUE;
}

int CSpaceFilter::FindMax()
{
	int MAX = -1;
	for (int i = 0; i<m_nLength; i++)
	{
		if (m_Sector[i]>MAX)
		{
			MAX = m_Sector[i];
		}
	}
	return MAX;
}

int CSpaceFilter::FindMin()
{
	int MIN = 256;
	for (int i = 0; i<m_nLength; i++)
	{
		if (m_Sector[i]<MIN)
		{
			MIN = m_Sector[i];
		}
	}
	return MIN;
}

int CSpaceFilter::FindMed()
{
	int* Temp = new int[m_nLength], SWAP;
	for (int i = 0; i<m_nLength; i++)
	{//拷贝一份
		Temp[i] = m_Sector[i];
	}
	//对拷贝进行排序，以获取中值
	for (int i = 0; i<m_nLength; i++)
	{
		for (int j = i+1; j<m_nLength; j++)
		{
			if (Temp[j]<Temp[i])
			{
				SWAP = Temp[j];
				Temp[j] = Temp[i];
				Temp[i] = SWAP;
			}
		}
	}
	int MED1 = Temp[m_nLength/2-m_nSizeIsEven];
	int MED2 = Temp[m_nLength/2];
	delete [] Temp;
	return (MED1+MED2)/2;
}

int CSpaceFilter::Visit(int x, int y)
{
	return m_Sector[x + y*m_nSize];
}