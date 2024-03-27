#include <iostream>
#define Max_length 105
using namespace std;
int* Distance(int prices[], int x);
int main()
{
	int days;
	int prices[Max_length];
	cout << "days:" << endl;//设定输入天数
	cin >> days;
	while (days <= 0 || days >= Max_length)
	{
		cout << "Error，enter it again." << endl;
		cout << "days:" << endl;
		cin >> days;
	} //代码的健壮性,考虑到 1 <= prices.length <= 105
	cout << "Please enter the stock price for each day:" << endl;
	int i = 0;
	int entered_price = 0;
	while (i < days)
	{
		cin >> entered_price;
		if (entered_price < 30 || entered_price > 100)
			cout << "Error，enter it again." << endl;
		else
		{
			prices[i] = entered_price;
			i++;
		}
	}//代码的健壮性,考虑到 30 <= prices[i] <= 100
	int* answer = Distance(prices, days);
	for (int j = 0; j < days; j++)
		cout << answer[j] << " ";//通过遍历输出答案
	return 0;
}
int* Distance(int prices[], int days)
{
	int* answer = new int[days];//动态定义一维数组answer[]
	for (int i = 0; i < days; i++)
		answer[i] = 0;//初始化条件
	for (int i = 0; i < days; i++)
	{
		for (int j = i + 1; j < days; j++)
		{
			if (prices[j] > prices[i])
			{
				answer[i] = j - i;
				break;
			}
		}
	}//运用嵌套循环来对数组内元素赋值
	return answer;
	delete[]answer;//释放一维数组answer[]
}
/*
 *测试数据:days:5
 *输入:40 50 30 70 60
 *输出 Max_profit:1 2 1 0 0
 */