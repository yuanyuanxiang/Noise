#include "stdafx.h"
#include "MyPoint.h"
#include "MyCircle.h"

//构造一个默认的单位圆
CMyCircle::CMyCircle()
{
	m_Center = CMyPoint(0, 0);
	m_Radius = 1;
	m_dDecay = 0;
}

//根据半径构造一个圆
CMyCircle::CMyCircle(TYPE Radius)
{
	m_Center = CMyPoint(0, 0);
	m_Radius = Radius;
	m_dDecay = 0;
}

CMyCircle::CMyCircle(CMyPoint Center, TYPE Radius)
{
	m_Center = Center;
	m_Radius = Radius;
	m_dDecay = 0;
}

CMyCircle::CMyCircle(CMyPoint Center, TYPE Radius, TYPE Decay)
{
	m_Center = Center;
	m_Radius = Radius;
	m_dDecay = Decay;
}

CMyCircle::~CMyCircle()
{

}

void CMyCircle::Create(CMyPoint Center, TYPE Radius, TYPE Decay)
{
	m_Center = Center;
	m_Radius = Radius;
	m_dDecay = Decay;
}