#include <iostream>
#include <vector>//使用动态vector容器
#include <math.h>
#include<unordered_map>//
#define MAX 1000000000
#define MIN -1000000000
#define Max_valueDiff 1000000000
#define Max_length 100000
#define Min_length 2
using namespace std;
//input函数,用于创建Number存储数据
void input(vector<int>& Number)
{
	cout << "请依次输入数据,以空格为分隔,遇到回车结束:" << endl;
	int number;
	Newbegin://代码的健壮性,考虑到2 <= nums.length,Newbegin作为输入数据个数小于2的后续操作
	for (int i = 0; i < Max_length; i++)
	{
		cin >> number;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(MAX, '\n');
			cout << "输入的不是数字,请重新输入:" << endl;//判断输入的数据类型是否正确
			continue;
		}
		if (number<MIN || number>MAX)
		{
			cin.clear();
			cin.ignore(MAX, '\n');
			cout << "输入数字不在范围内,请重新输入:" << endl;//判断输入的数字范围是否正确
			continue;
		}
		Number.push_back(number);
		if (Number.size() > Max_length)
		{
			cin.clear();
			cin.ignore(MAX, '\n');
			cout << "输入数据已满" << endl;//判断数据的总数是否正确
			break;
		}
		if (cin.get() == '\n')//当输入回车键的时候表示数据的输入已经结束
			break;
	}
	if (Number.size() <  Min_length)
	{
		cin.clear();
		cout << "输入数据不足2个,请继续 补充 数字:" << endl;//代码的健壮性,考虑到2 <= nums.length
		goto Newbegin;//如果输入的数据个数小于2个,则重新到达Newbegin处输入数据,注意这里是补充数字而不是重新输入
	}
}
//本题采用桶排序,get_bucket_ID函数,用于获得某个数所在桶的编号,我们将数值在一定范围内的数字放在一个桶里,这一个桶里的数字1相互满足题目要求
int get_bucket_ID(int Eachvalue, int valueDiff)
{
	if (Eachvalue >= 0)
		return Eachvalue / (valueDiff + 1);//当某个数的数值大等于0时
	else
		return (Eachvalue + 1) / (valueDiff + 1) - 1;//当某个数的数值小于0时
}
//
bool judgement(vector<int>&Number,int indexDiff,int valueDiff)
{
	unordered_map<int, int>bucket_map;//定义一个unordered_map,由两个int元素组成,first是id,second是Eachvalue
	for (int i = 0; i < Number.size(); i++)
	{
		int Eachvalue = Number[i];
		int id = get_bucket_ID(Eachvalue, valueDiff);
		if (i - (indexDiff + 1) >= 0)
			bucket_map.erase(get_bucket_ID(Number[i - (indexDiff + 1)], valueDiff));//i - (indexDiff + 1) >= 0说明这个桶已经超出我们的比较范围,肯定不存在满足条件的元素,所以把它清除,以免影响后续判断
		if (bucket_map.find(id) != bucket_map.end())
			return true;//当这个桶里就有元素时,说明有元素存在,return true
		if (bucket_map.find(id - 1) != bucket_map.end() && abs(bucket_map[id - 1] - Eachvalue) <= valueDiff)
			return true;//当这个桶的前一个桶里有元素时,说明有元素存在,return true
		if (bucket_map.find(id + 1) != bucket_map.end() && abs(bucket_map[id + 1] - Eachvalue) <= valueDiff)
			return true;//当这个桶的后一个桶里有元素时,说明有元素存在,return true
		bucket_map[id] = Eachvalue;//将这个元素放在这个桶里
	}
	return false;
}
//output函数,用于输出最终的判断结果
void output(vector<int>&Number, int valueDiff, int indexDiff)
{
	cout << boolalpha << judgement(Number, valueDiff, indexDiff) << endl;//在judgement(Number, valueDiff, indexDiff)前面加boolalpha,将返回值1输出为true,将返回值0输出为false
}
int main()
{
	vector<int>Number;
	input(Number);//调用input函数输入数据
	int indexDiff, valueDiff;
	cout << "请输入indexDiff:" << endl;
	cin >> indexDiff;
	if (cin.good() == 0 || indexDiff < 1 || indexDiff > Number.size())//代码的健壮性,考虑到1 <= indexDiff <= nums.length
	{
		cin.clear();
		cin.ignore(Max_length, '\n');
		cout << "输入错误,请重新输入:" << endl;
		cin >> indexDiff;
	}
	cout << "请输入valueDiff:" << endl;
	cin >> valueDiff;
	if (cin.good() == 0 || valueDiff < 0 || valueDiff > Max_valueDiff)//代码的健壮性,考虑到0 <= valueDiff <= 10^9
	{
		cin.clear();
		cin.ignore(Max_length, '\n');
		cout << "输入错误,请重新输入:" << endl;
		cin >> valueDiff;
	}
	output(Number, indexDiff, valueDiff);//调用output函数输出最终结果
	return 0;
}