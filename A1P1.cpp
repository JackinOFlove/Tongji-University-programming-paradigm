#include <iostream>
#define Max_length 105
using namespace std;
int* Distance(int prices[], int x);
int main()
{
	int days;
	int prices[Max_length];
	cout << "days:" << endl;//�趨��������
	cin >> days;
	while (days <= 0 || days >= Max_length)
	{
		cout << "Error��enter it again." << endl;
		cout << "days:" << endl;
		cin >> days;
	} //����Ľ�׳��,���ǵ� 1 <= prices.length <= 105
	cout << "Please enter the stock price for each day:" << endl;
	int i = 0;
	int entered_price = 0;
	while (i < days)
	{
		cin >> entered_price;
		if (entered_price < 30 || entered_price > 100)
			cout << "Error��enter it again." << endl;
		else
		{
			prices[i] = entered_price;
			i++;
		}
	}//����Ľ�׳��,���ǵ� 30 <= prices[i] <= 100
	int* answer = Distance(prices, days);
	for (int j = 0; j < days; j++)
		cout << answer[j] << " ";//ͨ�����������
	return 0;
}
int* Distance(int prices[], int days)
{
	int* answer = new int[days];//��̬����һά����answer[]
	for (int i = 0; i < days; i++)
		answer[i] = 0;//��ʼ������
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
	}//����Ƕ��ѭ������������Ԫ�ظ�ֵ
	return answer;
	delete[]answer;//�ͷ�һά����answer[]
}
/*
 *��������:days:5
 *����:40 50 30 70 60
 *��� Max_profit:1 2 1 0 0
 */