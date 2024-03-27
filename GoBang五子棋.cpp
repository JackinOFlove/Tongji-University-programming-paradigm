#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <vector>
using namespace std;
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define TRUE 1
#define FALSE 0
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"
int Turn = 0;                                  //�ִμ���
int myFlag;
int enemyFlag;
char board[BOARD_SIZE][BOARD_SIZE] = { 0 };
struct Command
{
    int x;
    int y;
};
//����ṹ�壺��һ�����ӵ�
struct Next_Command
{
    struct Command command;         //��һ�����ӵ�����
    int This_Command_Value;         //������ӵ����������
};  
//����ṹ�壺����
struct chessshape
{
    int score;                      //�������͵ķ���
    char shape[9];                  //�������͵���״
};
//30�����͵ķ�������״����
chessshape scoreof_eachshape[30] = {
    {50,"01100"},
    {50,"00110"},
    {200,"11010"},
    {500,"00111"},
    {500,"11100"},
    {5000,"01110"},
    {5000,"010110"},
    {5000,"011010"},
    {5000,"11101"},
    {5000,"11011"},
    {5000,"10111"},
    {5000,"11110"},
    {5000,"01111"},
    {50000,"011110"},
    {99999999,"11111"},

    {-50,"02200"},
    {-50,"00220"},
    {-200,"22020"},
    {-500,"00222"},
    {-500,"22200"},
    {-5000,"02220"},
    {-5000,"020220"},
    {-5000,"022020"},
    {-5000,"22202"},
    {-5000,"22022"},
    {-5000,"20222"},
    {-5000,"22220"},
    {-5000,"02222"},
    {-50000,"022220"},
    {-99999999,"22222"}
};
void debug(const char* str)
{
    printf("DEBUG %s\n", str);
    fflush(stdout);
}
bool isInBound(int x, int y)
{
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}
void place(struct Command cmd)
{
    board[cmd.x][cmd.y] = enemyFlag;
}
//���Է�������
void putchess(struct Command cmd, int flag)
{
    board[cmd.x][cmd.y] = flag;
}
//�õ�����
void unputchess(struct Command cmd)
{
    board[cmd.x][cmd.y] = EMPTY;
}
//��ӡ���̺������ύ����վʱ����ʾ��
void Print_Board() 
{
    int width = 5;
    cout << setw(width) << " ";
    for (int i = 0; i < BOARD_SIZE; i++)
        cout << setw(width) << i;
    cout << endl;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cout << setw(width) << i;
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == EMPTY)
                cout << setw(width) << ".";
            else if (board[i][j] == BLACK)
                cout << setw(width) << "X";
            else if (board[i][j] == WHITE)
                cout << setw(width) << "0";
        }
        cout << endl << endl;
    }
    cout << endl;
}
//�жϵ�ǰ����Ƿ�����������
int Have_Five_InBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            //����ҷ����������飬return 1
            if (col + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row][col + 1] == myFlag && board[row][col + 2] == myFlag && board[row][col + 3] == myFlag && board[row][col + 4] == myFlag)
                return 1;
            else if (row + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col] == myFlag && board[row + 2][col] == myFlag && board[row + 3][col] == myFlag && board[row + 4][col] == myFlag)
                return 1;
            else if (row + 4 < BOARD_SIZE && col + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col + 1] == myFlag && board[row + 2][col + 2] == myFlag && board[row + 3][col + 3] == myFlag && board[row + 4][col + 4] == myFlag)
                return 1;
            else if (row < BOARD_SIZE && row - 4 >= 0 && col < BOARD_SIZE && col - 4 >= 0 && board[row][col] == myFlag && board[row - 1][col - 1] == myFlag && board[row - 2][col - 2] == myFlag && board[row - 3][col - 3] == myFlag && board[row - 4][col - 4] == myFlag)
                return 1;
            //����з����������飬return -1
            else if (col + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row][col + 1] == enemyFlag && board[row][col + 2] == enemyFlag && board[row][col + 3] == enemyFlag && board[row][col + 4] == enemyFlag)
                return -1;
            else if (row + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row + 1][col] == enemyFlag && board[row + 2][col] == enemyFlag && board[row + 3][col] == enemyFlag && board[row + 4][col] == enemyFlag)
                return -1;
            else if (row + 4 < BOARD_SIZE && col + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row + 1][col + 1] == enemyFlag && board[row + 2][col + 2] == enemyFlag && board[row + 3][col + 3] == enemyFlag && board[row + 4][col + 4] == enemyFlag)
                return -1;
            else if (row < BOARD_SIZE && row - 4 >= 0 && col < BOARD_SIZE && col - 4 >= 0 && board[row][col] == enemyFlag && board[row - 1][col - 1] == enemyFlag && board[row - 2][col - 2] == enemyFlag && board[row - 3][col - 3] == enemyFlag && board[row - 4][col - 4] == enemyFlag)
                return -1;
        }
    }
    //����û���������飬return 0
    return 0;
}
void initAI(int me)
{
    enemyFlag = 3 - me;
}
void start(int flag)
{
    memset(board, 0, sizeof(board));
    int middlePlace = BOARD_SIZE / 2;
    board[middlePlace - 1][middlePlace - 1] = WHITE;
    board[middlePlace][middlePlace] = WHITE;
    board[middlePlace - 1][middlePlace] = BLACK;
    board[middlePlace][middlePlace - 1] = BLACK;
    initAI(flag);
}
//��������������һ��Ҫ�ߵĶ���ѡ��ķ����ߵ�����
bool Cmp_Max(Next_Command choiceA, Next_Command choiceB)
{
    return choiceA.This_Command_Value > choiceB.This_Command_Value;
}
//���㵱ǰ���̸�������ÿһ�����͵ĸ������������������涨����ˣ�
int Cal_Substr_Number(char* str, char* sub_str)
{
    char* str_local = NULL;
    char* sub_str_local = NULL;
    int num = 0;
    if (!str || !sub_str)           //����������ڣ�return 0
        return 0;  
    while (*str)
    {
        str_local = str;
        sub_str_local = sub_str;
        do
        {
            if (*sub_str_local == '\0')     //�ȶԳɹ�
            {
                num++;
                break;
            }
        } while (*str_local++ == *sub_str_local++);
        str += 1;                   //��0��ʼ�������𲽼�����������
    }
    return num;
}
//���ĸ���������ת����Ӧ���ַ���
char* Transform(int type, int x, int y)
{
    int i;
    static char result[BOARD_SIZE + 1];     //���巵�ص��ַ�������
    //����
    if (type == 1)
    {
        for (i = 0; i < BOARD_SIZE; i++)
            result[i] = board[x][i] + '0';
        result[i] = '\0';
        return result;
    }
    //����
    if (type == 2)
    {
        for (i = 0; i < BOARD_SIZE; i++)
            result[i] = board[i][y] + '0';
        result[i] = '\0';
        return result;
    }
    //���Խ���
    if (type == 3)
    {
        for (i = 0; (i + x < BOARD_SIZE && i + y < BOARD_SIZE); i++)
            result[i] = board[x + i][y + i] + '0';
        result[i] = '\0';
        return result;
    }
    //���Խ���
    if (type == 4)
    {
        for (i = 0; (x + i < BOARD_SIZE && y - i >= 0); i++)
            result[i] = board[x + i][y - i] + '0';
        result[i] = '\0';
        return result;
    }
}
//����ĳһ�������ڸ����̵��ܷ���
int Score_evaluation(char* str)
{
    int sum = 0;
    int subsumvalue = 0;
    for (int i = 0; i < 30; i++)
    {
        subsumvalue = scoreof_eachshape[i].score * Cal_Substr_Number(str, scoreof_eachshape[i].shape);      //���������ܷ���=������������*�������ͷ���
        if (myFlag == 2)                        //����з����֣�����ת��
            subsumvalue = -subsumvalue;
        sum += subsumvalue;
    }
    return sum;
}
//��������̵��ܷ������������͵�����*��Ӧ���͵ķ�����
int Sum_evaluation(char board[BOARD_SIZE][BOARD_SIZE])
{
    int x, y;
    int sum = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        x = i;
        y = 0;
        sum += Score_evaluation(Transform(1, x, y));
    }
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        x = 0;
        y = i;
        sum += Score_evaluation(Transform(2, x, y));
    }
    for (int i = 0; i < BOARD_SIZE - 4; i++)
    {
        x = 0;
        y = i;
        sum += Score_evaluation(Transform(3, x, y));
    }
    for (int i = 1; i < BOARD_SIZE - 4; i++)
    {
        x = i;
        y = 0;
        sum += Score_evaluation(Transform(3, x, y));
    }
    for (int i = 4; i < BOARD_SIZE; i++)
    {
        x = 0;
        y = i;
        sum += Score_evaluation(Transform(4, x, y));
    }
    for (int i = 1; i < BOARD_SIZE - 4; i++)
    {
        x = i;
        y = BOARD_SIZE - 1;
        sum += Score_evaluation(Transform(4, x, y));
    }
    return sum;             //��õ�ǰ������ܷ���
}
//�������ӣ�����������λ�ú�ı�ķ���
int Change_evaluation(struct Command cmd, int player)
{
    int sumvalue = 0;
    char* temp;
    putchess(cmd, player);                  //��������
    if (Have_Five_InBoard(board))           //�������������Ӻ�����������
    {
        if (player == myFlag)               //������ҷ���������
            sumvalue = INT_MAX;             //��������
        else                                //����ǵз���������
            sumvalue = INT_MIN;             //������С
        unputchess(cmd);                    //��������
    }
    else
    {
        unputchess(cmd);                    //��������
        // ����
        temp = Transform(1, cmd.x, 0);      //�������͸ı�
        sumvalue -= Score_evaluation(temp);
        putchess(cmd, player);              //��������
        temp = Transform(1, cmd.x, 0);
        sumvalue += Score_evaluation(temp);
        unputchess(cmd);                    //��������
        // ����
        temp = Transform(2, 0, cmd.y);      //�������͸ı�
        sumvalue -= Score_evaluation(temp);
        putchess(cmd, player);              //��������
        temp = Transform(2, 0, cmd.y);
        sumvalue += Score_evaluation(temp);
        unputchess(cmd);                    //��������
        // ���Խ��߷���
        int min_xy = min(cmd.x, cmd.y);
        temp = Transform(3, cmd.x - min_xy, cmd.y - min_xy);    //���Խ������͸ı�
        sumvalue -= Score_evaluation(temp);
        putchess(cmd, player);              //��������
        temp = Transform(3, cmd.x - min_xy, cmd.y - min_xy);
        sumvalue += Score_evaluation(temp);
        unputchess(cmd);                    //��������
        // ���Խ��߷���
        min_xy = min(cmd.x, BOARD_SIZE - cmd.y - 1);
        temp = Transform(4, cmd.x - min_xy, cmd.y + min_xy);    //���Խ������͸ı�
        sumvalue -= Score_evaluation(temp);
        putchess(cmd, player);              //��������
        temp = Transform(4, cmd.x - min_xy, cmd.y + min_xy);
        sumvalue += Score_evaluation(temp);
        unputchess(cmd);                    //��������
    }
    return sumvalue;            //���㲢���ظı�ķ���
}
//�������ӷ���
int Command_evaluation(struct Command command, int player)
{
    int Five_chess = 0, huosi = 0, miansi = 0, huosan = 0, miansan = 0, erzi = 0;     //�������顢���ġ����ġ������������Ͷ��ӵļ���
    // ��������ж�
    int leftSpace = 0, rightSpace = 0, number = 1;                              //������ո��������ҿո�������������
    for (int i = 1, Space_Flag = 0; isInBound(command.x - i, command.y) && board[command.x - i][command.y] != 3 - player; i++)  //Space_Flag�����жϡ��ߡ��롰�
    {
        if (!Space_Flag)                        
        {
            if (board[command.x - i][command.y] == player)
                number++;
            else
            {
                Space_Flag = 1;
                leftSpace++;
            }
        }
        else leftSpace++;
    }
    // �����Ҳ��ж�
    for (int i = 1, Space_Flag = 0; isInBound(command.x + i, command.y) && board[command.x + i][command.y] != 3 - player; i++)
    {
        if (!Space_Flag)
        {
            if (board[command.x + i][command.y] == player)
                number++;
            else
            {
                Space_Flag = 1;
                rightSpace++;
            }
        }
        else rightSpace++;
    }
    //����������Ӽ���
    if (number == 5)
        Five_chess++;
    else if (number == 4 && leftSpace && rightSpace)
        huosi++;
    else if (number == 4 && (leftSpace || rightSpace))
        miansi++;
    else if (number == 3 && leftSpace >= 2 && rightSpace >= 2)
        huosan++;
    else if (number == 3 && (leftSpace >= 2 || rightSpace >= 2))
        miansan++;
    else if (number == 2 && leftSpace >= 2 && rightSpace >= 2)
        erzi++;
    //��������ж�
    //����leftSpace��rightSpace��number
    leftSpace = rightSpace = 0;
    number = 1;
    for (int i = 1, Space_Flag = 0; isInBound(command.x, command.y - i) && board[command.x][command.y - i] != 3 - player; i++)
    {
        if (!Space_Flag)
        {
            if (board[command.x][command.y - i] == player)
                number++;
            else
            {
                Space_Flag = 1;
                leftSpace++;
            }
        }
        else leftSpace++;
    }
    //�����Ҳ��ж�
    for (int i = 1, Space_Flag = 0; isInBound(command.x, command.y + i) && board[command.x][command.y + i] != 3 - player; i++)
    {
        if (!Space_Flag)
        {
            if (board[command.x][command.y + i] == player)
                number++;
            else
            {
                Space_Flag = 1;
                rightSpace++;
            }
        }
        else rightSpace++;
    }
    //����������Ӽ���
    if (number == 5)
        Five_chess++;
    else if (number == 4 && leftSpace && rightSpace)
        huosi++;
    else if (number == 4 && (leftSpace || rightSpace))
        miansi++;
    else if (number == 3 && leftSpace >= 2 && rightSpace >= 2)
        huosan++;
    else if (number == 3 && (leftSpace >= 2 || rightSpace >= 2))
        miansan++;
    else if (number == 2 && leftSpace >= 2 && rightSpace >= 2)
        erzi++;
    //���Խ�������ж�
    //����leftSpace��rightSpace��number
    leftSpace = rightSpace = 0;
    number = 1;
    for (int i = 1, Space_Flag = 0; isInBound(command.x - i, command.y - i) && board[command.x - i][command.y - i] != 3 - player; i++)
    {
        if (!Space_Flag)
        {
            if (board[command.x - i][command.y - i] == player)
                number++;
            else
            {
                Space_Flag = 1;
                leftSpace++;
            }
        }
        else leftSpace++;
    }
    //���Խ����Ҳ��ж�
    for (int i = 1, Space_Flag = 0; isInBound(command.x + i, command.y + i) && board[command.x + i][command.y + i] != 3 - player; i++)
    {
        if (!Space_Flag)
        {
            if (board[command.x + i][command.y + i] == player)
                number++;
            else
            {
                Space_Flag = 1;
                rightSpace++;
            }
        }
        else rightSpace++;
    }
    //���Խ��߸������Ӽ���
    if (number == 5)
        Five_chess++;
    else if (number == 4 && leftSpace && rightSpace)
        huosi++;
    else if (number == 4 && (leftSpace || rightSpace))
        miansi++;
    else if (number == 3 && leftSpace >= 2 && rightSpace >= 2)
        huosan++;
    else if (number == 3 && (leftSpace >= 2 || rightSpace >= 2))
        miansan++;
    else if (number == 2 && leftSpace >= 2 && rightSpace >= 2)
        erzi++;
    //���Խ�������ж�
    //����leftSpace��rightSpace��number
    leftSpace = rightSpace = 0;
    number = 1;
    for (int i = 1, Space_Flag = 0; isInBound(command.x - i, command.y + i) && board[command.x - i][command.y + i] != 3 - player; i++)
    {
        if (!Space_Flag)
        {
            if (board[command.x - i][command.y + i] == player)
                number++;
            else
            {
                Space_Flag = 1;
                leftSpace++;
            }
        }
        else leftSpace++;
    }
    //���Խ����Ҳ��ж�
    for (int i = 1, Space_Flag = 0; isInBound(command.x + i, command.y - i) && board[command.x + i][command.y - i] != 3 - player; i++)
    {
        if (!Space_Flag)
        {
            if (board[command.x + i][command.y - i] == player)
                number++;
            else
            {
                Space_Flag = 1;
                rightSpace++;
            }
        }
        else rightSpace++;
    }
    //���Խ��߸������
    if (number == 5)
        Five_chess++;
    else if (number == 4 && leftSpace && rightSpace)
        huosi++;
    else if (number == 4 && (leftSpace || rightSpace))
        miansi++;
    else if (number == 3 && leftSpace >= 2 && rightSpace >= 2)
        huosan++;
    else if (number == 3 && (leftSpace >= 2 || rightSpace >= 2))
        miansan++;
    else if (number == 2 && leftSpace >= 2 && rightSpace >= 2)
        erzi++;
    //���������
    if (Five_chess)
        return INT_MAX;                              //�������������
    if (huosi)
        return 99999999;                            //����л���   
    return miansi * 1000 + huosan * 1000 + miansan * 200 + erzi * 200;      //���ఴȨ�ظ���
}
//���һ��װ����һ���������ӵ��vector�����ĺ���
//������һ�������ӵ����Χһ�㶼�������ӵģ����ǲ�����������̫Զ�������ҽ�������Χ��СΪ�����Ӵ��İ뾶Ϊ2�ķ�Χ��
//�����ܼ���������ʱ��
vector<Next_Command>* Next_Commands_Vector(int player)
{
    int Search_r = 2;                                //�����뾶Ϊ2
    bool Point_Flag[BOARD_SIZE][BOARD_SIZE];         //�����ά���������жϸõ��Ƿ�������������������Ͳ�������������Ҳ�ܼ���ʱ��
    for (int row = 0; row < BOARD_SIZE; row++)       //��ʼ��Point_Flag��ά���飬��ʼʱ��Ϊfalse����ʾ��û�б�������
    {
        for (int col = 0; col < BOARD_SIZE; col++)
            Point_Flag[row][col] = false;
    }
    vector<Next_Command>* Commands_Vector = new vector<Next_Command>;   //�����µĿռ�
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] != EMPTY)                                   //������λ���ǿյĻ�
            {
                for (int k = -Search_r; k <= Search_r; k++)
                {
                    if (k == 0)                                         //�õ㲻�������ӣ�ֱ������
                    {
                        k++;
                        continue;
                    }
                    if (isInBound(i + k, j) && !Point_Flag[i + k][j] && board[i + k][j] == EMPTY)   //����
                    {  
                        Point_Flag[i + k][j] = TRUE;        //�˵��Ϊ��������
                        Next_Command c;                     //��һ���Ŀ�������ѡ��
                        c.command.x = i + k;                //��ֵx
                        c.command.y = j;                    //��ֵy
                        c.This_Command_Value = Command_evaluation(c.command, player);   //�����ѡ��ķ���
                        Commands_Vector->push_back(c);      //�����ѡ����뵽װ����һ���������ӵ��vector����Commands_Vector��
                    }
                    if (isInBound(i, j + k) && !Point_Flag[i][j + k] && board[i][j + k] == EMPTY)   //����
                    {  
                        Point_Flag[i][j + k] = TRUE;        //�˵��Ϊ��������
                        Next_Command c;                     //��һ���Ŀ�������ѡ��
                        c.command.x = i;                    //��ֵx
                        c.command.y = j + k;                //��ֵy
                        c.This_Command_Value = Command_evaluation(c.command, player);   //�����ѡ��ķ���
                        Commands_Vector->push_back(c);      //�����ѡ����뵽װ����һ���������ӵ��vector����Commands_Vector��
                    }
                    if (isInBound(i + k, j + k) && !Point_Flag[i + k][j + k] && board[i + k][j + k] == EMPTY)   //���Խ��߷���
                    {  
                        Point_Flag[i + k][j + k] = TRUE;    //�˵��Ϊ��������
                        Next_Command c;                     //��һ���Ŀ�������ѡ��
                        c.command.x = i + k;                //��ֵx
                        c.command.y = j + k;                //��ֵy
                        c.This_Command_Value = Command_evaluation(c.command, player);    //�����ѡ��ķ���
                        Commands_Vector->push_back(c);      //�����ѡ����뵽װ����һ���������ӵ��vector����Commands_Vector��
                    }
                    if (isInBound(i + k, j - k) && !Point_Flag[i + k][j - k] && board[i + k][j - k] == EMPTY)   //���Խ��߷���
                    {  
                        Point_Flag[i + k][j - k] = TRUE;    //�˵��Ϊ��������
                        Next_Command c;                     //��һ���Ŀ�������ѡ��
                        c.command.x = i + k;                //��ֵx
                        c.command.y = j - k;                //��ֵy
                        c.This_Command_Value = Command_evaluation(c.command, player);   //�����ѡ��ķ���
                        Commands_Vector->push_back(c);      //�����ѡ����뵽װ����һ���������ӵ��vector����Commands_Vector��
                    }
                }
            }
        }
    }
    sort(Commands_Vector->begin(), Commands_Vector->end(), Cmp_Max);    //������������Щѡ�������������Ĵ�С���򣬿����ٺ�������ʱ��
    return Commands_Vector;
}
int MAX_DEPTH;          //��������������
int evaluationValue;    //�����ܷ�
//alphaBeta��֦����
int alphaBetapruning(int depth, int alpha, int beta, int MaxMinPlayer)
{
    int eval; 
    int Origin_EvaluationValue = evaluationValue;   //�����̴�ʱ�ķ�������Ϊ�������
    if (MaxMinPlayer == myFlag)                     //����ҷ���MaxPlayer
    {
        auto Commands_Vector = Next_Commands_Vector(MaxMinPlayer);
        for (Next_Command& node : *Commands_Vector)
        {
            struct Command c = node.command;
            if (depth == MAX_DEPTH - 1)
            {
                {
                    eval = Change_evaluation(c, myFlag);    //eval����Ϊ�������command�ı�ķ���
                    if (eval != INT_MAX && eval != INT_MIN)
                        eval += evaluationValue;            //���û���������飨eval != INT_MAX �� eval != INT_MIN�����������
                }
            }
            else
            {
                int Plus_Value = Change_evaluation(c, myFlag);
                if (Plus_Value == INT_MAX)                  //���������������ҷ��������飩
                {
                    alpha = INT_MAX;                        //alpha��Ϊ���������
                    break;
                }
                else
                {
                    evaluationValue = Origin_EvaluationValue + Plus_Value;  //��������������ܷ�
                    putchess(c, myFlag);                    //��������
                    if (Have_Five_InBoard(board))
                        eval = INT_MAX;
                    else
                        eval = alphaBetapruning(depth + 1, alpha, beta, 3 - MaxMinPlayer);  //�ݹ���������һ��
                    unputchess(c);                            //��������
                }
            }
            alpha = alpha > eval ? alpha : eval;
            if (beta <= alpha)           //beta��֦
            {
                delete Commands_Vector;
                return beta;
            }
        }
        delete Commands_Vector;
        return alpha;
    }
    else       //����ҷ���MinPlayer
    {
        auto Commands_Vector = Next_Commands_Vector(enemyFlag);
        for (Next_Command& node : *Commands_Vector)
        {
            Command c = node.command;
            if (depth == MAX_DEPTH - 1)
            {
                eval = Change_evaluation(c, myFlag);            //eval����Ϊ�������command�ı�ķ���
                if (eval != INT_MIN && eval != INT_MAX)         //���û���������飨eval != INT_MAX �� eval != INT_MIN�����������
                    eval += evaluationValue;
            }
            else
            {
                int Plus_Value = Change_evaluation(c, enemyFlag); 
                if (Plus_Value == INT_MIN)                          //������������С���з��������飩
                {
                    beta = INT_MIN;                                 //beta��Ϊ���������
                    break;
                }
                else
                {
                    evaluationValue = Origin_EvaluationValue + Plus_Value;  //��������������ܷ�
                    putchess(c, enemyFlag);         //��������
                    if (Have_Five_InBoard(board))
                        eval = INT_MIN;
                    else eval = alphaBetapruning(depth + 1, alpha, beta, 3 - MaxMinPlayer); //�ݹ���������һ��
                    unputchess(c);                  //��������
                }
            }
            beta = beta < eval ? beta : eval;
            if (beta <= alpha)              //alpha��֦
            {
                delete Commands_Vector;
                return alpha;
            }
        }
        delete Commands_Vector;
        return beta;
    }
}
//�õ���������λ�õĺ���
Command Get_Bestmove(int alpha, int beta, int MaxMinPlayer)
{
    vector<Next_Command>* Commands_Vector = Next_Commands_Vector(MaxMinPlayer);
    Command bestMove = (*Commands_Vector)[0].command;
    //һЩ��������ж�
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            //�ҷ����ģ�ֱ����
            if (col + 5 < BOARD_SIZE && board[row][col] == enemyFlag && board[row][col + 1] == myFlag && board[row][col + 2] == myFlag && board[row][col + 3] == myFlag && board[row][col + 4] == myFlag && board[row][col + 5] == EMPTY)
            {
                bestMove.x = row;
                bestMove.y = col + 5;
                return bestMove;
            }
            if (col + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row][col + 1] == myFlag && board[row][col + 2] == myFlag && board[row][col + 3] == myFlag && board[row][col + 4] == myFlag && board[row][col + 5] == enemyFlag)
            {
                bestMove.x = row;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && board[row][col] == enemyFlag && board[row + 1][col] == myFlag && board[row + 2][col] == myFlag && board[row + 3][col] == myFlag && board[row + 4][col] == myFlag && board[row + 5][col] == EMPTY)
            {
                bestMove.x = row + 5;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col] == myFlag && board[row + 2][col] == myFlag && board[row + 3][col] == myFlag && board[row + 4][col] == myFlag && board[row + 5][col] == enemyFlag)
            {
                bestMove.x = row;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col + 5 < BOARD_SIZE && board[row][col] == enemyFlag && board[row + 1][col + 1] == myFlag && board[row + 2][col + 2] == myFlag && board[row + 3][col + 3] == myFlag && board[row + 4][col + 4] == myFlag && board[row + 5][col + 5] == EMPTY)
            {
                bestMove.x = row + 5;
                bestMove.y = col + 5;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col + 1] == myFlag && board[row + 2][col + 2] == myFlag && board[row + 3][col + 3] == myFlag && board[row + 4][col + 4] == myFlag && board[row + 5][col + 5] == enemyFlag)
            {
                bestMove.x = row;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col - 5 >= 0 && board[row][col] == enemyFlag && board[row + 1][col - 1] == myFlag && board[row + 2][col - 2] == myFlag && board[row + 3][col - 3] == myFlag && board[row + 4][col - 4] == myFlag && board[row + 5][col - 5] == EMPTY)
            {
                bestMove.x = row + 5;
                bestMove.y = col - 5;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col - 5 >= 0 && board[row][col] == EMPTY && board[row + 1][col - 1] == myFlag && board[row + 2][col - 2] == myFlag && board[row + 3][col - 3] == myFlag && board[row + 4][col - 4] == myFlag && board[row + 5][col - 5] == enemyFlag)
            {
                bestMove.x = row;
                bestMove.y = col;
                return bestMove;
            }
            //�ҷ����ģ�ֱ����
            if (col + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row][col + 1] == myFlag && board[row][col + 2] == myFlag && board[row][col + 3] == myFlag && board[row][col + 4] == myFlag && board[row][col + 5] == EMPTY)
            {
                bestMove.x = row;
                bestMove.y = col + 5;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col] == myFlag && board[row + 2][col] == myFlag && board[row + 3][col] == myFlag && board[row + 4][col] == myFlag && board[row + 5][col] == EMPTY)
            {
                bestMove.x = row + 5;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col + 1] == myFlag && board[row + 2][col + 2] == myFlag && board[row + 3][col + 3] == myFlag && board[row + 4][col + 4] == myFlag && board[row + 5][col + 5] == EMPTY)
            {
                bestMove.x = row + 5;
                bestMove.y = col + 5;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col - 5 >= 0 && board[row][col] == EMPTY && board[row + 1][col - 1] == myFlag && board[row + 2][col - 2] == myFlag && board[row + 3][col - 3] == myFlag && board[row + 4][col - 4] == myFlag && board[row + 5][col - 5] == EMPTY)
            {
                bestMove.x = row + 5;
                bestMove.y = col - 5;
                return bestMove;
            }
            //�ҷ���+һ��ֱ����
            if (row + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col] == EMPTY && board[row + 2][col] == myFlag && board[row + 3][col] == myFlag && board[row + 4][col] == myFlag)
            {
                bestMove.x = row + 1;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col] == myFlag && board[row + 2][col] == EMPTY && board[row + 3][col] == myFlag && board[row + 4][col] == myFlag)
            {
                bestMove.x = row + 2;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col] == myFlag && board[row + 2][col] == myFlag && board[row + 3][col] == EMPTY && board[row + 4][col] == myFlag)
            {
                bestMove.x = row + 3;
                bestMove.y = col;
                return bestMove;
            }
            if (col + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row][col + 1] == EMPTY && board[row][col + 2] == myFlag && board[row][col + 3] == myFlag && board[row][col + 4] == myFlag)
            {
                bestMove.x = row;
                bestMove.y = col + 1;
                return bestMove;
            }
            if (col + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row][col + 1] == myFlag && board[row][col + 2] == myFlag && board[row][col + 3] == EMPTY && board[row][col + 4] == myFlag)
            {
                bestMove.x = row;
                bestMove.y = col + 3;
                return bestMove;
            }
            if (col + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row][col + 1] == myFlag && board[row][col + 2] == EMPTY && board[row][col + 3] == myFlag && board[row][col + 4] == myFlag)
            {
                bestMove.x = row;
                bestMove.y = col + 2;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col + 1] == EMPTY && board[row + 2][col + 2] == myFlag && board[row + 3][col + 3] == myFlag && board[row + 4][col + 4] == myFlag)
            {
                bestMove.x = row + 1;
                bestMove.y = col + 1;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col + 1] == myFlag && board[row + 2][col + 2] == EMPTY && board[row + 3][col + 3] == myFlag && board[row + 4][col + 4] == myFlag)
            {
                bestMove.x = row + 2;
                bestMove.y = col + 2;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col + 1] == myFlag && board[row + 2][col + 2] == myFlag && board[row + 3][col + 3] == EMPTY && board[row + 4][col + 4] == myFlag)
            {
                bestMove.x = row + 3;
                bestMove.y = col + 3;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col - 4 >= 0 && board[row][col] == myFlag && board[row + 1][col - 1] == EMPTY && board[row + 2][col - 2] == myFlag && board[row + 3][col - 3] == myFlag && board[row + 4][col - 4] == myFlag)
            {
                bestMove.x = row + 1;
                bestMove.y = col - 1;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col - 4 >= 0 && board[row][col] == myFlag && board[row + 1][col - 1] == myFlag && board[row + 2][col - 2] == EMPTY && board[row + 3][col - 3] == myFlag && board[row + 4][col - 4] == myFlag)
            {
                bestMove.x = row + 2;
                bestMove.y = col - 2;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col - 4 >= 0 && board[row][col] == myFlag && board[row + 1][col - 1] == myFlag && board[row + 2][col - 2] == myFlag && board[row + 3][col - 3] == EMPTY && board[row + 4][col - 4] == myFlag)
            {
                bestMove.x = row + 3;
                bestMove.y = col - 3;
                return bestMove;
            }
        }
    }
    //�з����ĺ͵з���+һ�������
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (col + 5 < BOARD_SIZE && board[row][col] == myFlag && board[row][col + 1] == enemyFlag && board[row][col + 2] == enemyFlag && board[row][col + 3] == enemyFlag && board[row][col + 4] == enemyFlag && board[row][col + 5] == EMPTY)
            {
                bestMove.x = row;
                bestMove.y = col + 5;
                return bestMove;
            }
            if (col + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row][col + 1] == enemyFlag && board[row][col + 2] == enemyFlag && board[row][col + 3] == enemyFlag && board[row][col + 4] == enemyFlag && board[row][col + 5] == myFlag)
            {
                bestMove.x = row;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col] == enemyFlag && board[row + 2][col] == enemyFlag && board[row + 3][col] == enemyFlag && board[row + 4][col] == enemyFlag && board[row + 5][col] == EMPTY)
            {
                bestMove.x = row + 5;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col] == enemyFlag && board[row + 2][col] == enemyFlag && board[row + 3][col] == enemyFlag && board[row + 4][col] == enemyFlag && board[row + 5][col] == myFlag)
            {
                bestMove.x = row;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col + 5 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col + 1] == enemyFlag && board[row + 2][col + 2] == enemyFlag && board[row + 3][col + 3] == enemyFlag && board[row + 4][col + 4] == enemyFlag && board[row + 5][col + 5] == EMPTY)
            {
                bestMove.x = row + 5;
                bestMove.y = col + 5;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col + 1] == enemyFlag && board[row + 2][col + 2] == enemyFlag && board[row + 3][col + 3] == enemyFlag && board[row + 4][col + 4] == enemyFlag && board[row + 5][col + 5] == myFlag)
            {
                bestMove.x = row;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col - 5 >= 0 && board[row][col] == myFlag && board[row + 1][col - 1] == enemyFlag && board[row + 2][col - 2] == enemyFlag && board[row + 3][col - 3] == enemyFlag && board[row + 4][col - 4] == enemyFlag && board[row + 5][col - 5] == EMPTY)
            {
                bestMove.x = row + 5;
                bestMove.y = col - 5;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col - 5 >= 0 && board[row][col] == EMPTY && board[row + 1][col - 1] == enemyFlag && board[row + 2][col - 2] == enemyFlag && board[row + 3][col - 3] == enemyFlag && board[row + 4][col - 4] == enemyFlag && board[row + 5][col - 5] == myFlag)
            {
                bestMove.x = row;
                bestMove.y = col;
                return bestMove;
            }
            
            if (row + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row + 1][col] == EMPTY && board[row + 2][col] == enemyFlag && board[row + 3][col] == enemyFlag && board[row + 4][col] == enemyFlag)
            {
                bestMove.x = row + 1;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row + 1][col] == enemyFlag && board[row + 2][col] == EMPTY && board[row + 3][col] == enemyFlag && board[row + 4][col] == enemyFlag)
            {
                bestMove.x = row + 2;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row + 1][col] == enemyFlag && board[row + 2][col] == enemyFlag && board[row + 3][col] == EMPTY && board[row + 4][col] == enemyFlag)
            {
                bestMove.x = row + 3;
                bestMove.y = col;
                return bestMove;
            }
            if (col + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row][col + 1] == EMPTY && board[row][col + 2] == enemyFlag && board[row][col + 3] == enemyFlag && board[row][col + 4] == enemyFlag)
            {
                bestMove.x = row;
                bestMove.y = col + 1;
                return bestMove;
            }
            if (col + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row][col + 1] == enemyFlag && board[row][col + 2] == enemyFlag && board[row][col + 3] == EMPTY && board[row][col + 4] == enemyFlag)
            {
                bestMove.x = row;
                bestMove.y = col + 3;
                return bestMove;
            }
            if (col + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row][col + 1] == enemyFlag && board[row][col + 2] == EMPTY && board[row][col + 3] == enemyFlag && board[row][col + 4] == enemyFlag)
            {
                bestMove.x = row;
                bestMove.y = col + 2;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row + 1][col + 1] == EMPTY && board[row + 2][col + 2] == enemyFlag && board[row + 3][col + 3] == enemyFlag && board[row + 4][col + 4] == enemyFlag)
            {
                bestMove.x = row + 1;
                bestMove.y = col + 1;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row + 1][col + 1] == enemyFlag && board[row + 2][col + 2] == EMPTY && board[row + 3][col + 3] == enemyFlag && board[row + 4][col + 4] == enemyFlag)
            {
                bestMove.x = row + 2;
                bestMove.y = col + 2;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col + 4 < BOARD_SIZE && board[row][col] == enemyFlag && board[row + 1][col + 1] == enemyFlag && board[row + 2][col + 2] == enemyFlag && board[row + 3][col + 3] == EMPTY && board[row + 4][col + 4] == enemyFlag)
            {
                bestMove.x = row + 3;
                bestMove.y = col + 3;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col - 4 >= 0 && board[row][col] == enemyFlag && board[row + 1][col - 1] == EMPTY && board[row + 2][col - 2] == enemyFlag && board[row + 3][col - 3] == enemyFlag && board[row + 4][col - 4] == enemyFlag)
            {
                bestMove.x = row + 1;
                bestMove.y = col - 1;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col - 4 >= 0 && board[row][col] == enemyFlag && board[row + 1][col - 1] == enemyFlag && board[row + 2][col - 2] == EMPTY && board[row + 3][col - 3] == enemyFlag && board[row + 4][col - 4] == enemyFlag)
            {
                bestMove.x = row + 2;
                bestMove.y = col - 2;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col - 4 >= 0 && board[row][col] == enemyFlag && board[row + 1][col - 1] == enemyFlag && board[row + 2][col - 2] == enemyFlag && board[row + 3][col - 3] == EMPTY && board[row + 4][col - 4] == enemyFlag)
            {
                bestMove.x = row + 3;
                bestMove.y = col - 3;
                return bestMove;
            }
        }
    }
    //�������У���turnΪ1��2ʱ������������Щλ��
    if (Turn == 1 && board[4][7] == enemyFlag)
    {
        bestMove.x = 7;
        bestMove.y = 4;
        return bestMove;
    }
    if (Turn == 1 && board[7][4] == enemyFlag)
    {
        bestMove.x = 4;
        bestMove.y = 7;
        return bestMove;
    }
    if (Turn == 2 && board[3][8] == enemyFlag && board[4][7] == enemyFlag)
    {
        bestMove.x = 2;
        bestMove.y = 9;
        return bestMove;
    }
    if (Turn == 2 && board[8][3] == enemyFlag && board[7][4] == enemyFlag)
    {
        bestMove.x = 9;
        bestMove.y = 2;
        return bestMove;
    }
    //�з����+һ���������ʱҪ�ο��ҷ��Ƿ���ڻ��+һ�����
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (col + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row][col + 1] == enemyFlag && board[row][col + 2] == EMPTY && board[row][col + 3] == enemyFlag && board[row][col + 4] == enemyFlag && board[row][col + 5] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row;
                bestMove.y = col + 2;
                return bestMove;
            }
            if (col + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row][col + 1] == enemyFlag && board[row][col + 2] == enemyFlag && board[row][col + 3] == EMPTY && board[row][col + 4] == enemyFlag && board[row][col + 5] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row;
                bestMove.y = col + 3;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col] == enemyFlag && board[row + 2][col] == EMPTY && board[row + 3][col] == enemyFlag && board[row + 4][col] == enemyFlag && board[row + 5][col] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row + 2;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col] == enemyFlag && board[row + 2][col] == enemyFlag && board[row + 3][col] == EMPTY && board[row + 4][col] == enemyFlag && board[row + 5][col] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row + 3;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col + 1] == enemyFlag && board[row + 2][col + 2] == EMPTY && board[row + 3][col + 3] == enemyFlag && board[row + 4][col + 4] == enemyFlag && board[row + 5][col + 5] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row + 2;
                bestMove.y = col + 2;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col + 5 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col + 1] == enemyFlag && board[row + 2][col + 2] == enemyFlag && board[row + 3][col + 3] == EMPTY && board[row + 4][col + 4] == enemyFlag && board[row + 5][col + 5] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row + 3;
                bestMove.y = col + 3;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col - 5 >= 0 && board[row][col] == EMPTY && board[row + 1][col - 1] == enemyFlag && board[row + 2][col - 2] == EMPTY && board[row + 3][col - 3] == enemyFlag && board[row + 4][col - 4] == enemyFlag && board[row + 5][col - 5] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row + 2;
                bestMove.y = col - 2;
                return bestMove;
            }
            if (row + 5 < BOARD_SIZE && col - 5 >= 0 && board[row][col] == EMPTY && board[row + 1][col - 1] == enemyFlag && board[row + 2][col - 2] == enemyFlag && board[row + 3][col - 3] == EMPTY && board[row + 4][col - 4] == enemyFlag && board[row + 5][col - 5] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row + 3;
                bestMove.y = col - 3;
                return bestMove;
            }
        }
    }
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            if (col + 4 < BOARD_SIZE && board[row][col] == EMPTY && board[row][col + 1] == enemyFlag && board[row][col + 2] == enemyFlag && board[row][col + 3] == enemyFlag && board[row][col + 4] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col] == enemyFlag && board[row + 2][col] == enemyFlag && board[row + 3][col] == enemyFlag && board[row + 4][col] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col + 4 < BOARD_SIZE && board[row][col] == EMPTY && board[row + 1][col + 1] == enemyFlag && board[row + 2][col + 2] == enemyFlag && board[row + 3][col + 3] == enemyFlag && board[row + 4][col + 4] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row;
                bestMove.y = col;
                return bestMove;
            }
            if (row + 4 < BOARD_SIZE && col - 4 >= 0 && board[row][col] == EMPTY && board[row + 1][col - 1] == enemyFlag && board[row + 2][col - 2] == enemyFlag && board[row + 3][col - 3] == enemyFlag && board[row + 4][col - 4] == EMPTY)
            {
                for (int i = 0; i < BOARD_SIZE; i++)
                {
                    for (int j = 0; j < BOARD_SIZE; j++)
                    {
                        //����ҷ��л��+һ�����
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
                        {
                            bestMove.x = i;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j + 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j + 4;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 1;
                            bestMove.y = j - 1;
                            return bestMove;
                        }
                        if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
                        {
                            bestMove.x = i + 4;
                            bestMove.y = j - 4;
                            return bestMove;
                        }
                    }
                }
                bestMove.x = row + 4;
                bestMove.y = col - 4;
                return bestMove;
            }

        }
    }
    //���з�û���������ʱ���ٿ�һ���ҷ��Ƿ��л��+һ�����
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == EMPTY && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == myFlag && board[i][j + 5] == EMPTY)
            {
                bestMove.x = i;
                bestMove.y = j + 1;
                return bestMove;
            }
            if (j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i][j + 1] == myFlag && board[i][j + 2] == myFlag && board[i][j + 3] == myFlag && board[i][j + 4] == EMPTY && board[i][j + 5] == EMPTY)
            {
                bestMove.x = i;
                bestMove.y = j + 4;
                return bestMove;
            }
            if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == EMPTY && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == myFlag && board[i + 5][j] == EMPTY)
            {
                bestMove.x = i + 1;
                bestMove.y = j;
                return bestMove;
            }
            if (i + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j] == myFlag && board[i + 2][j] == myFlag && board[i + 3][j] == myFlag && board[i + 4][j] == EMPTY && board[i + 5][j] == EMPTY)
            {
                bestMove.x = i + 4;
                bestMove.y = j;
                return bestMove;
            }
            if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == EMPTY && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == myFlag && board[i + 5][j + 5] == EMPTY)
            {
                bestMove.x = i + 1;
                bestMove.y = j + 1;
                return bestMove;
            }
            if (i + 5 < BOARD_SIZE && j + 5 < BOARD_SIZE && board[i][j] == EMPTY && board[i + 1][j + 1] == myFlag && board[i + 2][j + 2] == myFlag && board[i + 3][j + 3] == myFlag && board[i + 4][j + 4] == EMPTY && board[i + 5][j + 5] == EMPTY)
            {
                bestMove.x = i + 4;
                bestMove.y = j + 4;
                return bestMove;
            }
            if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == EMPTY && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == myFlag && board[i + 5][j - 5] == EMPTY)
            {
                bestMove.x = i + 1;
                bestMove.y = j - 1;
                return bestMove;
            }
            if (i + 5 < BOARD_SIZE && j - 5 >= 0 && board[i][j] == EMPTY && board[i + 1][j - 1] == myFlag && board[i + 2][j - 2] == myFlag && board[i + 3][j - 3] == myFlag && board[i + 4][j - 4] == EMPTY && board[i + 5][j - 5] == EMPTY)
            {
                bestMove.x = i + 4;
                bestMove.y = j - 4;
                return bestMove;
            }
        }
    }
    int depth = 0;
    int Max_Eval = INT_MIN;
    int Max_Depth = 0;
    evaluationValue = Sum_evaluation(board);        //���̴�ʱ�ķ�������
    int eval;
    int Origin_EvaluationValue = evaluationValue;   //�����̴�ʱ�ķ�������Ϊ�������
    for (MAX_DEPTH = 3; MAX_DEPTH <= 3; MAX_DEPTH += 2)
    {
        for (unsigned int i = 0; i < Commands_Vector->size(); i++)  //����Commands_Vector�����п��ܵ���һ������
        {
            Command c = (*Commands_Vector)[i].command;
            evaluationValue = Origin_EvaluationValue + Change_evaluation(c, myFlag);    //���µķ���=�������+����֮��ķ���
            putchess(c, myFlag);        //��������
            if (Have_Five_InBoard(board) == 1)                                          //����ҷ���������
                eval = INT_MAX;
            else
                eval = alphaBetapruning(depth + 1, alpha, beta, 3 - MaxMinPlayer);      //�������������һ��
            unputchess(c);              //��������
            if (eval > Max_Eval && MAX_DEPTH >= Max_Depth)
            {
                Max_Eval = eval;
                bestMove = c;           //������ӵ㼴Ϊ��õ�ѡ��
                Max_Depth = MAX_DEPTH;
            }
            if (Max_Eval == INT_MAX)
                break;
            (*Commands_Vector)[i].This_Command_Value = eval;
        }
        sort(Commands_Vector->begin(), Commands_Vector->end(), Cmp_Max);    //����
    }
    delete Commands_Vector;
    return bestMove;
}
void turn()
{
    int Startttime = clock();        //��ʼʱ�䣬���ύ����վʱ����ʾ
    Turn++;
    struct Command bestCommand = Get_Bestmove(INT_MIN + 1, INT_MAX, myFlag);
    putchess(bestCommand, myFlag);
    printf("%d %d", bestCommand.x, bestCommand.y);
    printf("\n");
    fflush(stdout);
    Print_Board();                   //��ӡ���̺��������ύ����վʱ����ʾ
    int Endtime = clock();           //����ʱ�䣬���ύ����վʱ����ʾ
    cout << Endtime - Startttime << endl;     //�����ʱ�����ύ����վʱ����ʾ
}
void end(int x)
{
    exit(0);
}
void loop()
{
    char tag[10] = { 0 };
    Command command;
    command.x = command.y = 0;
    int status;
    while (TRUE) {
        memset(tag, 0, sizeof(tag));
        scanf("%s", tag);
        if (strcmp(tag, START) == 0)
        {
            scanf("%d", &myFlag);
            start(myFlag);
            printf("OK\n");
            fflush(stdout);
        }
        else if (strcmp(tag, PLACE) == 0)
        {
            scanf("%d %d", &command.x, &command.y);
            place(command);
        }
        else if (strcmp(tag, TURN) == 0)
            turn();
        else if (strcmp(tag, END) == 0)
        {
            scanf("%d", &status);
            end(status);
        }
    }
}
int main()
{
    loop();
    return 0;
}