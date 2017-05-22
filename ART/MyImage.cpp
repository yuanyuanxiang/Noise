#include "stdafx.h"
#include "MyImage.h"

CMyImage::CMyImage()
{
	m_dWidth = 0;
	m_dHeight = 0;
	m_dWidthSpace = 0;
	m_dHeightSpace = 0;
	m_nWidthSplits = 0;
	m_nHeightSplits = 0;
	m_nQuantityOfPixel = 0;
	m_PixelArray = NULL;
	//m_Intersections
}

CMyImage::CMyImage(TYPE Width, TYPE Height, int WidthSplits, int HeightSplits)
{
	m_dWidth           = Width;                                    //x方向
	m_dHeight          = Height;                                   //y方向
	m_nWidthSplits     = WidthSplits;                              //x方向
	m_nHeightSplits    = HeightSplits;                             //y方向
	m_dWidthSpace      = m_dWidth/m_nWidthSplits;                  //x方向
	m_dHeightSpace     = m_dHeight/m_nHeightSplits;                //y方向
	m_nQuantityOfPixel = m_nWidthSplits*m_nHeightSplits;           //数量
	m_PixelArray       = new CMyPixel[m_nQuantityOfPixel];         //分配空间
	for (int i = 0; i<m_nHeightSplits; i++)                        //像素块赋值
	{
		for (int j = 0; j<m_nWidthSplits; j++)
		{
			m_PixelArray[j + i*m_nWidthSplits].m_nRow = i;         //y坐标
			m_PixelArray[j + i*m_nWidthSplits].m_nCol = j;         //x坐标
			m_PixelArray[j + i*m_nWidthSplits].m_dLeftBottom = CMyPoint(j*m_dWidthSpace, i*m_dHeightSpace);
			m_PixelArray[j + i*m_nWidthSplits].m_dRightTop = CMyPoint((j+1)*m_dWidthSpace, (i+1)*m_dHeightSpace);
			m_PixelArray[j + i*m_nWidthSplits].m_dValue = 0;
		}
	}
}

//生成当前对象的副本图像
void CMyImage::CreateImage(const CMyImage& image)
{
	m_dWidth           = image.m_dWidth;
	m_dHeight          = image.m_dHeight;
	m_nWidthSplits     = image.m_nWidthSplits;
	m_nHeightSplits    = image.m_nHeightSplits;
	m_dWidthSpace      = image.m_dWidthSpace;
	m_dHeightSpace     = image.m_dHeightSpace;
	m_nQuantityOfPixel = image.m_nQuantityOfPixel;
	m_PixelArray = new CMyPixel[m_nQuantityOfPixel];
	for (long i = 0; i<m_nQuantityOfPixel; i++)
	{
		m_PixelArray[i].m_nRow        = image.m_PixelArray[i].m_nRow;
		m_PixelArray[i].m_nCol        = image.m_PixelArray[i].m_nCol;
		m_PixelArray[i].m_dLeftBottom = image.m_PixelArray[i].m_dLeftBottom;
		m_PixelArray[i].m_dRightTop   = image.m_PixelArray[i].m_dRightTop;
		m_PixelArray[i].m_dValue      = image.m_PixelArray[i].m_dValue;
	}
	for (long i = 0; i<image.m_Intersections.size(); i++)
	{
		LINES* newLINE = new LINES;
		newLINE->m_dMeasured = image.m_Intersections[i]->m_dMeasured;
		m_Intersections.push_back(newLINE);
		for (int j = 0; j<image.m_Intersections[i]->m_Line.size(); j++)
		{
			CMyLine* newMyLine = new CMyLine;
			newMyLine->m_nRow    = image.m_Intersections[i]->m_Line[j]->m_nRow;
			newMyLine->m_nCol    = image.m_Intersections[i]->m_Line[j]->m_nCol;
			newMyLine->m_dLength = image.m_Intersections[i]->m_Line[j]->m_dLength;
			newLINE->m_Line.push_back(newMyLine);
		}
	}
}

