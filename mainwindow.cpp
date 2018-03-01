#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <cstdlib>
#include <QPen>
#include <QRadioButton>
#include <QSize>
#include <QLabel>
#include <QFont>
#include "sudoku.h"
#include <QRadialGradient>
#include <cmath>
#include <QColor>
#include <QSignalMapper>
#include <random>
#include <set>
#include <QSizePolicy>
#include <cstring>
#include <QPalette>
#include <QFont>
#include <QBrush>
#include <QSound>
#include "solve.h"
bool __1[9] ={1};
bool __2[9] ={1};
static int nummm ;
static int step = 0;
static int step_per = 0;
static bool flaggg = 1;
static int _x,_y;
bool ddd[9]={1,};
static int __step = 0;
Sudoku* Game = new Sudoku(1);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("SUDOKU CHALLENGE");
    difficulty_choose = new QWidget;
    button = new QPushButton("Confirm",difficulty_choose);
    QLabel* label = new QLabel("<h2>Please choose the level:</h2><b>NOTE: 1 for the easiest.</b>",difficulty_choose);
    QHBoxLayout* HHHLayout = new QHBoxLayout;
    HHHLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Preferred,QSizePolicy::Preferred));
    QGridLayout* diffiLayout = new QGridLayout;

    radiobutton = new QRadioButton*[10];

    for(int i=0;i<10;i++)
    {
        QString num = QString::number(i+1);
        radiobutton[i] = new QRadioButton("LEVEL  "+num,difficulty_choose);
        diffiLayout->addWidget(radiobutton[i]);
    }
    HHHLayout->addLayout(diffiLayout);
    QVBoxLayout* VLayout = new QVBoxLayout;
    VLayout->addWidget(label);
    VLayout->addLayout(HHHLayout);
    difficulty_choose->setLayout(VLayout);

    QHBoxLayout* hLayout = new QHBoxLayout;
    QSpacerItem* item = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding);
    hLayout->addSpacerItem(item);
    hLayout->addWidget(button);
    VLayout->addLayout(hLayout);
    button->setEnabled(0);
    QSignalMapper* m = new QSignalMapper(this);
    for(int ii=0;ii<10;ii++)
    {
        connect(radiobutton[ii],SIGNAL(toggled(bool)),this,SIGNAL(radioButtonPressed()));
        connect(radiobutton[ii],SIGNAL(toggled(bool)),m,SLOT(map()));
        m->setMapping(radiobutton[ii],ii);
    }
    connect(m,SIGNAL(mapped(int)),this,SLOT(begin_game(int)));
    connect(this,SIGNAL(radioButtonPressed()),this,SLOT(setEnabled_button()));
    connect(button,SIGNAL(clicked(bool)),difficulty_choose,SLOT(close()));
    connect(button,SIGNAL(clicked(bool)),this,SLOT(Start_clicked_again()));

    _restore = new int**[1000];
    for (int i = 0; i < 1000; i ++)
    {
        _restore[i] = new int*[9];
        for (int j = 0; j < 9; j ++)
        {
            _restore[i][j] = new int[9];
        }
    }
    for(int i=0;i<1000;i++)
    {
        for(int j=0;j<9;j++)
        {
            for(int k=0;k<9;k++)
            {
                _restore[i][j][k] = 0;
            }
        }
    }

    __restore = new QString**[1000];
    for (int i = 0; i < 1000; i ++)
    {
        __restore[i] = new QString*[9];
        for (int j = 0; j < 9; j ++)
        {
            __restore[i][j] = new QString[9];
        }
    }
    for(int i=0;i<1000;i++)
    {
        for(int j=0;j<9;j++)
        {
            for(int k=0;k<9;k++)
            {
                __restore[i][j][k] = "";
            }
        }
    }
    //sudoku_widget
    sudoku_widget = new QWidget;
    ui->sudokuLayout->addWidget(sudoku_widget);
    b = new QPushButton** [9];
    for(int i = 0 ; i < 9 ; i++ )
    {
        b[i] = new QPushButton*[9];
        for(int j=0;j<9;j++)
        {
            b[i][j] = new QPushButton;
            b[i][j] ->setMinimumSize(10,10);
            ui->sudokuLayout->addWidget(b[i][j],i,j);
            b[i][j]->setAccessibleName(QString::number(i*10+j));
            b[i][j]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        }
    }
    ui->sudokuLayout->setMargin(10);
    ui->sudokuLayout->setVerticalSpacing(10);
    ui->sudokuLayout->setHorizontalSpacing(10);
    for(int i=0;i<9;i++)
    {
        ui->sudokuLayout->setColumnStretch(i,1);
        ui->sudokuLayout->setRowStretch(i,1);
    }

    connect(ui->actionHow_to_play,SIGNAL(triggered(bool)),this,SLOT(learn_how_to_play()));
    connect(ui->actionGet_Hints,SIGNAL(triggered(bool)),this,SLOT(provide_hints()));

    //Timer...
    isStart = false;
    timer = new QTimer;
    TimeRecord = new QTime(0,0,0);
    ui->lcdNumber->setDigitCount(8);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->display(TimeRecord->toString("hh:mm:ss"));

    ui->Button_again->setEnabled(0);
    connect(button,SIGNAL(clicked(bool)),this,SLOT(enableAgainButton()));
    connect(ui->Button_again,SIGNAL(clicked()),this,SLOT(Start_clicked_again()));
    connect(ui->Button_Pause,SIGNAL(clicked()),this,SLOT(Stop_clicked_pause()));
    connect(ui->Button_Pause,SIGNAL(clicked(bool)),this,SLOT(prohibited()));
    connect(ui->Button_conti,SIGNAL(clicked(bool)),this,SLOT(reEnable()));
    connect(ui->Button_conti,SIGNAL(clicked()),this,SLOT(Conti_clicked()));
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));


    //choose difficulty.
    connect(ui->NewGame,SIGNAL(triggered(bool)),this,SLOT(choose_difficulty()));
    connect(ui->Button_again,SIGNAL(clicked(bool)),this,SLOT(refresh()));

    connect(ui->NewGame,SIGNAL(triggered(bool)),this,SLOT(enableInputButton_2()));
    connect(ui->Button_again,SIGNAL(clicked(bool)),this,SLOT(enableInputButton_2()));

    QSignalMapper *n = new QSignalMapper(this);
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            connect(b[i][j],SIGNAL(clicked(bool)),n,SLOT(map()));
            n->setMapping(b[i][j],i*9+j);
        }
    }
    connect(n,SIGNAL(mapped(int)),this,SLOT(take_it_down(int)));

    inputButton = new QPushButton*[9];
    for(int i=0;i<9;i++)
    {
        inputButton[i] = new QPushButton;
    }
    QString ss;
    for(int i=0;i<3;i++)
        ui->horizontalLayout_3->addWidget(inputButton[i]);
    for(int i=3;i<6;i++)
        ui->horizontalLayout_4->addWidget(inputButton[i]);
    for(int i=6;i<9;i++)
        ui->horizontalLayout_5->addWidget(inputButton[i]);
    for(int i=0;i<9;i++)
    {
        inputButton[i]->setEnabled(0);
        inputButton[i]->setText(ss.number(i+1));
        inputButton[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    }
    QSignalMapper* ib = new QSignalMapper(this);
    for(int i=0;i<9;i++)
    {
        connect(inputButton[i],SIGNAL(clicked(bool)),ib,SLOT(map()));
        ib->setMapping(inputButton[i],i);
    }
    connect(ib,SIGNAL(mapped(int)),this,SLOT(take_it_down_2(int)));
    //connect(ib,SIGNAL(mapped(int)),this,SLOT(check_once(int)));
    //connect(ib,SIGNAL(mapped(int)),this,SLOT(check_twice(int)));


    connect(ui->delButton,SIGNAL(clicked(bool)),this,SLOT(delNum()));
    //
    connect(ui->repealButton,SIGNAL(clicked(bool)),this,SLOT(repealNum()));
    connect(ui->recoverButton,SIGNAL(clicked(bool)),this,SLOT(recoverNum()));
    connect(ui->markButton,SIGNAL(clicked(bool)),this,SLOT(mark()));

}

