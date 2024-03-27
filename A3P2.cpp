#include <iostream>
#include <algorithm>//引用<algorithm>库,方便在vector中调取sort函数
#include<math.h>
#include <vector>//使用动态vector容器
#define Max 1000000000
#define Min -1000000000
#define Max_length 100000
#define Maxk 100000
using namespace std;
//input函数,用于创建Array存储数据
struct Node
{
	int value;
	int number;
};//定义Node类型的结构体,它存放着这个数字在Array数组原来的位置和这个数字的值
bool cmp_max(Node x, Node y)
{
	return x.value < y.value;
}//排序函数,升序的方法对Array中的每一个数字进行排序
//input函数,传入一个vector数组,对每一个元素进行赋值(每一个元素都是结构体,有两个变量,一个是在原来数组的下标,一个是需要我们输入的值)
void input(vector<Node>&Array)
{
	Node Eachnumber;
	cout << "请依次输入数据,以空格为分隔,遇到回车结束:" << endl;
	for (int i = 0; i < Max; i++)
	{
		Eachnumber.number = i;
		cin >> Eachnumber.value;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(Max, '\n');
			cout << "输入的不是数字,请重新输入:" << endl;//判断输入的数据类型是否正确
			continue;
		}
		if (Eachnumber.value<Min || Eachnumber.value>Max)
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
		Array.push_back(Eachnumber);//vector中的push_back, 即在Array后插入新的元素, 即合法的新的结构体Eachnumber
		if (cin.get() == '\n') // 当输入回车键的时候表示数据的输入已经结束
			break;
	}
}
//judgement函数,判断是否有符合条件的数字
bool judgement(vector<Node>&Array, int k)
{
	if (k >= Array.size())
		return false;//对输入的k进行特殊性判断,如果k大等于数组的长度,则不可能会有数据满足情况,直接返回false
	int Distance=Array.size();//首先将Distance初始化为Array数组的长度,参与后续的计算
	sort(Array.begin(), Array.end(), cmp_max);//排序,利用升序的方法对Array中的每一个数字进行排序,虽然他们的顺序被打乱了,都是我们有number这一变量来储存它们原来的位置
	for (unsigned int i = 0; i < Array.size()-1; i++)
	{
		if (Array[i].value == Array[i + 1].value)//因为排序过后,如果有两个相同的数,它们肯定是连在一起的,所以遍历是如果有两个相同的数,则会进行下列操作
			Distance = min(abs(Array[i].number - Array[i + 1].number), Distance);//迭代,求出Distance的最小值,即两个大小相同的数字在原来数组下标之差的最小值
	}
	if (Distance <= k)
		return true;//如果两个大小相同的数字在原来数组下标之差的最小值小于等于k,说明有满足题意的情况出现,返回true
	else
		return false; //如果两个大小相同的数字在原来数组下标之差的最小值大于k, 说明没有满足题意的情况出现, 返回false
}
//output函数,用于输出最终的判断结果
void output(vector<Node>&Array,int k)
{
	cout << boolalpha << judgement(Array,k) << endl;//在judgement(Array,k)前面加boolalpha,将返回值1输出为true,将返回值0输出为false
}
int main()
{
	int k;
	vector<Node>Array;
	input(Array);//调用input函数输入数据
	cout << "请输入k:" << endl;
	cin >> k;
	if (cin.good() == 0 || k < 0||k > Maxk)
	{
		cin.clear();
		cin.ignore(Max, '\n');
		cout << "输入错误,请重新输入:" << endl;//判断输入k的数据类型和数字范围是否正确
		cin >> k;
	}
	output(Array, k);//调用output函数输出最终结果
}
/*测试数据：
* 输入:数据:1 2 3 1,k=3
* 输出:true
* 输入:数据:1 0 1 1,k=1
* 输出:true
* 输入:数据:1 2 3 4 5 4 3 2 1,k=1
* 输出:false
* 输入:数据:1 2 3 4 5,k=5
* 输出:false
*/