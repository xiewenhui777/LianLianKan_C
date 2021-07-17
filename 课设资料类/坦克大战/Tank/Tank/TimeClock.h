#pragma once


/*
* Timer1()  ��ʼ����һ��
* Timer2() ѭ������
* IsTimeOut() ѭ�����ü��ʱ����Ƿ�������õ�ֵ
*/

class TimeClock
{
public:
	TimeClock();
	~TimeClock() {}

	/*����ʱ���, �����ٶ�*/
	void SetDrtTime(double);

	/*���� QPart1*/
	void Timer1();

	void Init();		// ���� t1 = t2

	/*�Ƚ�������ʱʱ���, ���� true false ������ʱ���*/
	bool IsTimeOut();		// ʱ��


private:
	LARGE_INTEGER litmp;		// ����ýṹ���һ������
	LONGLONG QPart1, QPart2;	// ���ڻ�ȡ�ṹ������� QuadPart ��Ա
	double dfFreq;				// ʱ��Ƶ��
	double drtTime;				// ����ʱ���, ���������˶��ٶ�
};