#ifndef SUDOKU_H
#define SUDOKU_H

#include<vector>
#include<cstdlib>
#include<set>
#include "solve.h"

typedef std::vector<int> firstdim;
typedef std::vector<firstdim> secdim;

class Sudoku
{
public:
    explicit Sudoku(int a);
    ~Sudoku(){}
    
    firstdim& operator[] (int a) {return secnum[a];}
    secdim& getresult() {return result;}
    std::pair<int ,int> help();
    
    bool wrong_occur(int a,int b);
    // bool check_win();
    
    
private:
    int difficulty;
    int tempcheck;
    secdim secnum;
    secdim result;
    
    
    void initialize();//Initialize the Sudoku game.
    //five initialize algorithm
    void change_value(int l,int r);
    void swap_col(int l,int r);
    void swap_row(int l,int r);
    
    //int checkifsole(int a,int b);
    
    
};



#endif // SUDOKU_H
