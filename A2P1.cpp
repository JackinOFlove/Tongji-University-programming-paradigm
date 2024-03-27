#include <iostream>
#include <vector>//使用动态vector容器
#include <algorithm>//引用<algorithm>库,方便在vector中调取sort函数
#define Max_length 10000
#define Max_time 1000000
using namespace std;
/*
*我们先定义一个名叫intervals的vector容器,里面存放的是每一部电影的interval_times;
*我们再定义一个名叫interval_times的int类型的vector容器,里面存放着两个元素[interval_starting_times, interval_ending_times],即输入的每部电影的开始时间和结束时间。
*/
bool cmp_max(vector<int> &x, vector<int> &y)
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
    while(cin.good()==0||n < 0||n > Max_length)
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
    sort(*intervals.begin(), *intervals.end(),cmp_max);//对intervals中的n部电影进行排序
    if (n == 1 || n == 0)
        cout << "true" << endl;//当n=0或1时,即电影场数为0场或1场时,都可以看完,输出true
    for (int j = 1; j < intervals.size(); j++)//intervals.size()即movies容器中电影场数总数
    {
        if (intervals[j - 1][1] >= intervals[j][0])//当前一场电影的结束时间晚于或等于后一场电影的开始时间(这里我们认为当前一场电影的结束时间等于后一场电影的开始时间时也是不能一起看的),即两场电影时间相互重合,输出false
        {
            cout << "false" << endl;
            return 0;
        }
    }
    cout << "true" << endl;
    return 0;
}

/*测试数据:
*输入:场数:3,时间:0 30 5 10 15 20
*输出:false
*输入:场数:2,时间:7 10 2 4
*输出:true 
*/