//赋值：对当前对象进行更新
void CMyImage::Assign(const CMyImage& image)
{
	m_dWidth           = image.m_dWidth;
	m_dHeight          = image.m_dHeight;
	m_nWidthSplits     = image.m_nWidthSplits;
	m_nHeightSplits    = image.m_nHeightSplits;
	m_dWidthSpace      = image.m_dWidthSpace;
	m_dHeightSpace     = image.m_dHeightSpace;
	m_nQuantityOfPixel = image.m_nQuantityOfPixel;
	for (long i = 0; i<m_nQuantityOfPixel; i++)
	{
		m_PixelArray[i].m_nRow        = image.m_PixelArray[i].m_nRow;
		m_PixelArray[i].m_nCol        = image.m_PixelArray[i].m_nCol;
		m_PixelArray[i].m_dLeftBottom = image.m_PixelArray[i].m_dLeftBottom;
		m_PixelArray[i].m_dRightTop   = image.m_PixelArray[i].m_dRightTop;
		m_PixelArray[i].m_dValue      = image.m_PixelArray[i].m_dValue;
	}
	for (long i = 0; i<image.m_Intersections.size(); i++)
	{
		m_Intersections[i]->m_dMeasured = image.m_Intersections[i]->m_dMeasured;
		for (int j = 0; j<image.m_Intersections[i]->m_Line.size(); j++)
		{
			//特别注意m_Line是指针
			*(m_Intersections[i]->m_Line[j]) = *(image.m_Intersections[i]->m_Line[j]);
		}
	}
}

CMyImage::~CMyImage()
{
	delete [] m_PixelArray;
	for (long i = 0; i<m_Intersections.size(); i++)
	{
		for (long j = 0; j<m_Intersections[i]->m_Line.size(); j++)
		{
			if(m_Intersections[i]->m_Line[j] != NULL)
				delete m_Intersections[i]->m_Line[j];
			m_Intersections[i]->m_Line[j] = NULL;
		}
		m_Intersections[i]->m_Line.~vector();
		delete m_Intersections[i];
	}
	m_Intersections.~vector();
}

//创建图像,代码和构造函数重复
void CMyImage::CreateImage(TYPE Width, TYPE Height, int WidthSplits, int HeightSplits)
{
	m_dWidth           = Width;                                    //x方向
	m_dHeight          = Height;                                   //y方向
	m_nWidthSplits     = WidthSplits;                              //x方向
	m_nHeightSplits    = HeightSplits;                             //y方向
	m_dWidthSpace      = m_dWidth/m_nWidthSplits;                  //x方向
	m_dHeightSpace     = m_dHeight/m_nHeightSplits;                //y方向
	m_nQuantityOfPixel = m_nWidthSplits*m_nHeightSplits;           //数量
	m_PixelArray       = new CMyPixel[m_nQuantityOfPixel];         //分配空间
	for (int i = 0; i<m_nHeightSplits; i++)                        //像素块赋值
	{
		for (int j = 0; j<m_nWidthSplits; j++)
		{
			m_PixelArray[j + i*m_nWidthSplits].m_nRow = i;         //y坐标
			m_PixelArray[j + i*m_nWidthSplits].m_nCol = j;         //x坐标
			m_PixelArray[j + i*m_nWidthSplits].m_dLeftBottom = CMyPoint(j*m_dWidthSpace, i*m_dHeightSpace);
			m_PixelArray[j + i*m_nWidthSplits].m_dRightTop = CMyPoint((j+1)*m_dWidthSpace, (i+1)*m_dHeightSpace);
			m_PixelArray[j + i*m_nWidthSplits].m_dValue = 0;
		}
	}
}

