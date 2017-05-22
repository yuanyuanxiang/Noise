#pragma once

#include "MyDefs.h"
#include "MyPoint.h"
#include "MyRay.h"
#include "MyCircle.h"

//
//ÉùÃ÷
//
TYPE Distance(CMyPoint& PointOne, CMyPoint& PointTwo);
TYPE random(TYPE start, TYPE end);
void Measured(CMyRay *Ray, CMyCircle Big, CMyCircle Small);
CMyPoint* InterSect(CMyRay& Ray, CMyCircle Circle);
CMyRay* Demo(TYPE AngleBegin, TYPE AngleEnd, int AngleQuantity, TYPE DistanceBegin, TYPE DistanceEnd, int DistanceQuantity, CMyCircle& Big, CMyCircle& Small);
void SaveDemoData(CMyRay* pRayArray, int AngleQuantity, int DistanceQuantity, char* filepath);