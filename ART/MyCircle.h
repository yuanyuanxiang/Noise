#pragma once

//圆类，用来生成模拟数据

class CMyCircle
{
public:
	CMyPoint                      m_Center;  //圆心
	TYPE                          m_Radius;  //半径
	TYPE                          m_dDecay;  //衰减系数

public:
	CMyCircle();
	CMyCircle(TYPE Radius);
	CMyCircle(CMyPoint Center, TYPE Radius);
	CMyCircle(CMyPoint Center, TYPE Radius, TYPE Decay);
	~CMyCircle();

	void Create(CMyPoint Center, TYPE Radius, TYPE Decay);
};