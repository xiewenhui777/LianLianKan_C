#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int num[4], cnt = 0, n;
    srand(clock());  // 设置随机数种子
    while (cnt < 4)
    {
        n = rand() % 13; // 生成4以内随机数，这样更利于测试
        for (int i = 0; i < cnt; i++)
            if (num[i] == n) // 遍历数组，有相同的重新生成随机数
                continue;
        num[cnt++] = n;
    }
    for (int i = 0; i < cnt; i++) // 打印随机数数组
        printf("%d ", num[i]);
    return 0;
}
