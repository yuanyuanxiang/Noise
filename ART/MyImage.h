#pragma once

//图像类

#include "MyLine.h"
#include "MyRay.h"
#include "MyPixel.h"
#include "MyPoint.h"
#include <vector>

//交线的定义：一条交线占用16*m_Line.Size()+8
typedef struct MyLines 
{
	vector<CMyLine*>              m_Line;  //包含一段一段的交线
	TYPE                     m_dMeasured;  //对应射线实际测量值
}LINES;

class CMyImage
{
public:
	TYPE                                   m_dWidth;  //图像宽度
	TYPE                                  m_dHeight;  //图像高度
	TYPE                              m_dWidthSpace;  //行方向间距(x)
	TYPE                             m_dHeightSpace;  //列方向间距(y)
	int                              m_nWidthSplits;  //行方向数量
	int                             m_nHeightSplits;  //列方向数量
	int                          m_nQuantityOfPixel;  //像素个数
	CMyPixel*                          m_PixelArray;  //像素数组(B)：48*m_nWidthSplits*m_nHeightSplits
	vector<LINES*>                  m_Intersections;  //交线数组(B)：(16*CMyIntersections::m_Line.Size()+8)*m_Intersections.size()

public:
	CMyImage();
	CMyImage(TYPE Width, TYPE Height, int WidthSplits, int HeightSplits);
	~CMyImage();

public:
	void CreateImage(const CMyImage& image);
	void Assign(const CMyImage& image);
	void CreateImage(TYPE Width, TYPE Height, int WidthSplits, int HeightSplits);
	CMyPixel GetSector(int Row, int Col, int* WhichSector);
	void Iterate(CMyPixel* Pixel, CMyRay* Ray, CMyPoint* Point, int* Direction);
	void Solve(CMyRay* Ray);
	void SolveAll(CMyRay Lines[], long LinesQuantity = 1);
	void ARTSingleLine(int nCurrentLine);
	void ARTSingleStep();
	void ARTSolution(int Steps = 16);
	void SaveResult(char* filepath);
	TYPE FindMax();
};