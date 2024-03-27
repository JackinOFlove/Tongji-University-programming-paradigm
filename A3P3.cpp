#include <iostream>
#include <vector>//ʹ�ö�̬vector����
#include <math.h>
#include<unordered_map>//
#define MAX 1000000000
#define MIN -1000000000
#define Max_valueDiff 1000000000
#define Max_length 100000
#define Min_length 2
using namespace std;
//input����,���ڴ���Number�洢����
void input(vector<int>& Number)
{
	cout << "��������������,�Կո�Ϊ�ָ�,�����س�����:" << endl;
	int number;
	Newbegin://����Ľ�׳��,���ǵ�2 <= nums.length,Newbegin��Ϊ�������ݸ���С��2�ĺ�������
	for (int i = 0; i < Max_length; i++)
	{
		cin >> number;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(MAX, '\n');
			cout << "����Ĳ�������,����������:" << endl;//�ж���������������Ƿ���ȷ
			continue;
		}
		if (number<MIN || number>MAX)
		{
			cin.clear();
			cin.ignore(MAX, '\n');
			cout << "�������ֲ��ڷ�Χ��,����������:" << endl;//�ж���������ַ�Χ�Ƿ���ȷ
			continue;
		}
		Number.push_back(number);
		if (Number.size() > Max_length)
		{
			cin.clear();
			cin.ignore(MAX, '\n');
			cout << "������������" << endl;//�ж����ݵ������Ƿ���ȷ
			break;
		}
		if (cin.get() == '\n')//������س�����ʱ���ʾ���ݵ������Ѿ�����
			break;
	}
	if (Number.size() <  Min_length)
	{
		cin.clear();
		cout << "�������ݲ���2��,����� ���� ����:" << endl;//����Ľ�׳��,���ǵ�2 <= nums.length
		goto Newbegin;//�����������ݸ���С��2��,�����µ���Newbegin����������,ע�������ǲ������ֶ�������������
	}
}
//�������Ͱ����,get_bucket_ID����,���ڻ��ĳ��������Ͱ�ı��,���ǽ���ֵ��һ����Χ�ڵ����ַ���һ��Ͱ��,��һ��Ͱ�������1�໥������ĿҪ��
int get_bucket_ID(int Eachvalue, int valueDiff)
{
	if (Eachvalue >= 0)
		return Eachvalue / (valueDiff + 1);//��ĳ��������ֵ�����0ʱ
	else
		return (Eachvalue + 1) / (valueDiff + 1) - 1;//��ĳ��������ֵС��0ʱ
}
//
bool judgement(vector<int>&Number,int indexDiff,int valueDiff)
{
	unordered_map<int, int>bucket_map;//����һ��unordered_map,������intԪ�����,first��id,second��Eachvalue
	for (int i = 0; i < Number.size(); i++)
	{
		int Eachvalue = Number[i];
		int id = get_bucket_ID(Eachvalue, valueDiff);
		if (i - (indexDiff + 1) >= 0)
			bucket_map.erase(get_bucket_ID(Number[i - (indexDiff + 1)], valueDiff));//i - (indexDiff + 1) >= 0˵�����Ͱ�Ѿ��������ǵıȽϷ�Χ,�϶�����������������Ԫ��,���԰������,����Ӱ������ж�
		if (bucket_map.find(id) != bucket_map.end())
			return true;//�����Ͱ�����Ԫ��ʱ,˵����Ԫ�ش���,return true
		if (bucket_map.find(id - 1) != bucket_map.end() && abs(bucket_map[id - 1] - Eachvalue) <= valueDiff)
			return true;//�����Ͱ��ǰһ��Ͱ����Ԫ��ʱ,˵����Ԫ�ش���,return true
		if (bucket_map.find(id + 1) != bucket_map.end() && abs(bucket_map[id + 1] - Eachvalue) <= valueDiff)
			return true;//�����Ͱ�ĺ�һ��Ͱ����Ԫ��ʱ,˵����Ԫ�ش���,return true
		bucket_map[id] = Eachvalue;//�����Ԫ�ط������Ͱ��
	}
	return false;
}
//output����,����������յ��жϽ��
void output(vector<int>&Number, int valueDiff, int indexDiff)
{
	cout << boolalpha << judgement(Number, valueDiff, indexDiff) << endl;//��judgement(Number, valueDiff, indexDiff)ǰ���boolalpha,������ֵ1���Ϊtrue,������ֵ0���Ϊfalse
}
int main()
{
	vector<int>Number;
	input(Number);//����input������������
	int indexDiff, valueDiff;
	cout << "������indexDiff:" << endl;
	cin >> indexDiff;
	if (cin.good() == 0 || indexDiff < 1 || indexDiff > Number.size())//����Ľ�׳��,���ǵ�1 <= indexDiff <= nums.length
	{
		cin.clear();
		cin.ignore(Max_length, '\n');
		cout << "�������,����������:" << endl;
		cin >> indexDiff;
	}
	cout << "������valueDiff:" << endl;
	cin >> valueDiff;
	if (cin.good() == 0 || valueDiff < 0 || valueDiff > Max_valueDiff)//����Ľ�׳��,���ǵ�0 <= valueDiff <= 10^9
	{
		cin.clear();
		cin.ignore(Max_length, '\n');
		cout << "�������,����������:" << endl;
		cin >> valueDiff;
	}
	output(Number, indexDiff, valueDiff);//����output����������ս��
	return 0;
}