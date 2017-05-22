#ifndef SPACE_FILTER_H
#define SPACE_FILTER_H

//空间滤波器类

class CSpaceFilter
{
public:
	CSpaceFilter();
	~CSpaceFilter();

protected://新增成员
	int                                     m_nSize;          //滤波器S的大小
	int                                      m_nRow;          //滤波器中心y坐标
	int                                      m_nCol;          //滤波器中心x坐标
	int                                   m_nLength;          //滤波器长度
	int                               m_nSizeIsEven;          //奇数-0，偶数-1
	int*                                   m_Sector;          //滤波器

public://新增函数
	BOOL Create(BYTE* dataSrc, BITMAP bmpInfo, int curChannel, int row, int col, int size);
	                                                          //构造一个滤波器
	int FindMax();                                            //S中的最大灰度值
	int FindMin();                                            //S中的最小灰度值
	int FindMed();                                            //S中的灰度值中值
	int Visit(int x, int y);                                  //访问x列,y行处值

private:

};

#endif