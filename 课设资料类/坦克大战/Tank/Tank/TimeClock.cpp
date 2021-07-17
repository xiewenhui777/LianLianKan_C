#include "stdafx.h"
#include "TimeClock.h"

TimeClock::TimeClock()
{
	QueryPerformanceFrequency(&litmp);	// ��ȡʱ��Ƶ��
	dfFreq = (double)litmp.QuadPart;

	QueryPerformanceCounter(&litmp);		// ��ȡʱ�Ӽ���(Ƶ����ʱ��ĳ˻�)
	QPart1 = litmp.QuadPart;
}

void TimeClock::SetDrtTime(double t)
{
	drtTime = t;
}

void TimeClock::Timer1()
{
	QueryPerformanceCounter(&litmp);		// ��ȡʱ�Ӽ���(Ƶ����ʱ��ĳ˻�)
	QPart1 = litmp.QuadPart;
}

void TimeClock::Init()
{
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;
	QPart2 = litmp.QuadPart;
}

bool TimeClock::IsTimeOut()
{
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;

	if (((double)(QPart2 - QPart1) * 1000) / dfFreq > drtTime)
	{
		QPart1 = QPart2;
		return true;
	}
	return false;
}