void MainWindow::take_it_down(int pos_focus)
{
    for(int i=0;i<9;i++)
        ddd[i] =1;
    pos_focused = pos_focus;
    sameHighlight();
    _x = pos_focused/9;
    _y = pos_focused%9;
    update();
}

void MainWindow::take_it_down_2(int inputButtonNumber)
{
    inputButtonNum = inputButtonNumber;
    show_press_button();
}

void MainWindow::enableAgainButton()
{
    ui->Button_again->setEnabled(1);
}

void MainWindow::show_press_button()
{
    if(clickable[pos_focused/9][pos_focused%9]==1)
    {

        /*QString ss="",bb;
        for(;;)
        {
            bb = QString::number(inputButtonNum+1);
            ss = ss+" "+bb;
            b[pos_focused/9][pos_focused-pos_focused/9*9]->setText(ss);
            for(int i=0;i<9;i++)
                for(int j=0;j<9;j++)
                    connect(b[i][j],SIGNAL(clicked(bool)),
        }       */

        bool _1=0,_2=0;
        if(b[pos_focused/9][pos_focused-pos_focused/9*9]->text()=="")
            _1 = 1;
        QString ss = b[pos_focused/9][pos_focused-pos_focused/9*9]->text();
        bool state[15];
        for (int i = 0; i < 9; i ++) state[i] = 0;
        for (int i = 0; i < ss.length(); i ++)
            if ('1' <= ss[i] && ss[i] <= '9')
            {
                char c = (ss.toStdString())[i];
                state[c - '1'] = ! state[c - '1'];
            }
        state[inputButtonNum] = ! state[inputButtonNum];
        int cnt = 0;
        ss = "";
        for (int i = 0; i < 9; i ++)
        {
            if (state[i])
            {
                if (cnt > 0 && cnt % 3 == 0) ss += "\n";
                ss += QString('1' + i);
                cnt ++;
            }

               //ss += "";
        }


        //ss += " " + QString::number(inputButtonNum+1);
/*
        if (ss.length()>=5&&ss.length()<8)
                    ss = ss + '\n';*/
        b[pos_focused/9][pos_focused-pos_focused/9*9]->setText(ss);

        sameHighlight();

        if(b[pos_focused/9][pos_focused-pos_focused/9*9]->text()!="")
            _2 = 1;

        step_per = __step;
        step_per++;
        __step ++;



        QPalette p;
        p.setColor(QPalette::ButtonText,QColor(0,0,0));
        b[pos_focused/9][pos_focused-pos_focused/9*9]->setPalette(p);

        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                //if(b[i][j]->text().length()<=1)
                {
                    __restore[step_per][i][j] = b[i][j]->text();
                }
            }
        }
       // step_per++;
    }

    flaggg = 1;
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            if(b[i][j]->text().length()==0)
            {
                flaggg = 0;
                break;
            }
        }
    }
    if(flaggg)
    {
        checkWin();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::learn_how_to_play()
{
    QMessageBox::about(NULL,tr("Learn How To Play"),tr("    The objective is to fill a 9×9 grid with digits so that each column,"
                            "each row, and each of the nine 3×3 subgrids that compose the grid "
                            "(also called 'boxes', 'blocks', or 'regions') contains all of the digits from 1 to 9. "
                            "The puzzle setter provides a partially completed grid, "
                            "which for a well-posed puzzle has a single solution.\n"
                            "    9×9 grids are divided into nine 3×3 grids:\n"
                            "3×3|3×3|3×3\n"
                            "3×3|3×3|3×3\n"
                            "3×3|3×3|3×3\n"));
}
void MainWindow::provide_hints()
{
    int* ans;
    ans = new int[81];
    int nu = 0;

    int m =solve->beginCrack();
    if(m==1)
    {
        const vvint &r = solve->getResult();
        for(auto _ans : r)
        {
             for(auto __ans:_ans)
             {
                 ans[nu++] = __ans;
             }
        }
        bool flag = 0;
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                if(b[i][j]->text()=="")
                {
                    b[i][j]->setText(QString::number(ans[i*9+j]));
                    flag = 1;
                    break;
                }
            }
            if(flag)
                break;
        }
        sameHighlight();
        step_per = __step;
        step_per++;
        __step ++;
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                //if(b[i][j]->text().length()<=1)
                {
                    __restore[step_per][i][j] = b[i][j]->text();
                }
            }
        }
    }
}
void MainWindow::updateTime()
{
    *TimeRecord = TimeRecord->addSecs(1);
    ui->lcdNumber->display(TimeRecord->toString("hh:mm:ss"));
}