//取得(Row, Col)的邻近像素,WhichSector指明方向，如果获取失败,WhichSector = DIRECTION_TERMINATE.
CMyPixel CMyImage::GetSector(int Row, int Col, int* WhichSector)
{
	switch (*WhichSector)
	{
	case DIRECTION_TOP:
		if (Row <= m_nHeightSplits -2)
		{//最上面一行像素没有Top
			return m_PixelArray[Col + (Row+1)*m_nWidthSplits];
		}
		break;
	case DIRECTION_BOTTOM:
		if (Row >= 1)
		{//最下面一行像素没有Bottom
			return m_PixelArray[Col + (Row-1)*m_nWidthSplits];
		}
		break;
	case DIRECTION_LEFT:
		if (Col >= 1)
		{//最左边一列没有Left
			return m_PixelArray[(Col-1) + Row*m_nWidthSplits];
		}
		break;
	case DIRECTION_RIGHT:
		if (Col <= m_nWidthSplits-2)
		{//最右边一列没有Right
			return m_PixelArray[(Col+1) + Row*m_nWidthSplits];
		}
		break;
	case DIRECTION_LEFT_TOP:
		if (Row <= m_nHeightSplits-2 && Col >= 1)
		{//最左边一列和最上面一行没有Left-Top
			return m_PixelArray[(Col-1) + (Row+1)*m_nWidthSplits];
		}
		break;
	case DIRECTION_LEFT_BOTTOM:
		if (Row >= 1 && Col >= 1)
		{//最左边一列和最下面一行没有Left-Bottom
			return m_PixelArray[(Col-1) + (Row-1)*m_nWidthSplits];
		}
		break;
	case DIRECTION_RIGHT_TOP:
		if (Row <= m_nHeightSplits-2 && Col <= m_nWidthSplits-2)
		{//最右边一列和最上面一行没有Right-Top
			return m_PixelArray[(Col+1) + (Row+1)*m_nWidthSplits];
		}
		break;
	case DIRECTION_RIGHT_BOTTOM:
		if (Row >= 1 && Col <= m_nWidthSplits-2)
		{//最右边一列和最下面一行没有Right-Bottom
				return m_PixelArray[(Col+1) + (Row-1)*m_nWidthSplits];
		}
		break;
	default:
		break;
	}
	*WhichSector = DIRECTION_TERMINATE;//其他情况导致迭代终止，返回像素本身.
	return m_PixelArray[Col + Row*m_nWidthSplits];
}

