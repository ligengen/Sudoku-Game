#ifndef SOLVE_H
#define SOLVE_H

#include<vector>
#include<set>

typedef std::vector<int> vint;
typedef std::vector<vint> vvint;
typedef std::set<int> sint;
class Solve
{
public:
    Solve(vvint &);
    int beginCrack();//开始求解
    static vvint result;
    vvint vvnum;
    const vvint& getResult();
private:
    bool checkwin();
    int selectBetter();//选择最优点
    int countLackNum(int i, int j);//统计x,y对应坐标的可能数目
    sint& getLackedNumSet( int i, int j );
    int attemptLocate( int i, int j);//对最优点进行猜

   // vvint vvnum;//0是没有数字，1-9为正常数字

    static int times;
};

#endif