void MainWindow::Start_clicked_again()
{
    QApplication::beep();
    TimeRecord->setHMS(0,0,0);
    timer->start();
}
void MainWindow::Stop_clicked_pause()
{
    QApplication::beep();
    timer->stop();
    ui->lcdNumber->display(TimeRecord->toString("hh:mm:ss"));
}
void MainWindow::Conti_clicked()
{
    QApplication::beep();
    timer->start();
}

void MainWindow::paintEvent(QPaintEvent*)
{
    int bm = ui->horizontalLayout->geometry().bottom();
    QPainter p(this);
    QRadialGradient g(this->width()/2,this->height()/2,sqrt(((this->width()*this->width())+(this->height()*this->height()))/2),this->width()/2,this->height()/2-20);

    g.setColorAt(0,Qt::red);
    g.setColorAt(1,Qt::yellow);
    p.setBrush(g);
    p.drawRect(0,0,this->width(),this->height());
    QPen ss;
    ss.setWidth(2);
    ss.setColor(Qt::yellow);
    p.setPen(ss);
    for(int i=3;i<8;i+=3)
    {
        int x1=(b[0][i]->geometry().left()+b[0][i-1]->geometry().right())/2;
        int y1=b[0][i]->geometry().top();
        int y2= b[8][i]->geometry().bottom();
        p.drawLine(x1,y1+bm,x1,y2+bm);
    }
    for(int i=3;i<8;i+=3)
    {
        int y1 = (b[i][0]->geometry().top()+b[i-1][0]->geometry().bottom())/2;
        int x1 = b[0][0]->geometry().left();
        int x2 = b[0][8]->geometry().right();
        p.drawLine(x1,y1+bm-1,x2,y1+bm-1);
    }
}

