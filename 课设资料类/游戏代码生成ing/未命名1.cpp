#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int num[4], cnt = 0, n;
    srand(clock());  // �������������
    while (cnt < 4)
    {
        n = rand() % 13; // ����4��������������������ڲ���
        for (int i = 0; i < cnt; i++)
            if (num[i] == n) // �������飬����ͬ���������������
                continue;
        num[cnt++] = n;
    }
    for (int i = 0; i < cnt; i++) // ��ӡ���������
        printf("%d ", num[i]);
    return 0;
}
