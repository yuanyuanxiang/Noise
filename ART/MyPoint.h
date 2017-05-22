#pragma once

//点类：一个点占用16

class CMyPoint
{
public:
	TYPE x;  //x坐标
	TYPE y;  //y坐标

public:
	CMyPoint();
	CMyPoint(TYPE a, TYPE b);
	~CMyPoint();
};