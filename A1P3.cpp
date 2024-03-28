#include <iostream>
#include <cmath>
#define Max_length 5001
using namespace std;
int Algorithm(int prices[], int x);
int main()
{
	int days;
	int prices[Max_length];
	cout << "days:" << endl;
	cin >> days;
	while (days <= 0 || days > Max_length)
	{
		cout << "Error，enter it again." << endl;
		cout << "days:" << endl;
		cin >> days;
	}//代码的健壮性,考虑到 1 <= prices.length <= 5000
	cout << "Please enter the stock price for each day:" << endl;
	int i = 0;
	int entered_price = 0;
	while (i < days)
	{
		cin >> entered_price;
		if (entered_price < 0 || entered_price > 1000)
			cout << "Error，enter it again." << endl;
		else
		{
			prices[i] = entered_price;
			i++;
		}
	}//代码的健壮性,考虑到 0 <= prices[i] <= 1000
	int Max_profit = Algorithm(prices, days);
	cout << "Max_profit:" << Max_profit << endl;//输出结果
	return 0;
}
//本题采用一维动态规划
int Algorithm(int prices[], int days)
{
	int profit[Max_length];//profit[i]表示第i天卖出股票后,可以持有的最大钱数
	int Max_profit = 0;
	for (int i = 0; i < days; i++)
		profit[i] = 0;
	profit[0] = 0;
	profit[1] = max(prices[1] - prices[0], 0);//初始化profit[i]
	for (int i = 2; i < days; i++)
	{
		profit[i] = max(profit[i], prices[i] - min(prices[0], prices[1]));
		//外层循环, 由于下文中j是从2开始遍历的,这里先将profit[i]定义为(profit[i]和第i天股票价减去第0天和第1天股票价低的一个)中的最大值
		for (int j = 2; j < i; j++)
			profit[i] = max(profit[i], profit[j - 2] + (prices[i] - prices[j]));
		//内层循环,由状态转移方程 profit[i] = max(profit[i], profit[j - 2] + (prices[i] - prices[j]))，即从第j天买入,第i天卖出时,利润的最大值
	}
	for (int k = 0; k < days; k++)
		Max_profit = max(Max_profit, profit[k]);//通过遍历,找到 Max_profit
	return Max_profit;
}
/*
 *测试数据:days:5
 *输入:1 2 3 0 2
 *输出 Max_profit:3
 *测试数据:days:5
 *输入:1 2 3 4 5
 *输出 Max_profit:4
 *测试数据:days:1 
 *输入:30 
 *输出 Max_profit:0 
 */