//已知Point是直线Ray和Pixel的一个交点，求解另一个.Direction表示交点的方位.
//沿着射线逐个求解交线长:Pixel-开始的像素， Ray-射线， Point-射线穿过像素的交点， Direction-射线从何处穿过
void CMyImage::Iterate(CMyPixel* Pixel, CMyRay* Ray, CMyPoint* Point, int* Direction)
{
	TYPE x, y;
	CMyPoint* TempPoint = new CMyPoint;
	if (Ray->GetSlope()>=0)
	{//斜率非负
		switch (*Direction)
		{
		case DIRECTION_TOP:
			x = Pixel->m_dLeftBottom.x;
			y = Ray->GetValueAxisY(x);
			if (y>Pixel->m_dLeftBottom.y)
			{//交左边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_LEFT;
			}
			else if (fabs(y - Pixel->m_dLeftBottom.y)<PRECISION)
			{//左下角
				*TempPoint = Pixel->m_dLeftBottom;
				*Direction = DIRECTION_LEFT_BOTTOM;
			} 
			else
			{//交底边
				TempPoint->y = Pixel->m_dLeftBottom.y;
				TempPoint->x = Ray->GetValueAxisX(TempPoint->y);
				*Direction = DIRECTION_BOTTOM;
			}
			break;
		case DIRECTION_BOTTOM:
			x = Pixel->m_dRightTop.x;
			y = Ray->GetValueAxisY(x);
			if (y>Pixel->m_dRightTop.y)
			{//交上边
				TempPoint->y = Pixel->m_dRightTop.y;
				TempPoint->x = Ray->GetValueAxisX(TempPoint->y);
				*Direction = DIRECTION_TOP;
			}
			else if (fabs(y - Pixel->m_dRightTop.y)<PRECISION)
			{//右上角
				*TempPoint = Pixel->m_dRightTop;
				*Direction = DIRECTION_RIGHT_TOP;
			} 
			else
			{//交右边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_RIGHT;
			}
			break;
		case DIRECTION_LEFT:
			y = Pixel->m_dRightTop.y;
			x = Ray->GetValueAxisX(y);
			if (x>Pixel->m_dRightTop.x)
			{//交右边
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_RIGHT;
			}
			else if (fabs(x - Pixel->m_dRightTop.x)<PRECISION)
			{//右上角
				*TempPoint = Pixel->m_dRightTop;
				*Direction = DIRECTION_RIGHT_TOP;
			} 
			else
			{//交顶边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_TOP;
			}
			break;
		case DIRECTION_RIGHT:
			y = Pixel->m_dLeftBottom.y;
			x = Ray->GetValueAxisX(y);
			if (x<Pixel->m_dLeftBottom.x)
			{//交左边
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_LEFT;
			}
			else if (fabs(x - Pixel->m_dLeftBottom.x)<PRECISION)
			{//左下角
				*TempPoint = Pixel->m_dLeftBottom;
				*Direction = DIRECTION_LEFT_BOTTOM;
			} 
			else
			{//交底边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_BOTTOM;
			}
			break;
		case DIRECTION_LEFT_TOP:/*这种情况不会有
			TempPoint->x = Pixel->m_dLeftBottom.x;
			TempPoint->y = Pixel->m_dRightTop.y;
			*Direction = DIRECTION_LEFT_TOP;*/
			break;
		case DIRECTION_LEFT_BOTTOM://和LEFT一样
			y = Pixel->m_dRightTop.y;
			x = Ray->GetValueAxisX(y);
			if (x>Pixel->m_dRightTop.x)
			{//交右边
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_RIGHT;
			}
			else if (fabs(x - Pixel->m_dRightTop.x)<PRECISION)
			{//右上角
				*TempPoint = Pixel->m_dRightTop;
				*Direction = DIRECTION_RIGHT_TOP;
			} 
			else
			{//交顶边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_TOP;
			}
			break;
		case DIRECTION_RIGHT_TOP://和RIGHT一样
			y = Pixel->m_dLeftBottom.y;
			x = Ray->GetValueAxisX(y);
			if (x<Pixel->m_dLeftBottom.x)
			{//交左边
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_LEFT;
			}
			else if (fabs(x - Pixel->m_dLeftBottom.x)<PRECISION)
			{//左下角
				*TempPoint = Pixel->m_dLeftBottom;
				*Direction = DIRECTION_LEFT_BOTTOM;
			} 
			else
			{//交底边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_BOTTOM;
			}
			break;
		case DIRECTION_RIGHT_BOTTOM:/*这种情况不会有
			TempPoint->x = Pixel->m_dRightTop.x;
			TempPoint->y = Pixel->m_dLeftBottom.y;
			*Direction = DIRECTION_RIGHT_BOTTOM;*/
			break;
		default:
			break;
		}
	}
	else//斜率小于0
	{
		switch (*Direction)
		{
		case DIRECTION_TOP:
			x = Pixel->m_dRightTop.x;
			y = Ray->GetValueAxisY(x);
			if (y<Pixel->m_dLeftBottom.y)
			{//交底边
				TempPoint->y = Pixel->m_dLeftBottom.y;
				TempPoint->x = Ray->GetValueAxisX(TempPoint->y);
				*Direction = DIRECTION_BOTTOM;
			}
			else if (fabs(y - Pixel->m_dLeftBottom.y)<PRECISION)
			{//右下角
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Pixel->m_dLeftBottom.y;
				*Direction = DIRECTION_RIGHT_BOTTOM;
			} 
			else
			{//交右边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_RIGHT;
			}
			break;
		case DIRECTION_BOTTOM:
			x = Pixel->m_dLeftBottom.x;
			y = Ray->GetValueAxisY(x);
			if (y>Pixel->m_dRightTop.y)
			{//交上边
				TempPoint->y = Pixel->m_dRightTop.y;
				TempPoint->x = Ray->GetValueAxisX(TempPoint->y);
				*Direction = DIRECTION_TOP;
			}
			else if (fabs(y - Pixel->m_dRightTop.y)<PRECISION)
			{//左上角
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Pixel->m_dRightTop.y;
				*Direction = DIRECTION_LEFT_TOP;
			} 
			else
			{//交左边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_LEFT;
			}
			break;
		case DIRECTION_LEFT:
			y = Pixel->m_dLeftBottom.y;
			x = Ray->GetValueAxisX(y);
			if (x>Pixel->m_dRightTop.x)
			{//交右边
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_RIGHT;
			}
			else if (fabs(x - Pixel->m_dRightTop.x)<PRECISION)
			{//右下角
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Pixel->m_dLeftBottom.y;
				*Direction = DIRECTION_RIGHT_BOTTOM;
			} 
			else
			{//交底边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_BOTTOM;
			}
			break;
		case DIRECTION_RIGHT:
			y = Pixel->m_dRightTop.y;
			x = Ray->GetValueAxisX(y);
			if (x<Pixel->m_dLeftBottom.x)
			{//交左边
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_LEFT;
			}
			else if (fabs(x - Pixel->m_dLeftBottom.x)<PRECISION)
			{//左上角
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Pixel->m_dRightTop.y;
				*Direction = DIRECTION_LEFT_TOP;
			} 
			else
			{//交顶边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_TOP;
			}
			break;
		case DIRECTION_LEFT_TOP://和LEFT一样
			y = Pixel->m_dLeftBottom.y;
			x = Ray->GetValueAxisX(y);
			if (x>Pixel->m_dRightTop.x)
			{//交右边
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_RIGHT;
			}
			else if (fabs(x - Pixel->m_dRightTop.x)<PRECISION)
			{//右下角
				TempPoint->x = Pixel->m_dRightTop.x;
				TempPoint->y = Pixel->m_dLeftBottom.y;
				*Direction = DIRECTION_RIGHT_BOTTOM;
			} 
			else
			{//交底边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_BOTTOM;
			}
			break;
		case DIRECTION_LEFT_BOTTOM:/*这种情况不会有
			*TempPoint = Pixel->m_dLeftBottom;
			*Direction = DIRECTION_LEFT_BOTTOM;*/
			break;
		case DIRECTION_RIGHT_TOP:/*这种情况不会有
			*TempPoint = Pixel->m_dRightTop;
			*Direction = DIRECTION_RIGHT_TOP;*/
			break;
		case DIRECTION_RIGHT_BOTTOM://和RIGHT一样
			y = Pixel->m_dRightTop.y;
			x = Ray->GetValueAxisX(y);
			if (x<Pixel->m_dLeftBottom.x)
			{//交左边
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Ray->GetValueAxisY(TempPoint->x);
				*Direction = DIRECTION_LEFT;
			}
			else if (fabs(x - Pixel->m_dLeftBottom.x)<PRECISION)
			{//左上角
				TempPoint->x = Pixel->m_dLeftBottom.x;
				TempPoint->y = Pixel->m_dRightTop.y;
				*Direction = DIRECTION_LEFT_TOP;
			} 
			else
			{//交顶边
				TempPoint->x = x;
				TempPoint->y = y;
				*Direction = DIRECTION_TOP;
			}
			break;
		default:
			break;
		}
	}

	//算出一条交线
	CMyLine* pNewLine = new CMyLine;
	pNewLine->m_nRow = Pixel->m_nRow;
	pNewLine->m_nCol = Pixel->m_nCol;
	pNewLine->m_dLength = Distance(*Point, *TempPoint);
	m_Intersections.back()->m_Line.push_back(pNewLine);

	//输出一些信息
    //cout<<"("<<pNewLine->m_nCol<<","<<pNewLine->m_nRow<<"): "<<pNewLine->m_dLength<<endl;
	//修改相关参数
	*Pixel = GetSector(Pixel->m_nRow, Pixel->m_nCol, Direction);
	*Point = *TempPoint;
	*Direction = -*Direction;
	delete TempPoint;
}

