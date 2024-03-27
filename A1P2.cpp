#include <iostream>
#define Max_length 105
using namespace std;
int profit(int prices[], int days);
int main()
{
	int days;
	int prices[Max_length];
	cout << "days:" << endl;
	cin >> days;
	while (days <= 0 || days >= Max_length)
	{
		cout << "Error��enter it again." << endl;
		cout << "days:" << endl;
		cin >> days;
	}//����Ľ�׳��,���ǵ� 1 <= prices.length <= 105
	cout << "Please enter the stock price for each day:" << endl;
	int i = 0;
	int entered_price = 0;
	while (i < days)
	{
		cin >> entered_price;
		if (entered_price < 0 || entered_price > 104)
			cout << "Error��enter it again." << endl;
		else
		{
			prices[i] = entered_price;
			i++;
		}
	}//����Ľ�׳��,���ǵ� 0 <= prices[i] <= 104
	int Max_profit = profit(prices, days);
	cout << "Max_profit:"<< Max_profit << endl;//������
	return 0;
}
int profit(int prices[], int days)
{
	int Max_profit = 0;
	for (int i = 0; i < days; i++)
	{
		for (int j = i + 1; j < days; j++)
		{
			if (prices[j] > prices[i] && prices[j] - prices[i] >= Max_profit)
				Max_profit = prices[j] - prices[i];
		}
	}//ͨ��Ƕ��ѭ���ҳ�Max_profit
	return Max_profit;
}
/*
 *��������:days:6
 *����:7 1 5 3 6 4
 *��� Max_profit:5
 */