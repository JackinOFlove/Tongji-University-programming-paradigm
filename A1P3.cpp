#include <iostream>
#include "math.h"//����math.h�ļ�,�����õ�max��min����
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
		cout << "Error��enter it again." << endl;
		cout << "days:" << endl;
		cin >> days;
	}//����Ľ�׳��,���ǵ� 1 <= prices.length <= 5000
	cout << "Please enter the stock price for each day:" << endl;
	int i = 0;
	int entered_price = 0;
	while (i < days)
	{
		cin >> entered_price;
		if (entered_price < 0 || entered_price > 1000)
			cout << "Error��enter it again." << endl;
		else
		{
			prices[i] = entered_price;
			i++;
		}
	}//����Ľ�׳��,���ǵ� 0 <= prices[i] <= 1000
	int Max_profit = Algorithm(prices, days);
	cout << "Max_profit:" << Max_profit << endl;//������
	return 0;
}
//�������һά��̬�滮
int Algorithm(int prices[], int days)
{
	int profit[Max_length];//profit[i]��ʾ��i��������Ʊ��,���Գ��е����Ǯ��
	int Max_profit = 0;
	for (int i = 0; i < days; i++)
		profit[i] = 0;
	profit[0] = 0;
	profit[1] = max(prices[1] - prices[0], 0);//��ʼ��profit[i]
	for (int i = 2; i < days; i++)
	{
		profit[i] = max(profit[i], prices[i] - min(prices[0], prices[1]));
		//���ѭ��, ����������j�Ǵ�2��ʼ������,�����Ƚ�profit[i]����Ϊ(profit[i]�͵�i���Ʊ�ۼ�ȥ��0��͵�1���Ʊ�۵͵�һ��)�е����ֵ
		for (int j = 2; j < i; j++)
			profit[i] = max(profit[i], profit[j - 2] + (prices[i] - prices[j]));
		//�ڲ�ѭ��,��״̬ת�Ʒ��� profit[i] = max(profit[i], profit[j - 2] + (prices[i] - prices[j]))�����ӵ�j������,��i������ʱ,��������ֵ
	}
	for (int k = 0; k < days; k++)
		Max_profit = max(Max_profit, profit[k]);//ͨ������,�ҵ� Max_profit
	return Max_profit;
}
/*
 *��������:days:5
 *����:1 2 3 0 2
 *��� Max_profit:3
 *��������:days:5
 *����:1 2 3 4 5
 *��� Max_profit:4
 *��������:days:1 
 *����:30 
 *��� Max_profit:0 
 */