#include <iostream>
#include <vector>//ʹ�ö�̬vector����
#include <algorithm>//����<algorithm>��,������vector�е�ȡsort����
#include <math.h>//����<math.h>��,��������ʹ��max��min����
#define Max_length 10000
#define Max_time 1000000
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
    cout << "��ֱ�����ÿһ����Ӱ�Ŀ�ʼʱ��ͽ���ʱ��(0-1000000��):" << endl;
    for (int i = 1; i <= n; i++)
    {
        cin >> interval_starting_times >> interval_ending_times;
        while (cin.good() == 0 || interval_starting_times<0 || interval_starting_times>Max_time || interval_ending_times<0 || interval_ending_times>Max_time)
        {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "�������,����������:" << endl;
            i = 1;
            cin >> interval_starting_times >> interval_ending_times;
        }//����Ľ�׳��,���ǵ� 0 <= starti < endi <= 10^6 �Լ������������͵���ȷ��
        interval_times[0] = interval_starting_times;//����һ����Ӱ�Ŀ�ʼʱ��
        interval_times[1] = interval_ending_times;//����һ����Ӱ�Ľ���ʱ��
        intervals.push_back(interval_times);//vector�е�push_back,����intervals������µ�Ԫ��,��������һ����Ӱ�Ŀ�ʼʱ��ͽ���ʱ��
    }
    sort(intervals.begin(), intervals.end(), cmp_max);//��intervals�е�n����Ӱ��������,�������ݼ�����bool_max����
    int numbers = 1;//�趨��ʼ����:������Ҫһ����ӳ��
    for (int j = 1; j < intervals.size();j++)
    {
        if (intervals[j][0] >= intervals[j - 1][1])
        {
            numbers = numbers;
        }//ǰһ����Ӱ�Ľ���ʱ��С���ں�һ����Ӱ�Ŀ�ʼʱ��,����Ҫ�����ӷ�ӳ��,��numbers==numbers
        else 
        {
            numbers += 1;
            intervals[j][1] = min(intervals[j - 1][1], intervals[j][1]);
        }//ǰһ����Ӱ�Ľ���ʱ����ں�һ����Ӱ�Ŀ�ʼʱ��,˵��������Ӱ���ص�����.��Ҫ������һ����ӳ����number+=1,ͬʱ,���µ�Ӱʱ���Ľ���ʱ��,����������������Ӱ����ʱ�����Сֵ,�μ���һ�εı���
    }
    cout <<"������Ҫ"<< numbers<<"����ӳ��" << endl;//������
    return 0;
}
/*��������:
*����:����:3,ʱ��:0 30 5 10 15 20
*���:2
*����:����:2,ʱ��:7 10 2 4
*���:1
*/