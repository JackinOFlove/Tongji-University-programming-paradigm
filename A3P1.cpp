#include <iostream>
#include <vector>//ʹ�ö�̬vector����
#include <algorithm>//����<algorithm>��,������vector�е�ȡsort����
#define Max 1000000000
#define Min -1000000000
#define Max_length 100000
using namespace std;
//input����,���ڴ���Array�洢����
void input(vector<int>& Array)
{
	cout << "��������������,�Կո�Ϊ�ָ�,�����س�����:" << endl;
	int number;
	for (int i = 0; i < Max; i++)
	{
		cin >> number;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(Max, '\n');
			cout << "����Ĳ�������,����������:" << endl; //�ж���������������Ƿ���ȷ
			continue;
		}
		if (number<Min || number>Max)
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
		Array.push_back(number); //vector�е�push_back, ����Array������µ�Ԫ��, ���Ϸ����µ�����
		if (cin.get() == '\n')//������س�����ʱ���ʾ���ݵ������Ѿ�����
			break;
	}
}
//judgement����,�����ж�Array���Ƿ����ظ���Ԫ��
bool judgement(vector<int>& Array)
{
	sort(Array.begin(), Array.end());//��������ķ�����Array�е�Ԫ�ؽ�������
	for (unsigned int i = 1; i < Array.size(); i++)
	{
		if (Array[i] == Array[i - 1])//����֮��,��Array�е�Ԫ�ش�С������ǰһ��Ԫ�صĴ�С,˵��Ԫ�����ظ�,����true������break
			return true;
	}
	return false;//��������,���û���ظ���Ԫ��,����false
}
//output����,����������յ��жϽ��
void output(vector<int>& Array)
{
	cout << boolalpha << judgement(Array) << endl;//��judgement(Array)ǰ���boolalpha,������ֵ1���Ϊtrue,������ֵ0���Ϊfalse
}
int main()
{
	vector<int> Array;
	input(Array);//����input������������
	output(Array);//����output����������ս��
	return 0;
}
/*�������ݣ�
* ����:1 2 3 1
* ���:true
* ����:1 2 3 4
* ���:false
* ����:1 2 3 4 5 4 3 2 1
* ���:true
*/