void MainWindow::choose_difficulty()
{
    difficulty_choose->show();
}
void MainWindow::enableInputButton_2()
{
    //qDebug("enable");
    for(int i=0;i<9;i++)
        this->inputButton[i]->setEnabled(1);
}

void MainWindow::setEnabled_button()
{
    this->button->setEnabled(1);
}

void MainWindow::begin_game(int a)
{
    Game = new Sudoku(a);
    Sudoku& game = *Game;
    clickable = vvbool(9,std::vector<bool>(9,1));
    QString ss;
    srand((unsigned)time(0));
    if(a==0)
    {
        for(int o=0;o<4;o++)
        {
            get_rand();
            for(int p=0;p<3;p++)
            {
                game[o][ran[p]]=0;
            }
        }
        for(int o=4;o<7;o++)
        {
            get_rand();
            for(int p=0;p<4;p++)
            {

                game[o][ran[p]]=0;
            }
        }
        get_rand();
        game[7][ran[0]]=game[7][ran[1]]=0;
        get_rand();
        for(int p=0;p<5;p++)
            game[8][ran[p]]=0;
       // game[1][4]=game[3][6]=game[2][7]=game[4][2]=game[6][3]=game[8][4]=0;
    }
    else if(a==1)
    {
        get_rand();
        game[0][ran[0]]=game[0][ran[1]]=0;
        get_rand();
        for(int p=0;p<3;p++)
        {
            game[1][ran[p]]=0;
            game[2][ran[p+3]]=0;
        }
        for(int p=3;p<7;p++)
        {
            get_rand();
            for(int o=0;o<4;o++)
            {

                game[p][ran[o]]=0;
            }
        }
        for(int o=7;o<9;o++)
        {
            get_rand();
            for(int p=0;p<5;p++)
            {
                game[o][ran[p]] = 0;
            }
        }
    }
    else if(a==2)
    {
        for(int o=0;o<2;o++)
        {
            get_rand();
            for(int p=0;p<3;p++)
            {
                game[o][ran[p]]=0;
            }
        }
        for(int o=2;o<5;o++)
        {
            get_rand();
            for(int p=0;p<4;p++)
            {
                game[o][ran[p]]=0;
            }
        }
        get_rand();
        game[5][ran[0]]=game[5][ran[1]]=0;
        get_rand();
        for(int p=0;p<5;p++)
            game[6][ran[p]]=0;
        for(int o=7;o<9;o++)
        {
            get_rand();
            for(int p=0;p<6;p++)
                game[o][ran[p]]=0;
        }
    }

    else if(a==3)
    {
        for(int o=0;o<2;o++)
        {
            get_rand();
            for(int p=0;p<3;p++)
            {
                game[o][ran[p]]=0;
            }
        }
        for(int o=2;o<5;o++)
        {
            get_rand();
            for(int p=0;p<4;p++)
            {
                game[o][ran[p]]=0;
            }
        }
        for(int o=5;o<7;o++)
        {
            get_rand();
            for(int p=0;p<5;p++)
            {
                game[o][ran[p]]=0;
            }
        }
        for(int o=7;o<9;o++)
        {
            get_rand();
            for(int p=0;p<6;p++)
                game[o][ran[p]]=0;
        }

    }

    else if(a==4)
    {
        for(int o=0;o<2;o++)
        {
            get_rand();
            for(int p=0;p<5;p++)
                game[o][ran[p]]=0;
        }
        for(int o=2;o<4;o++)
        {
            get_rand();
            for(int p=0;p<4;p++)
                game[o][ran[p]]=0;
        }
        for(int o=4;o<6;o++)
        {
            get_rand();
            for(int p=0;p<3;p++)
                game[o][ran[p]]=0;
        }
        get_rand();
        for(int o=0;o<7;o++)
        {
            //get_rand();
            game[6][ran[o]]=0;
        }
        for(int o=7;o<9;o++)
        {
            get_rand();
            for(int p=0;p<6;p++)
                game[o][ran[p]]=0;
        }
    }
    else if(a==5)
    {
        for(int o=0;o<2;o++)
        {
            get_rand();
            for(int p=0;p<5;p++)
                game[o][ran[p]]=0;
        }
        for(int o=2;o<4;o++)
        {
            get_rand();
            for(int p=0;p<6;p++)
                game[o][ran[p]]=0;
        }
        for(int o=4;o<6;o++)
        {
            get_rand();
            for(int p=0;p<3;p++)
                game[o][ran[p]]=0;
        }
        get_rand();
        for(int o=0;o<4;o++)
            game[6][ran[o]]=0;
        for(int o=7;o<9;o++)
        {
            get_rand();
            for(int p=0;p<7;p++)
                game[o][ran[p]]=0;
        }
    }
    else if(a==6)
    {
        for(int o=0;o<2;o++)
        {
            get_rand();
            for(int p=0;p<4;p++)
                game[o][ran[p]]=0;
        }
        for(int o=2;o<5;o++)
        {
            get_rand();
            for(int p=0;p<6;p++)
                game[o][ran[p]]=0;
        }
        get_rand();
        for(int o=0;o<5;o++)
            game[5][ran[o]]=0;
        for(int o=6;o<9;o++)
        {
            get_rand();
            for(int p=0;p<6;p++)
                game[o][ran[p]]=0;
        }
    }
    else if(a==7)
    {
        for(int o=0;o<2;o++)
        {
            get_rand();
            for(int p=0;p<6;p++)
                game[o][ran[p]]=0;
        }
        for(int o=2;o<4;o++)
        {
            get_rand();
            for(int p=0;p<7;p++)
                game[o][ran[p]]=0;
        }
        get_rand();
        for(int o=0;o<4;o++)
            game[4][ran[o]]=0;
        for(int o=5;o<7;o++)
        {
            get_rand();
            for(int p=0;p<6;p++)
                game[o][ran[p]]=0;
        }
        for(int o=7;o<9;o++)
        {
            get_rand();
            for(int p=0;p<5;p++)
                game[o][ran[p]]=0;
        }
    }
    else if(a==8)
    {
        for(int o=0;o<2;o++)
        {
            get_rand();
            for(int p=0;p<6;p++)
                game[o][ran[p]]=0;
        }
        for(int o=2;o<4;o++)
        {
            get_rand();
            for(int p=0;p<7;p++)
                game[o][ran[p]]=0;
        }
        for(int o=4;o<6;o++)
        {
            get_rand();
            for(int p=0;p<5;p++)
                game[o][ran[p]]=0;
        }
        for(int o=6;o<8;o++)
        {
            get_rand();
            for(int p=0;p<7;p++)
                game[o][ran[p]]=0;
        }
        get_rand();
        for(int o=0;o<5;o++)
            game[8][ran[o]]=0;
    }
    else if(a==9)
    {
        get_rand();
        for(int o=0;o<7;o++)
            game[0][ran[o]]=0;
        get_rand();
        for(int o=0;o<6;o++)
            game[1][ran[o]]=0;
        get_rand();
        for(int o=0;o<7;o++)
            game[2][ran[o]]=0;
        get_rand();
        for(int o=0;o<6;o++)
            game[3][ran[o]]=0;
        get_rand();
        for(int o=0;o<7;o++)
            game[4][ran[o]]=0;
        get_rand();
        for(int o=0;o<6;o++)
            game[5][ran[o]]=0;
        get_rand();
        for(int o=0;o<7;o++)
            game[6][ran[o]]=0;
        get_rand();
        for(int o=0;o<6;o++)
            game[7][ran[o]]=0;
        get_rand();
        for(int o=0;o<7;o++)
            game[8][ran[o]]=0;

    }

    vvint vv;
    for(int i=0;i<9;i++)
    {
        vint v;
        for(int j=0;j<9;j++)
        {
            v.push_back(game[i][j]);
        }
        vv.push_back(v);
    }
    if (solve) delete solve;
    solve = new Solve(vv);

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
            b[i][j]->setStyleSheet("background-color: #FFFACD;");
    }
    for(int i=3;i<6;i++)
    {
        for(int j=0;j<3;j++)
            b[i][j]->setStyleSheet("background-color: #D2B48C;");
    }
    for(int i=0;i<3;i++)
    {
        for(int j=3;j<6;j++)
            b[i][j]->setStyleSheet("background-color: #D2B48C;");
    }
    for(int i=6;i<9;i++)
        for(int j=3;j<6;j++)
            b[i][j]->setStyleSheet("background-color: #D2B48C;");
    for(int i=3;i<6;i++)
        for(int j=6;j<9;j++)
            b[i][j]->setStyleSheet("background-color: #D2B48C;");

    for(int i=6;i<9;i++)
        for(int j=0;j<3;j++)
            b[i][j]->setStyleSheet("background-color: #FFFACD;");
    for(int i=3;i<6;i++)
        for(int j=3;j<6;j++)
            b[i][j]->setStyleSheet("background-color: #FFFACD;");
    for(int i=0;i<3;i++)
        for(int j=6;j<9;j++)
            b[i][j]->setStyleSheet("background-color: #FFFACD;");
    for(int i=6;i<9;i++)
        for(int j=6;j<9;j++)
            b[i][j]->setStyleSheet("background-color: #FFFACD;");
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            b[i][j]->setFont(QFont("Microsoft YaHei",11));
            if(game[i][j]!=0)
            {
                QPalette p;
                p.setColor(QPalette::ButtonText,QColor(100,100,200));
                b[i][j]->setPalette(p);
                b[i][j]->setText(ss.setNum(game[i][j]));
                clickable[i][j] = 0;
            }
            else
            {
                b[i][j]->setText("");

            }
            _restore[step][i][j] = game[i][j];

        }
    }
    sameHighlight();
    step++;
}

