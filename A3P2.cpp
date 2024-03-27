#include <iostream>
#include <algorithm>//����<algorithm>��,������vector�е�ȡsort����
#include<math.h>
#include <vector>//ʹ�ö�̬vector����
#define Max 1000000000
#define Min -1000000000
#define Max_length 100000
#define Maxk 100000
using namespace std;
//input����,���ڴ���Array�洢����
struct Node
{
	int value;
	int number;
};//����Node���͵Ľṹ��,����������������Array����ԭ����λ�ú�������ֵ�ֵ
bool cmp_max(Node x, Node y)
{
	return x.value < y.value;
}//������,����ķ�����Array�е�ÿһ�����ֽ�������
//input����,����һ��vector����,��ÿһ��Ԫ�ؽ��и�ֵ(ÿһ��Ԫ�ض��ǽṹ��,����������,һ������ԭ��������±�,һ������Ҫ���������ֵ)
void input(vector<Node>&Array)
{
	Node Eachnumber;
	cout << "��������������,�Կո�Ϊ�ָ�,�����س�����:" << endl;
	for (int i = 0; i < Max; i++)
	{
		Eachnumber.number = i;
		cin >> Eachnumber.value;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(Max, '\n');
			cout << "����Ĳ�������,����������:" << endl;//�ж���������������Ƿ���ȷ
			continue;
		}
		if (Eachnumber.value<Min || Eachnumber.value>Max)
		{
			cin.clear();
			cin.ignore(Max, '\n');
			cout << "�������ֲ��ڷ�Χ��,����������:" << endl;//�ж���������ַ�Χ�Ƿ���ȷ
			continue;
		}
		if (Array.size() > Max_length)
		{
			cin.clear();
			cin.ignore(Max, '\n');
			cout << "������������" << endl;//�ж����ݵ������Ƿ���ȷ
			break;
		}
		//����Ľ�׳��,���ǵ�-10^9 <= nums[i] <= 10^9��1 <= nums.length <= 10^5
		Array.push_back(Eachnumber);//vector�е�push_back, ����Array������µ�Ԫ��, ���Ϸ����µĽṹ��Eachnumber
		if (cin.get() == '\n') // ������س�����ʱ���ʾ���ݵ������Ѿ�����
			break;
	}
}
//judgement����,�ж��Ƿ��з�������������
bool judgement(vector<Node>&Array, int k)
{
	if (k >= Array.size())
		return false;//�������k�����������ж�,���k���������ĳ���,�򲻿��ܻ��������������,ֱ�ӷ���false
	int Distance=Array.size();//���Ƚ�Distance��ʼ��ΪArray����ĳ���,��������ļ���
	sort(Array.begin(), Array.end(), cmp_max);//����,��������ķ�����Array�е�ÿһ�����ֽ�������,��Ȼ���ǵ�˳�򱻴�����,����������number��һ��������������ԭ����λ��
	for (unsigned int i = 0; i < Array.size()-1; i++)
	{
		if (Array[i].value == Array[i + 1].value)//��Ϊ�������,�����������ͬ����,���ǿ϶�������һ���,���Ա����������������ͬ����,���������в���
			Distance = min(abs(Array[i].number - Array[i + 1].number), Distance);//����,���Distance����Сֵ,��������С��ͬ��������ԭ�������±�֮�����Сֵ
	}
	if (Distance <= k)
		return true;//���������С��ͬ��������ԭ�������±�֮�����СֵС�ڵ���k,˵��������������������,����true
	else
		return false; //���������С��ͬ��������ԭ�������±�֮�����Сֵ����k, ˵��û������������������, ����false
}
//output����,����������յ��жϽ��
void output(vector<Node>&Array,int k)
{
	cout << boolalpha << judgement(Array,k) << endl;//��judgement(Array,k)ǰ���boolalpha,������ֵ1���Ϊtrue,������ֵ0���Ϊfalse
}
int main()
{
	int k;
	vector<Node>Array;
	input(Array);//����input������������
	cout << "������k:" << endl;
	cin >> k;
	if (cin.good() == 0 || k < 0||k > Maxk)
	{
		cin.clear();
		cin.ignore(Max, '\n');
		cout << "�������,����������:" << endl;//�ж�����k���������ͺ����ַ�Χ�Ƿ���ȷ
		cin >> k;
	}
	output(Array, k);//����output����������ս��
}
/*�������ݣ�
* ����:����:1 2 3 1,k=3
* ���:true
* ����:����:1 0 1 1,k=1
* ���:true
* ����:����:1 2 3 4 5 4 3 2 1,k=1
* ���:false
* ����:����:1 2 3 4 5,k=5
* ���:false
*/