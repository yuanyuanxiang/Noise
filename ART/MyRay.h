#pragma once

//射线类：一条射线占用24

class CMyRay
{
public:
	TYPE                             m_dNormalAngle;  //法向夹角
	TYPE                                m_dDistance;  //到原点距离
	TYPE                                m_dMeasured;  //实际测量值

public:
	CMyRay();
	CMyRay(TYPE NormalAngle, TYPE Distance);
	CMyRay(TYPE NormalAngle, TYPE Distance, TYPE Measured);
	~CMyRay();
	TYPE GetSlope();               //求解斜率
	TYPE GetMeetX();               //求解与x轴交点
	TYPE GetMeetY();               //求解与y轴交点
	TYPE GetValueAxisY(TYPE x);    //求解x对应的y值
	TYPE GetValueAxisX(TYPE y);    //求解y对应的x值
};