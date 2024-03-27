#include <iostream>
#include <vector>//使用动态vector容器
#include <algorithm>//引用<algorithm>库,方便在vector中调取sort函数
#define Max 1000000000
#define Min -1000000000
#define Max_length 100000
using namespace std;
//input函数,用于创建Array存储数据
void input(vector<int>& Array)
{
	cout << "请依次输入数据,以空格为分隔,遇到回车结束:" << endl;
	int number;
	for (int i = 0; i < Max; i++)
	{
		cin >> number;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(Max, '\n');
			cout << "输入的不是数字,请重新输入:" << endl; //判断输入的数据类型是否正确
			continue;
		}
		if (number<Min || number>Max)
		{
			cin.clear();
			cin.ignore(Max, '\n');
			cout << "输入数字不在范围内,请重新输入:" << endl;//判断输入的数字范围是否正确
			continue;
		}
		if (Array.size() > Max_length)
		{
			cin.clear();
			cin.ignore(Max, '\n');
			cout << "输入数据已满" << endl;//判断数据的总数是否正确
			break;
		}
		//代码的健壮性,考虑到-10^9 <= nums[i] <= 10^9和1 <= nums.length <= 10^5
		Array.push_back(number); //vector中的push_back, 即在Array后插入新的元素, 即合法的新的数字
		if (cin.get() == '\n')//当输入回车键的时候表示数据的输入已经结束
			break;
	}
}
//judgement函数,用于判断Array中是否有重复的元素
bool judgement(vector<int>& Array)
{
	sort(Array.begin(), Array.end());//采用升序的方法将Array中的元素进行排序
	for (unsigned int i = 1; i < Array.size(); i++)
	{
		if (Array[i] == Array[i - 1])//排序之后,当Array中的元素大小等于它前一个元素的大小,说明元素有重复,返回true，并且break
			return true;
	}
	return false;//遍历过后,如果没有重复的元素,返回false
}
//output函数,用于输出最终的判断结果
void output(vector<int>& Array)
{
	cout << boolalpha << judgement(Array) << endl;//在judgement(Array)前面加boolalpha,将返回值1输出为true,将返回值0输出为false
}
int main()
{
	vector<int> Array;
	input(Array);//调用input函数输入数据
	output(Array);//调用output函数输出最终结果
	return 0;
}
/*测试数据：
* 输入:1 2 3 1
* 输出:true
* 输入:1 2 3 4
* 输出:false
* 输入:1 2 3 4 5 4 3 2 1
* 输出:true
*/