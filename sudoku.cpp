#include "sudoku.h"
#include "solve.h"
#include<random>
#include<ctime>
#include<set>


static std::default_random_engine e(time(0));
Sudoku::Sudoku(int d):difficulty(d),secnum({
                {8,7,4,6,3,1,5,9,2},
                {5,9,6,7,2,8,4,3,1},
                {2,3,1,4,5,9,6,8,7},
                {4,8,2,1,9,6,7,5,3},
                {7,6,5,3,8,4,2,1,9},
                {9,1,3,5,7,2,8,4,6},
                {3,2,9,8,6,5,1,7,4},
                {1,5,7,2,4,3,9,6,8},
                {6,4,8,9,1,7,3,2,5}})
{
    initialize();
}


void Sudoku::change_value(int l,int r)
{
    for(firstdim & mm :secnum)
        for(int &a :mm)
        {
            if(a == l)
                a = r;
            else if(a == r)
                a = l;
            else
                a = a;
        }
}
void Sudoku::swap_col(int l,int r)
{
    for(int i=0;i<9;i++)
    {
        using std::swap;
        swap(secnum[i][l],secnum[i][r]);
    }
}
void Sudoku::swap_row(int l,int r)
{
    using std::swap;
    swap(secnum[l],secnum[r]);
}

//数独问题产生器
void Sudoku::initialize()
{
    srand((unsigned)time(NULL));
    for(int i=0;i<100;i++)
    {
        int mm=5;
        while(mm)
        {
            change_value(rand()%9+1,rand()%9+1);
            mm--;
        }

        for(int j=0;j<8;j+=3)
        {
            swap_row(rand()%3+j,rand()%3+j);
        }
        for(int k=0;k<8;k+=3)
        {
            swap_col(rand()%3+k,rand()%3+k);
        }
    }
}



std::pair<int ,int> Sudoku::help()
{
    srand((unsigned)time(NULL));
    int i,j;
    while(1)
    {
        i=rand()%9,j=rand()%9;
        if(!secnum[i][j])
            break;
    }
    secnum[i][j] = result[i][j];
    return std::make_pair(i,j);
}

bool Sudoku::wrong_occur(int a, int b)
{
    for(int i=0;i<9;i++)
    {
        if(i==b)
            continue;
        if(secnum[a][i]==secnum[a][b])
            return 1;
    }
    for(int i=0;i<9;i++)
    {
        if(i==a)
            continue;
        if(secnum[i][b]==secnum[a][b])
            return 1;
    }
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            if( (i/3) == (a/3) && (j/3) == (b/3) )
            {
                if(i==a&&j==b)
                    continue;
                if(secnum[i][j]==secnum[a][b])
                    return 1;
            }
        }
    }
    return 0;
}
