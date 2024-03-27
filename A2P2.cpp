#include <iostream>
#include <vector>//使用动态vector容器
#include <algorithm>//引用<algorithm>库,方便在vector中调取sort函数
#include <math.h>//引用<math.h>库,方便下文使用max和min函数
#define Max_length 10000
#define Max_time 1000000
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
    cout << "请分别输入每一场电影的开始时间和结束时间(0-1000000内):" << endl;
    for (int i = 1; i <= n; i++)
    {
        cin >> interval_starting_times >> interval_ending_times;
        while (cin.good() == 0 || interval_starting_times<0 || interval_starting_times>Max_time || interval_ending_times<0 || interval_ending_times>Max_time)
        {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "输入错误,请重新输入:" << endl;
            i = 1;
            cin >> interval_starting_times >> interval_ending_times;
        }//代码的健壮性,考虑到 0 <= starti < endi <= 10^6 以及输入数据类型的正确性
        interval_times[0] = interval_starting_times;//输入一场电影的开始时间
        interval_times[1] = interval_ending_times;//输入一场电影的结束时间
        intervals.push_back(interval_times);//vector中的push_back,即在intervals后插入新的元素,即输入下一场电影的开始时间和结束时间
    }
    sort(intervals.begin(), intervals.end(), cmp_max);//对intervals中的n部电影进行排序,排序依据见下文bool_max函数
    int numbers = 1;//设定起始条件:至少需要一个放映厅
    for (int j = 1; j < intervals.size();j++)
    {
        if (intervals[j][0] >= intervals[j - 1][1])
        {
            numbers = numbers;
        }//前一部电影的结束时间小等于后一部电影的开始时间,则不需要再增加放映厅,即numbers==numbers
        else 
        {
            numbers += 1;
            intervals[j][1] = min(intervals[j - 1][1], intervals[j][1]);
        }//前一部电影的结束时间大于后一部电影的开始时间,说明两部电影有重叠部分.需要再增加一个放映厅即number+=1,同时,更新电影时长的结束时间,让它等于这两部电影结束时间的最小值,参加下一次的遍历
    }
    cout <<"最少需要"<< numbers<<"个放映厅" << endl;//输出结果
    return 0;
}
/*测试数据:
*输入:场数:3,时间:0 30 5 10 15 20
*输出:2
*输入:场数:2,时间:7 10 2 4
*输出:1
*/