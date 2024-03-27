#include <iostream>
#include <vector>//ʹ�ö�̬vector����
#include <algorithm>//����<algorithm>��,������vector�е�ȡsort����
#include <math.h>//����<math.h>��,�����������max��min����
#define Max_length 10000
using namespace std;
/*
*�����ȶ���һ������intervals��vector����,�����ŵ���ÿһ����Ӱ��interval_times;
*�����ٶ���һ������interval_times��int���͵�vector����,������������Ԫ��[interval_starting_times, interval_ending_times],�������ÿ����Ӱ�Ŀ�ʼʱ��ͽ���ʱ�䡣
*/
bool cmp_max(vector<int> x, vector<int> y)
{
    return x[0] < y[0];//�ȽϺ���cmp_max,��intervals�е�n����Ӱ��������,���������Ϊÿһ����Ӱ�Ŀ�ʼʱ�������
}
int main()
{
    vector <vector<int>> intervals;//���嶯̬vector����intervals
    vector <int> interval_times(2);//��������vector����interval_times,�����������Ԫ��
    int n = 0;
    cout << "�������Ӱ�ĳ���(0-10000����):";//ȷ����Ӱ����
    cin >> n;
    while (cin.good() == 0 || n < 0 || n > Max_length)
    {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "�������,����������:" << endl;
        cin >> n;
    }//����Ľ�׳��,���ǵ� 0 <= intervals.length <= 10^4�Լ������������͵���ȷ��
    int interval_starting_times, interval_ending_times;//��������vector�����������Ԫ��
    cout << "��ֱ�����ÿһ����Ӱ�Ŀ�ʼʱ��ͽ���ʱ��(0-10000��):" << endl;
    for (int i = 1; i <= n; i++)
    {
        cin >> interval_starting_times >> interval_ending_times;
        while (cin.good() == 0 || interval_starting_times<0 || interval_starting_times>Max_length || interval_ending_times<0 || interval_ending_times>Max_length)
        {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "�������,����������:" << endl;
            i = 1;
            cin >> interval_starting_times >> interval_ending_times;
        }//����Ľ�׳��,���ǵ� 0 <= starti < endi <= 10^4 �Լ������������͵���ȷ��
        interval_times[0] = interval_starting_times;//����һ����Ӱ�Ŀ�ʼʱ��
        interval_times[1] = interval_ending_times;//����һ����Ӱ�Ľ���ʱ��
        intervals.push_back(interval_times);//vector�е�push_back,����intervals������µ�Ԫ��,��������һ����Ӱ�Ŀ�ʼʱ��ͽ���ʱ��
    }
    sort(intervals.begin(), intervals.end(), cmp_max);//��intervals�е�n����Ӱ��������,�������ݼ�����bool cmp_max����
    int ending_time = intervals[0][1];//����ending_time��һ������,������¼
    cout <<"[["<<intervals[0][0] <<",";//��ʼ�������˵���������intervals�������interval_times�ĵ�һ��Ԫ��
    for (unsigned int j = 0; j < intervals.size(); j++)
    {
        if (intervals[j][0] <= ending_time)//���������������һ����Ӱ�Ŀ�ʼʱ��С�ڻ����ending_time,˵��ǰ��������Ӱ�н���
            ending_time = max(ending_time, intervals[j][1]);//��ʱҪ����ending_time,���ú��ending_time����������Ӱ����ʱ��ϴ���Ǹ�
        else if(intervals[j][0] > ending_time)// ���������������һ����Ӱ�Ŀ�ʼʱ�����ending_time,˵��ǰ��������Ӱû�н���
        {
            cout << ending_time << "]" << "["<<intervals[j][0] << "," ;//��ʱֱ�����ǰһ����Ӱ�Ľ���ʱ�����һ����Ӱ�Ŀ�ʼʱ��
            ending_time = intervals[j][1];//ͬʱҪ����ending_time,���ú��ending_timeΪ��һ����Ӱ�Ľ���ʱ��,���Ҽ���������һ��ѭ��
        }
    }
    cout << ending_time <<"]]"<< endl;//������һ��ending_time,�Ӷ���ɵ�Ӱ��Ŀ�ĺϲ�
    return 0;
}
/*��������:
*����:����:4,ʱ��:1 3 2 6 8 10 15 18
*���:[[1,6][8,10][15,18]]
*����:����:2,ʱ��:1 4 4 5
*���:[[1,5]]
*/