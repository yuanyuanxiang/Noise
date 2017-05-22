#include "stdafx.h"
#include "MyRay.h"

CMyRay::CMyRay()
{
	m_dNormalAngle = 0;
	m_dDistance = 0;
	m_dMeasured = 0;
}

CMyRay::CMyRay(TYPE NormalAngle, TYPE Distance)
{
	m_dNormalAngle = NormalAngle;
	m_dDistance = Distance;
	m_dMeasured = 0;
}

CMyRay::CMyRay(TYPE NormalAngle, TYPE Distance, TYPE Measured)
{
	m_dNormalAngle = NormalAngle;
	m_dDistance = Distance;
	m_dMeasured = Measured;
}

CMyRay::~CMyRay()
{

}

TYPE CMyRay::GetSlope()
{
	if (m_dNormalAngle != -PI/2)
	{
		return TYPE (tan(m_dNormalAngle+PI/2));
	}
	else
	{
		return TYPE (THIS_REAL_MAX);
	}
}

TYPE CMyRay::GetMeetX()
{
	//PI/2永远取不到,所以不考虑除0情况.
	return m_dDistance/cos(m_dNormalAngle);
}

TYPE CMyRay::GetMeetY()
{
	if (m_dNormalAngle != 0)
	{
		return m_dDistance/sin(m_dNormalAngle);//0不能作为除数
	}
	else 
	{
		return TYPE (THIS_REAL_MAX);
	}
}

//求解x对应的y值，y=kx+C
TYPE CMyRay::GetValueAxisY(TYPE x)
{
	return GetSlope()*x + GetMeetY();
}

//求解y对应的x值，x=(y-C)/k
TYPE CMyRay::GetValueAxisX(TYPE y)
{
	return (y-GetMeetY())/GetSlope();
}