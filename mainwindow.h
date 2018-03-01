#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QFrame>
#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include <QPixmap>
#include <QPushButton>
#include <QRadioButton>
#include "sudoku.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "solve.h"
typedef std::vector<std::vector<bool> > vvbool;
typedef std::vector<int> vint;
typedef std::vector<vint > vvint;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent*);


private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTime *TimeRecord;
    bool isStart;
    QWidget* sudoku_widget;
   // QFrame* frame;
    ///QWidget* selectLevel;
   /// QLabel* selectLevel_label;
    QPushButton*** b;
    QWidget* difficulty_choose;
    QRadioButton** radiobutton;
    QPushButton* button;
    //QHBoxLayout* __;
    //QVBoxLayout* ___;
    QPushButton** inputButton;
    QPushButton* THIS;
    QPushButton* ANOTHER;
    vvbool clickable;
    int ran[7];
    void get_rand();
    int pos_focused = 0;
    int inputButtonNum;
    int*** _restore;
    QString*** __restore;
    Sudoku* sudoku;
    //bool _newGamebool = 0;
    //bool _againGamebool = 0;
    bool check_win();
    Solve* solve;
    //std::pair<int, int> help();

signals:
    void radioButtonPressed();
    void enableInputButton();

private slots:
    void learn_how_to_play();
    void provide_hints();
    void updateTime();
    void Start_clicked_again();
    void Stop_clicked_pause();
    void Conti_clicked();
    void choose_difficulty();
    void begin_game(int);
    void setEnabled_button();
    void refresh();
    void take_it_down(int pos_focus);
    void take_it_down_2(int);
    void show_press_button();
    void enableInputButton_2();
    void enableAgainButton();
    void checkWin();
    void repealNum();
    void recoverNum();
    void mark();
    void delNum();
    void sameHighlight();
    void prohibited();
    void reEnable();
    //void check_once(int);
    //void check_twice(int);
    //void highlight();

    //void stay();
};
#endif // MAINWINDOW_H
