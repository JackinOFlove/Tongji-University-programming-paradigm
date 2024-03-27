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
int Turn = 0;                                  //轮次计数
int myFlag;
int enemyFlag;
char board[BOARD_SIZE][BOARD_SIZE] = { 0 };
struct Command
{
    int x;
    int y;
};
//定义结构体：下一个落子点
struct Next_Command
{
    struct Command command;         //下一个落子点坐标
    int This_Command_Value;         //这个落子点的评估分数
};  
//定义结构体：棋型
struct chessshape
{
    int score;                      //这种棋型的分数
    char shape[9];                  //这种棋型的形状
};
//30种棋型的分数和形状定义
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
//尝试放置棋子
void putchess(struct Command cmd, int flag)
{
    board[cmd.x][cmd.y] = flag;
}
//拿掉棋子
void unputchess(struct Command cmd)
{
    board[cmd.x][cmd.y] = EMPTY;
}
//打印键盘函数（提交到网站时不显示）
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
//判断当前棋局是否含有五子连珠
int Have_Five_InBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            //如果我方有五子连珠，return 1
            if (col + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row][col + 1] == myFlag && board[row][col + 2] == myFlag && board[row][col + 3] == myFlag && board[row][col + 4] == myFlag)
                return 1;
            else if (row + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col] == myFlag && board[row + 2][col] == myFlag && board[row + 3][col] == myFlag && board[row + 4][col] == myFlag)
                return 1;
            else if (row + 4 < BOARD_SIZE && col + 4 < BOARD_SIZE && board[row][col] == myFlag && board[row + 1][col + 1] == myFlag && board[row + 2][col + 2] == myFlag && board[row + 3][col + 3] == myFlag && board[row + 4][col + 4] == myFlag)
                return 1;
            else if (row < BOARD_SIZE && row - 4 >= 0 && col < BOARD_SIZE && col - 4 >= 0 && board[row][col] == myFlag && board[row - 1][col - 1] == myFlag && board[row - 2][col - 2] == myFlag && board[row - 3][col - 3] == myFlag && board[row - 4][col - 4] == myFlag)
                return 1;
            //如果敌方有五子连珠，return -1
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
    //棋盘没有五子连珠，return 0
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
//排序函数，按照下一步要走的多种选择的分数高低排序
bool Cmp_Max(Next_Command choiceA, Next_Command choiceB)
{
    return choiceA.This_Command_Value > choiceB.This_Command_Value;
}
//计算当前棋盘各棋型中每一种棋型的个数（各类棋型在上面定义好了）
int Cal_Substr_Number(char* str, char* sub_str)
{
    char* str_local = NULL;
    char* sub_str_local = NULL;
    int num = 0;
    if (!str || !sub_str)           //如果都不存在，return 0
        return 0;  
    while (*str)
    {
        str_local = str;
        sub_str_local = sub_str;
        do
        {
            if (*sub_str_local == '\0')     //比对成功
            {
                num++;
                break;
            }
        } while (*str_local++ == *sub_str_local++);
        str += 1;                   //从0开始计数，逐步计数棋型数量
    }
    return num;
}
//从四个方向将棋盘转化相应的字符串
char* Transform(int type, int x, int y)
{
    int i;
    static char result[BOARD_SIZE + 1];     //定义返回的字符串数组
    //横向
    if (type == 1)
    {
        for (i = 0; i < BOARD_SIZE; i++)
            result[i] = board[x][i] + '0';
        result[i] = '\0';
        return result;
    }
    //纵向
    if (type == 2)
    {
        for (i = 0; i < BOARD_SIZE; i++)
            result[i] = board[i][y] + '0';
        result[i] = '\0';
        return result;
    }
    //主对角线
    if (type == 3)
    {
        for (i = 0; (i + x < BOARD_SIZE && i + y < BOARD_SIZE); i++)
            result[i] = board[x + i][y + i] + '0';
        result[i] = '\0';
        return result;
    }
    //副对角线
    if (type == 4)
    {
        for (i = 0; (x + i < BOARD_SIZE && y - i >= 0); i++)
            result[i] = board[x + i][y - i] + '0';
        result[i] = '\0';
        return result;
    }
}
//计算某一种棋型在该棋盘的总分数
int Score_evaluation(char* str)
{
    int sum = 0;
    int subsumvalue = 0;
    for (int i = 0; i < 30; i++)
    {
        subsumvalue = scoreof_eachshape[i].score * Cal_Substr_Number(str, scoreof_eachshape[i].shape);      //该类棋型总分数=该类棋型数量*该类棋型分数
        if (myFlag == 2)                        //如果敌方先手，分数转换
            subsumvalue = -subsumvalue;
        sum += subsumvalue;
    }
    return sum;
}
//计算该棋盘的总分数（各类棋型的数量*对应棋型的分数）
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
    return sum;             //获得当前局面的总分数
}
//尝试落子，获得这个落子位置后改变的分数
int Change_evaluation(struct Command cmd, int player)
{
    int sumvalue = 0;
    char* temp;
    putchess(cmd, player);                  //尝试下子
    if (Have_Five_InBoard(board))           //如果下了这个棋子后有五子连珠
    {
        if (player == myFlag)               //如果是我方五子连珠
            sumvalue = INT_MAX;             //分数极大
        else                                //如果是敌方五子连珠
            sumvalue = INT_MIN;             //分数极小
        unputchess(cmd);                    //撤销落子
    }
    else
    {
        unputchess(cmd);                    //撤销落子
        // 横向
        temp = Transform(1, cmd.x, 0);      //横向棋型改变
        sumvalue -= Score_evaluation(temp);
        putchess(cmd, player);              //尝试落子
        temp = Transform(1, cmd.x, 0);
        sumvalue += Score_evaluation(temp);
        unputchess(cmd);                    //撤销落子
        // 纵向
        temp = Transform(2, 0, cmd.y);      //纵向棋型改变
        sumvalue -= Score_evaluation(temp);
        putchess(cmd, player);              //尝试落子
        temp = Transform(2, 0, cmd.y);
        sumvalue += Score_evaluation(temp);
        unputchess(cmd);                    //撤销落子
        // 主对角线方向
        int min_xy = min(cmd.x, cmd.y);
        temp = Transform(3, cmd.x - min_xy, cmd.y - min_xy);    //主对角线棋型改变
        sumvalue -= Score_evaluation(temp);
        putchess(cmd, player);              //尝试落子
        temp = Transform(3, cmd.x - min_xy, cmd.y - min_xy);
        sumvalue += Score_evaluation(temp);
        unputchess(cmd);                    //撤销落子
        // 副对角线方向
        min_xy = min(cmd.x, BOARD_SIZE - cmd.y - 1);
        temp = Transform(4, cmd.x - min_xy, cmd.y + min_xy);    //副对角线棋型改变
        sumvalue -= Score_evaluation(temp);
        putchess(cmd, player);              //尝试落子
        temp = Transform(4, cmd.x - min_xy, cmd.y + min_xy);
        sumvalue += Score_evaluation(temp);
        unputchess(cmd);                    //撤销落子
    }
    return sumvalue;            //计算并返回改变的分数
}
//计算落子分数
int Command_evaluation(struct Command command, int player)
{
    int Five_chess = 0, huosi = 0, miansi = 0, huosan = 0, miansan = 0, erzi = 0;     //五子连珠、活四、眠四、活三、眠三和二子的计数
    // 横向左侧判断
    int leftSpace = 0, rightSpace = 0, number = 1;                              //定义左空格数量、右空格数量和棋子数
    for (int i = 1, Space_Flag = 0; isInBound(command.x - i, command.y) && board[command.x - i][command.y] != 3 - player; i++)  //Space_Flag用于判断“眠”与“活”
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
    // 横向右侧判断
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
    //横向各类棋子计数
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
    //纵向左侧判断
    //重置leftSpace、rightSpace和number
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
    //纵向右侧判断
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
    //纵向各类棋子计数
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
    //主对角线左侧判断
    //重置leftSpace、rightSpace和number
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
    //主对角线右侧判断
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
    //主对角线各类棋子计数
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
    //副对角线左侧判断
    //重置leftSpace、rightSpace和number
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
    //副对角线右侧判断
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
    //副对角线各类计数
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
    //总体的评分
    if (Five_chess)
        return INT_MAX;                              //如果有五子连珠
    if (huosi)
        return 99999999;                            //如果有活四   
    return miansi * 1000 + huosan * 1000 + miansan * 200 + erzi * 200;      //其余按权重给分
}
//获得一个装有下一步可能落子点的vector容器的函数
//鉴于下一步的落子点的周围一般都是有棋子的，就是不会离别的棋子太远，所以我将搜索范围缩小为有棋子处的半径为2的范围里
//这样能减少搜索的时间
vector<Next_Command>* Next_Commands_Vector(int player)
{
    int Search_r = 2;                                //搜索半径为2
    bool Point_Flag[BOARD_SIZE][BOARD_SIZE];         //这个二维数组用来判断该点是否被搜索过，如果搜索过就不再搜索，这样也能减少时间
    for (int row = 0; row < BOARD_SIZE; row++)       //初始化Point_Flag二维数组，开始时都为false，表示都没有被搜索过
    {
        for (int col = 0; col < BOARD_SIZE; col++)
            Point_Flag[row][col] = false;
    }
    vector<Next_Command>* Commands_Vector = new vector<Next_Command>;   //分配新的空间
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] != EMPTY)                                   //如果这个位置是空的话
            {
                for (int k = -Search_r; k <= Search_r; k++)
                {
                    if (k == 0)                                         //该点不可能落子，直接跳过
                    {
                        k++;
                        continue;
                    }
                    if (isInBound(i + k, j) && !Point_Flag[i + k][j] && board[i + k][j] == EMPTY)   //横向
                    {  
                        Point_Flag[i + k][j] = TRUE;        //此点标为以搜索过
                        Next_Command c;                     //下一步的可能落子选择
                        c.command.x = i + k;                //赋值x
                        c.command.y = j;                    //赋值y
                        c.This_Command_Value = Command_evaluation(c.command, player);   //计算该选择的分数
                        Commands_Vector->push_back(c);      //将这个选择放入到装有下一步可能落子点的vector容器Commands_Vector中
                    }
                    if (isInBound(i, j + k) && !Point_Flag[i][j + k] && board[i][j + k] == EMPTY)   //纵向
                    {  
                        Point_Flag[i][j + k] = TRUE;        //此点标为以搜索过
                        Next_Command c;                     //下一步的可能落子选择
                        c.command.x = i;                    //赋值x
                        c.command.y = j + k;                //赋值y
                        c.This_Command_Value = Command_evaluation(c.command, player);   //计算该选择的分数
                        Commands_Vector->push_back(c);      //将这个选择放入到装有下一步可能落子点的vector容器Commands_Vector中
                    }
                    if (isInBound(i + k, j + k) && !Point_Flag[i + k][j + k] && board[i + k][j + k] == EMPTY)   //主对角线方向
                    {  
                        Point_Flag[i + k][j + k] = TRUE;    //此点标为以搜索过
                        Next_Command c;                     //下一步的可能落子选择
                        c.command.x = i + k;                //赋值x
                        c.command.y = j + k;                //赋值y
                        c.This_Command_Value = Command_evaluation(c.command, player);    //计算该选择的分数
                        Commands_Vector->push_back(c);      //将这个选择放入到装有下一步可能落子点的vector容器Commands_Vector中
                    }
                    if (isInBound(i + k, j - k) && !Point_Flag[i + k][j - k] && board[i + k][j - k] == EMPTY)   //副对角线方向
                    {  
                        Point_Flag[i + k][j - k] = TRUE;    //此点标为以搜索过
                        Next_Command c;                     //下一步的可能落子选择
                        c.command.x = i + k;                //赋值x
                        c.command.y = j - k;                //赋值y
                        c.This_Command_Value = Command_evaluation(c.command, player);   //计算该选择的分数
                        Commands_Vector->push_back(c);      //将这个选择放入到装有下一步可能落子点的vector容器Commands_Vector中
                    }
                }
            }
        }
    }
    sort(Commands_Vector->begin(), Commands_Vector->end(), Cmp_Max);    //用排序函数将这些选择按照评估分数的大小排序，可以再后续减少时间
    return Commands_Vector;
}
int MAX_DEPTH;          //定义最大搜索深度
int evaluationValue;    //定义总分
//alphaBeta剪枝函数
int alphaBetapruning(int depth, int alpha, int beta, int MaxMinPlayer)
{
    int eval; 
    int Origin_EvaluationValue = evaluationValue;   //将棋盘此时的分数定义为最初分数
    if (MaxMinPlayer == myFlag)                     //如果我方是MaxPlayer
    {
        auto Commands_Vector = Next_Commands_Vector(MaxMinPlayer);
        for (Next_Command& node : *Commands_Vector)
        {
            struct Command c = node.command;
            if (depth == MAX_DEPTH - 1)
            {
                {
                    eval = Change_evaluation(c, myFlag);    //eval分数为放置这个command改变的分数
                    if (eval != INT_MAX && eval != INT_MIN)
                        eval += evaluationValue;            //如果没有五子连珠（eval != INT_MAX 且 eval != INT_MIN），计入分数
                }
            }
            else
            {
                int Plus_Value = Change_evaluation(c, myFlag);
                if (Plus_Value == INT_MAX)                  //如果额外分数极大（我方五子连珠）
                {
                    alpha = INT_MAX;                        //alpha即为无穷大，跳出
                    break;
                }
                else
                {
                    evaluationValue = Origin_EvaluationValue + Plus_Value;  //其他情况，加入总分
                    putchess(c, myFlag);                    //尝试落子
                    if (Have_Five_InBoard(board))
                        eval = INT_MAX;
                    else
                        eval = alphaBetapruning(depth + 1, alpha, beta, 3 - MaxMinPlayer);  //递归搜索下面一层
                    unputchess(c);                            //撤销落子
                }
            }
            alpha = alpha > eval ? alpha : eval;
            if (beta <= alpha)           //beta剪枝
            {
                delete Commands_Vector;
                return beta;
            }
        }
        delete Commands_Vector;
        return alpha;
    }
    else       //如果我方是MinPlayer
    {
        auto Commands_Vector = Next_Commands_Vector(enemyFlag);
        for (Next_Command& node : *Commands_Vector)
        {
            Command c = node.command;
            if (depth == MAX_DEPTH - 1)
            {
                eval = Change_evaluation(c, myFlag);            //eval分数为放置这个command改变的分数
                if (eval != INT_MIN && eval != INT_MAX)         //如果没有五子连珠（eval != INT_MAX 且 eval != INT_MIN），计入分数
                    eval += evaluationValue;
            }
            else
            {
                int Plus_Value = Change_evaluation(c, enemyFlag); 
                if (Plus_Value == INT_MIN)                          //如果额外分数极小（敌方五子连珠）
                {
                    beta = INT_MIN;                                 //beta即为无穷大，跳出
                    break;
                }
                else
                {
                    evaluationValue = Origin_EvaluationValue + Plus_Value;  //其他情况，加入总分
                    putchess(c, enemyFlag);         //尝试落子
                    if (Have_Five_InBoard(board))
                        eval = INT_MIN;
                    else eval = alphaBetapruning(depth + 1, alpha, beta, 3 - MaxMinPlayer); //递归搜索下面一层
                    unputchess(c);                  //撤销落子
                }
            }
            beta = beta < eval ? beta : eval;
            if (beta <= alpha)              //alpha剪枝
            {
                delete Commands_Vector;
                return alpha;
            }
        }
        delete Commands_Vector;
        return beta;
    }
}
//得到最终下子位置的函数
Command Get_Bestmove(int alpha, int beta, int MaxMinPlayer)
{
    vector<Next_Command>* Commands_Vector = Next_Commands_Vector(MaxMinPlayer);
    Command bestMove = (*Commands_Vector)[0].command;
    //一些特殊棋局判断
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            //我方眠四，直接下
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
            //我方活四，直接下
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
            //我方三+一，直接下
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
    //敌方眠四和敌方三+一，必须堵
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
    //开局特判，当turn为1或2时，必须下在那些位置
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
    //敌方活二+一与活三，此时要参考我方是否存在活二+一与活三
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
                        //如果我方有活二+一与活三
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
                        //如果我方有活二+一与活三
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
                        //如果我方有活二+一与活三
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
                        //如果我方有活二+一与活三
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
                        //如果我方有活二+一与活三
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
                        //如果我方有活二+一与活三
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
                        //如果我方有活二+一与活三
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
                        //如果我方有活二+一与活三
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
                        //如果我方有活二+一与活三
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
                        //如果我方有活二+一与活三
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
                        //如果我方有活二+一与活三
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
                        //如果我方有活二+一与活三
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
    //当敌方没有上述情况时，再看一下我方是否有活二+一与活三
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
    evaluationValue = Sum_evaluation(board);        //棋盘此时的分数计算
    int eval;
    int Origin_EvaluationValue = evaluationValue;   //将棋盘此时的分数定义为最初分数
    for (MAX_DEPTH = 3; MAX_DEPTH <= 3; MAX_DEPTH += 2)
    {
        for (unsigned int i = 0; i < Commands_Vector->size(); i++)  //遍历Commands_Vector中所有可能的下一步落子
        {
            Command c = (*Commands_Vector)[i].command;
            evaluationValue = Origin_EvaluationValue + Change_evaluation(c, myFlag);    //最新的分数=最初分数+落子之后的分数
            putchess(c, myFlag);        //尝试落子
            if (Have_Five_InBoard(board) == 1)                                          //如果我方五子连珠
                eval = INT_MAX;
            else
                eval = alphaBetapruning(depth + 1, alpha, beta, 3 - MaxMinPlayer);      //否则继续搜索下一层
            unputchess(c);              //撤销落子
            if (eval > Max_Eval && MAX_DEPTH >= Max_Depth)
            {
                Max_Eval = eval;
                bestMove = c;           //这个落子点即为最好的选择
                Max_Depth = MAX_DEPTH;
            }
            if (Max_Eval == INT_MAX)
                break;
            (*Commands_Vector)[i].This_Command_Value = eval;
        }
        sort(Commands_Vector->begin(), Commands_Vector->end(), Cmp_Max);    //排序
    }
    delete Commands_Vector;
    return bestMove;
}
void turn()
{
    int Startttime = clock();        //开始时间，在提交至网站时不显示
    Turn++;
    struct Command bestCommand = Get_Bestmove(INT_MIN + 1, INT_MAX, myFlag);
    putchess(bestCommand, myFlag);
    printf("%d %d", bestCommand.x, bestCommand.y);
    printf("\n");
    fflush(stdout);
    Print_Board();                   //打印棋盘函数，在提交至网站时不显示
    int Endtime = clock();           //结束时间，在提交至网站时不显示
    cout << Endtime - Startttime << endl;     //输出用时，在提交至网站时不显示
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