//沿着射线逐个求解交线长:Ray-射线
void CMyImage::Solve(CMyRay* Ray)
{
	CMyPixel *Pixel = new CMyPixel;
	CMyPoint *Point = new CMyPoint;
	int *Direction = new int;
	TYPE x = Ray->GetMeetX();
	TYPE y = Ray->GetMeetY();
	int Index;

	if (x<0)
	{//看射线与x轴相交的类型
		if (y>0 && y<m_dHeight)
		{//射线从图像左边射入
			Point->y = y;
			Index = int(y/m_dHeightSpace);
			*Pixel = m_PixelArray[Index*m_nWidthSplits];
			*Direction = DIRECTION_LEFT;
		}
		else
		{
			delete Pixel;
			delete Point;
			delete Direction;
			return;
		}
	}
	else if (x>m_dWidth)
	{//射线从图像右边射入
		TYPE Temp = Ray->GetValueAxisY(m_dWidth);
		if (Temp>0 && Temp<m_dHeight)
		{
			Point->x = m_dWidth;
			Point->y = Temp;
			Index = int(Temp/m_dHeightSpace);
			*Pixel = m_PixelArray[m_nWidthSplits-1 + Index*m_nWidthSplits];
			*Direction = DIRECTION_RIGHT;
		}
		else
		{
			delete Pixel;
			delete Point;
			delete Direction;
			return;
		}
	}
	else
	{//射线从图像底边射入
		Point->x = x;
		Index = int(x/m_dHeightSpace);
		*Pixel = m_PixelArray[Index];
		*Direction = DIRECTION_BOTTOM;
	}

	//新建一个LINES结构存放交线和对应的测量值
	LINES* pNewStruct = new LINES;
	m_Intersections.push_back(pNewStruct);
	pNewStruct->m_dMeasured = Ray->m_dMeasured;

	while (*Direction != DIRECTION_TERMINATE)
	{//DIRECTION_TERMINATE 用来控制迭代结束
		Iterate(Pixel, Ray, Point, Direction);
	}

	delete Pixel;
	delete Point;
	delete Direction;
}