void MainWindow::refresh()
{
    if(step >= 1)
    {
        int _step = step-1;
       /* for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
                if(_restore[_step][i][j]!=0)
                {
                    QString ss;
                    b[i][j]->setText(ss.setNum(_restore[_step][i][j]));
                    b[i][j]->setStyleSheet("background-color: #D2B48C;");/////C0C0C0
                    clickable[i][j] = 0;
                }
                else
                    b[i][j]->setText("");
            }
        }*/


        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        }
        for(int i=3;i<6;i++)
        {
            for(int j=0;j<3;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");
        }
        for(int i=0;i<3;i++)
        {
            for(int j=3;j<6;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");
        }
        for(int i=6;i<9;i++)
            for(int j=3;j<6;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");
        for(int i=3;i<6;i++)
            for(int j=6;j<9;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");

        for(int i=6;i<9;i++)
            for(int j=0;j<3;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        for(int i=3;i<6;i++)
            for(int j=3;j<6;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        for(int i=0;i<3;i++)
            for(int j=6;j<9;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        for(int i=6;i<9;i++)
            for(int j=6;j<9;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        sameHighlight();
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                b[i][j]->setFont(QFont("Microsoft YaHei",11));
                if(_restore[_step][i][j]!=0)
                {
                    QPalette p;
                    p.setColor(QPalette::ButtonText,QColor(100,100,200));
                    b[i][j]->setPalette(p);
                    QString ss;
                    b[i][j]->setText(ss.setNum(_restore[_step][i][j]));
                    clickable[i][j] = 0;
                }
                else
                    b[i][j]->setText("");
                //_restore[step][i][j] = game[i][j];
            }
        }
    }
}

void MainWindow::get_rand()
{
    int ii,jj,tt;
    for(ii=0;ii<7;)
    {
        tt = rand()%9;
        for(jj=0;jj<ii;jj++)
            if(ran[jj]==tt)
                break;
        if(jj==ii)
        {
            ran[ii]=tt;
            ii++;
        }
    }
}
void MainWindow::checkWin()
{
    if(check_win())
    {
        QApplication::beep();
        timer->stop();
        if(QMessageBox::information(NULL,"Won!","<h2>You have won the game!</h2>\nConquer another one?",
                                    QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes) == QMessageBox::No)
        {
            qApp->exit(0);
        }
        else
        {
            difficulty_choose->show();
        }
    }
}
bool MainWindow::check_win()
{
    const std::set<int> s={1,2,3,4,5,6,7,8,9};
    for(int i=0;i<9;i++)
    {
        std::set<int> m;
        std::set<int> n;
        for(int j=0;j<9;j++)
        {
            m.insert((b[i][j]->text()).toInt());
            n.insert((b[j][i]->text()).toInt());
        }
        if(s!=m||s!=n)
            return 0;
    }
    for(int i=0;i<8;i+=3)
    {
        for(int j=0;j<8;j+=3)
        {
            std::set<int> m;
            for(int x=0;x<3;x++)
            {
                for(int y=0;y<3;y++)
                {
                    m.insert((b[x+i][y+j]->text()).toInt());
                }
            }
            if(m!=s)
                return 0;
        }
    }
    return 1;
}

void MainWindow::repealNum()
{
    QApplication::beep();
    if (__step > 0) __step--;
    if(__step>=0)
    {
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        }
        for(int i=3;i<6;i++)
        {
            for(int j=0;j<3;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");
        }
        for(int i=0;i<3;i++)
        {
            for(int j=3;j<6;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");
        }
        for(int i=6;i<9;i++)
            for(int j=3;j<6;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");
        for(int i=3;i<6;i++)
            for(int j=6;j<9;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");

        for(int i=6;i<9;i++)
            for(int j=0;j<3;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        for(int i=3;i<6;i++)
            for(int j=3;j<6;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        for(int i=0;i<3;i++)
            for(int j=6;j<9;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        for(int i=6;i<9;i++)
            for(int j=6;j<9;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        //sameHighlight();
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                b[i][j]->setFont(QFont("Microsoft YaHei",11));
                if((*Game)[i][j]!=0)
                {
                    QPalette p;
                    p.setColor(QPalette::ButtonText,QColor(100,100,200));
                    b[i][j]->setPalette(p);
                    b[i][j]->setText(QString::number((*Game)[i][j]));
                    clickable[i][j] = 0;
                }
                else if((*Game)[i][j]==0&&__step>=0)
                {
                    b[i][j]->setText(__restore[__step][i][j]);
                }
                else
                    b[i][j]->setText("");
                //_restore[step][i][j] = game[i][j];
            }
        }
        sameHighlight();
        //nummm = __step;
    }
}

void MainWindow::recoverNum()
{
    QApplication::beep();
    if (__step < step_per) __step ++;
    //nummm++;
    nummm = __step;
    if(nummm>=0&&nummm<=step_per)
    {
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        }
        for(int i=3;i<6;i++)
        {
            for(int j=0;j<3;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");
        }
        for(int i=0;i<3;i++)
        {
            for(int j=3;j<6;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");
        }
        for(int i=6;i<9;i++)
            for(int j=3;j<6;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");
        for(int i=3;i<6;i++)
            for(int j=6;j<9;j++)
                b[i][j]->setStyleSheet("background-color: #D2B48C;");

        for(int i=6;i<9;i++)
            for(int j=0;j<3;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        for(int i=3;i<6;i++)
            for(int j=3;j<6;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        for(int i=0;i<3;i++)
            for(int j=6;j<9;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        for(int i=6;i<9;i++)
            for(int j=6;j<9;j++)
                b[i][j]->setStyleSheet("background-color: #FFFACD;");
        //sameHighlight();
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                b[i][j]->setFont(QFont("Microsoft YaHei",11));
                if((*Game)[i][j]!=0)
                {
                    QPalette p;
                    p.setColor(QPalette::ButtonText,QColor(100,100,200));
                    b[i][j]->setPalette(p);
                    b[i][j]->setText(QString::number((*Game)[i][j]));
                    clickable[i][j] = 0;
                }
                else if((*Game)[i][j]==0&&nummm>=0&&nummm<=step_per)
                {
                    b[i][j]->setText(__restore[nummm][i][j]);
                }
                else
                    b[i][j]->setText("");
                //_restore[step][i][j] = game[i][j];
            }
        }
        sameHighlight();
        //__step = nummm;
    }
}

void MainWindow::mark()
{
    QApplication::beep();
    if((*Game)[pos_focused/9][pos_focused%9]==0)
    {
        QString ss;
        ss = b[pos_focused/9][pos_focused%9]->text();
        ss = ss + " ！";
        b[pos_focused/9][pos_focused%9]->setText(ss);
    }
}

void MainWindow::sameHighlight()
{

    int ii,jj;
    ii = pos_focused / 9, jj = pos_focused % 9;
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            if((i==ii||j==jj)&&!(i==ii&&j==jj))
                b[i][j]->setStyleSheet("background-color: #80FFFF");
            if(i==ii&&j==jj)
                b[i][j]->setStyleSheet("background-color: #80FFFF");
        }
    }
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            if((b[i][j]->text().toInt() == b[pos_focused/9][pos_focused%9]->text().toInt())&&b[i][j]->text().toInt() >= 1)
                b[i][j]->setStyleSheet("background-color: #53FF53;");
            else if(i!=pos_focused/9 && j!=pos_focused%9)
            {
                if((i/3+j/3)%2==0)
                    b[i][j]->setStyleSheet("background-color: #FFFACD");
                else
                    b[i][j]->setStyleSheet("background-color: #D2B48C");
            }
        }
    }
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            b[i][j]->setFont(QFont("Microsoft YaHei",11));
            if((*Game)[i][j]!=0)
            {
                QPalette p;
                p.setColor(QPalette::ButtonText,QColor(100,100,200));
                b[i][j]->setPalette(p);
            }
        }
    }

}
void MainWindow::delNum()
{
    QApplication::beep();
   // for(int i=0;i<9;i++)
    {
       // for(int j=0;j<9;j++)
        {
            if(((*Game)[pos_focused/9][pos_focused%9])==0)
            {
                b[pos_focused/9][pos_focused%9]->setText("");
            }
        }
    }
    sameHighlight();
    step_per = __step;
    step_per++;
    __step ++;
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            //if(b[i][j]->text().length()<=1)
            {
                __restore[step_per][i][j] = b[i][j]->text();
            }
        }
    }
}
void MainWindow::prohibited()
{
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            b[i][j]->setEnabled(0);
        }
    }
    for(int i=0;i<9;i++)
        inputButton[i]->setEnabled(0);
    ui->markButton->setEnabled(0);
    ui->delButton->setEnabled(0);
    ui->recoverButton->setEnabled(0);
    ui->repealButton->setEnabled(0);
}
void MainWindow::reEnable()
{
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {

            b[i][j]->setEnabled(1);
        }
    }
    for(int i=0;i<9;i++)
        inputButton[i]->setEnabled(1);
    ui->markButton->setEnabled(1);
    ui->delButton->setEnabled(1);
    ui->recoverButton->setEnabled(1);
    ui->repealButton->setEnabled(1);
}

