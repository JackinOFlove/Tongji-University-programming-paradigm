#include <iostream>
#include <vector>//使用动态vector容器
#include <algorithm>//引用<algorithm>库,方便在vector中调取sort函数
#include <math.h>//运用<math.h>库,方便后续调用max和min函数
#define Max_length 10000
using namespace std;
/*
*我们先定义一个名叫intervals的vector容器,里面存放的是每一部电影的interval_times;
*我们再定义一个名叫interval_times的int类型的vector容器,里面存放着两个元素[interval_starting_times, interval_ending_times],即输入的每部电影的开始时间和结束时间。
*/
bool cmp_max(vector<int> x, vector<int> y)
{
    return x[0] < y[0];//比较函数cmp_max,将intervals中的n部电影进行排序,排序的依据为每一步电影的开始时间的早晚
}
int main()
{
    vector <vector<int>> intervals;//定义动态vector容器intervals
    vector <int> interval_times(2);//定义整型vector容器interval_times,里面存着两个元素
    int n = 0;
    cout << "请输入电影的场数(0-10000场内):";//确定电影场数
    cin >> n;
    while (cin.good() == 0 || n < 0 || n > Max_length)
    {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "输入错误,请重新输入:" << endl;
        cin >> n;
    }//代码的健壮性,考虑到 0 <= intervals.length <= 10^4以及输入数据类型的正确性
    int interval_starting_times, interval_ending_times;//定义整型vector容器里的两个元素
    cout << "请分别输入每一场电影的开始时间和结束时间(0-10000内):" << endl;
    for (int i = 1; i <= n; i++)
    {
        cin >> interval_starting_times >> interval_ending_times;
        while (cin.good() == 0 || interval_starting_times<0 || interval_starting_times>Max_length || interval_ending_times<0 || interval_ending_times>Max_length)
        {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "输入错误,请重新输入:" << endl;
            i = 1;
            cin >> interval_starting_times >> interval_ending_times;
        }//代码的健壮性,考虑到 0 <= starti < endi <= 10^4 以及输入数据类型的正确性
        interval_times[0] = interval_starting_times;//输入一场电影的开始时间
        interval_times[1] = interval_ending_times;//输入一场电影的结束时间
        intervals.push_back(interval_times);//vector中的push_back,即在intervals后插入新的元素,即输入下一场电影的开始时间和结束时间
    }
    sort(intervals.begin(), intervals.end(), cmp_max);//对intervals中的n部电影进行排序,排序依据见上文bool cmp_max函数
    int ending_time = intervals[0][1];//定义ending_time这一个变量,用来记录
    cout <<"[["<<intervals[0][0] <<",";//起始区间的左端点是排序后的intervals中最左边interval_times的第一个元素
    for (unsigned int j = 0; j < intervals.size(); j++)
    {
        if (intervals[j][0] <= ending_time)//如果遍历过程中下一场电影的开始时间小于或等于ending_time,说明前后两场电影有交集
            ending_time = max(ending_time, intervals[j][1]);//此时要重置ending_time,重置后的ending_time是这两场电影结束时间较大的那个
        else if(intervals[j][0] > ending_time)// 如果遍历过程中下一场电影的开始时间大于ending_time,说明前后两场电影没有交集
        {
            cout << ending_time << "]" << "["<<intervals[j][0] << "," ;//此时直接输出前一场电影的结束时间和这一场电影的开始时间
            ending_time = intervals[j][1];//同时要重置ending_time,重置后的ending_time为这一场电影的结束时间,并且继续参与下一次循环
        }
    }
    cout << ending_time <<"]]"<< endl;//输出最后一个ending_time,从而完成电影节目的合并
    return 0;
}
/*测试数据:
*输入:场数:4,时间:1 3 2 6 8 10 15 18
*输出:[[1,6][8,10][15,18]]
*输入:场数:2,时间:1 4 4 5
*输出:[[1,5]]
*/