//求解一系列射线与像素的交线
void CMyImage::SolveAll(CMyRay Lines[], long LinesQuantity)
{
	for (long i = 0; i<LinesQuantity; i++)
	{
		Solve(&Lines[i]);
	}
}

//ART迭代法：单线
void CMyImage::ARTSingleLine(int nCurrentLine)
{
	long i, Row, Col;
	LINES* pLineStruct;
	TYPE Delta, LengthSquare;
	pLineStruct = m_Intersections[nCurrentLine];
	Delta = 0;
	LengthSquare = 0;
	for (i = 0; i<pLineStruct->m_Line.size(); i++)
	{
		Row = pLineStruct->m_Line.at(i)->m_nRow;
		Col = pLineStruct->m_Line.at(i)->m_nCol;
		Delta += pLineStruct->m_Line.at(i)->m_dLength * m_PixelArray[Col+Row*m_nWidthSplits].m_dValue;
		LengthSquare += pLineStruct->m_Line.at(i)->m_dLength * pLineStruct->m_Line.at(i)->m_dLength;//此处有待改进
	}
	Delta = (pLineStruct->m_dMeasured - Delta);
	for (i = 0; i<pLineStruct->m_Line.size(); i++)
	{
		Row = pLineStruct->m_Line.at(i)->m_nRow;
		Col = pLineStruct->m_Line.at(i)->m_nCol;
		m_PixelArray[Col+Row*m_nWidthSplits].m_dValue += pLineStruct->m_Line.at(i)->m_dLength * Delta/LengthSquare;
	}
}

//ART迭代法：单步
void CMyImage::ARTSingleStep()
{
	for (long j = 0; j<m_Intersections.size(); j++)
	{
		ARTSingleLine(j);
	}
}

//ART迭代法：N步
void CMyImage::ARTSolution(int Steps)
{
	int k = 0;
	do 
	{
		ARTSingleStep();
		k++;
	} while (k<Steps);
	//SaveResult("D:\Output.txt");
}

//保存结果
void CMyImage::SaveResult(char* filepath)
{
	//写入文件
	ofstream OutputFile(filepath);//打开文件用于写，若文件不存在就创建它
	if(!OutputFile)
		return;                           //打开文件失败则结束运行
	for (int i = 0; i<m_nHeightSplits; i++)
	{
		for (int j = 0; j<m_nWidthSplits; j++)
		{                                 //使用插入运算符写文件内容
			OutputFile<<m_PixelArray[j + i*m_nWidthSplits].m_dValue<<",";
		}
		OutputFile<<endl;
	}
	OutputFile.close();
}

//找到像素的最大值
TYPE CMyImage::FindMax()
{
	TYPE max = -1;
	for (int i = 0; i<m_nWidthSplits; i++)
	{
		for (int j = 0; j<m_nHeightSplits; j++)
		{
			if (m_PixelArray[j + i*m_nWidthSplits].m_dValue>max)
			{
				max = m_PixelArray[j + i*m_nWidthSplits].m_dValue;
			}
		}
	}
	return max;
}