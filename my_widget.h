#ifndef MY_WIDGET_H
#define MY_WIDGET_H

#include "maze_solve.h"
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QPen>
#include <QKeyEvent>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrentRun>

//主菜单界面
class MainUI : public QWidget
{
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = nullptr) : QWidget(parent) {
        row = 15;
        column = 15;
        this->resize(WIDTH * column + 4 * WIDTH, WIDTH * row);
        this->setWindowTitle("maze");
        setupMainWindow();
        setupConnections();
    }

private:
    QLabel *labelTitle;
    QPushButton *autoMazeButton;
    QPushButton *userSetButton;
    //QPushButton *helpButton;
    QPushButton *exitButton;

private:
    void setupMainWindow();
    void setupConnections();

private slots:
    void intoAutoMaze();
    void intoUserSetMaze();
    //void helpWords();
    void exitMaze();

protected:
    void paintEvent(QPaintEvent * event) override;
};

class AutoWidget : public QWidget
{
    Q_OBJECT    //使用信号槽机制必须要包含这个宏

public:
    explicit AutoWidget(QWidget *parent = nullptr) : QWidget(parent){
        this->resize(WIDTH * column + 4 * WIDTH, WIDTH * row);
        this->setWindowTitle("maze");
        setupUI();
        setupConnections();
    }

private:
    void setupUI();
    void setupConnections();

private:
    void paintMaze(char *maze,int nowPosition[2]);

private slots:
    void mazeSizeSet(int size);
    void backLastPage();


private:
    QPushButton *backButton;            //返回按键
    QPushButton *mazeSizeButton[5];     //迷宫难度选择按键
};

//用户自定义迷宫的类
class UserSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserSetWidget(QWidget *parent = nullptr) : QWidget(parent){
        row = 15;
        column = 15;
        this->resize(WIDTH * column + 4 * WIDTH, WIDTH * row);
        this->setWindowTitle("maze");
        setupUI();
        setupInitConnections();
    }

private:
    void setupUI();
    void setupInitConnections();

private slots:
    void backMainWindow();
    void mazeSizeSet(int size);

private:
    QPushButton *mazeSizeButton[5];
    QPushButton *backMainButton;
};

//只服务于AutoWidget
class PlayWidget : public QWidget{
    Q_OBJECT

public:
    explicit PlayWidget(QWidget *parent,int size) : QWidget(parent){
        //自动生成迷宫
        if (size > 0){
            row = 4 * size + 7;
            column = 4 * size + 7;
        }
        this->resize(WIDTH * column + 4 * WIDTH, WIDTH * row);
        this->setWindowTitle("maze");
        initMaze();
        setupUI();
        setupInitConnections();
        showMode();
        hidePlay();
    }
    ~PlayWidget(){
        //释放空间
        if (maze){
            free(maze);
        }
        if (mazeWithUserWay){
            free(mazeWithUserWay);
        }
        if (mazeWithWay){
            free(mazeWithWay);
        }
    }

private:
    void initMaze();
    void setupUI();
    void setupInitConnections();
    void paintMaze(char *maze,int nowPosition[2]);
    void backModePage();
    void createMaze_1();
    void createMaze_2();
    void createMaze_3();
    void showMode();
    void hideMode();
    void showPlay();
    void hidePlay();
    void clearMaze();


private:
    //需要申请空间row * column
    char *maze;             //迷宫二维数组，用'1'表示空路，用'0'表示有障碍，用'#'表示已走过的位置，用'*'表示终点，用'@'表示起点
    char *mazeWithWay;      //A*算法求解的有路径的迷宫
    char *mazeWithUserWay;  //用户已走过的路径
    //需要申请空间row * column
    int nowPosition[2];
    int nextPosition[2];
    int start[2];
    int end[2];
    int moveNumber;
    bool isShowBestWay = false;
    bool isEnableMove = false;
    bool isEnablePaint = false;
    QLabel *labelWaining;
    QLabel *labelMoveNum;
    QLabel *labelExplanation[6];
    QPushButton *sloveMazeByAButton;    //A*算法求解迷宫按键
    QPushButton *backButton;            //返回按键
    QPushButton *resetButton;           //重置按键
    QPushButton *createButton1;
    QPushButton *createButton2;
    QPushButton *createButton3;
    QPushButton *backModeButton;

private slots:
    void solveMazeByA();
    void backLastWindow();
    void resetMaze();

protected:
    void paintEvent(QPaintEvent * event) override;
    void keyPressEvent(QKeyEvent *event);
};

//服务于UserSetWidget
class PlayUserSetWidget : public QWidget{
    Q_OBJECT

public:
    explicit PlayUserSetWidget(QWidget *parent = nullptr) : QWidget(parent){
        //动态分配
        mazeUser = (char *)malloc(row * column * sizeof(char));
        mazeWithWay = (char *)malloc(row * column * sizeof(char));
        mazeWithUserWay = (char *)malloc(row * column * sizeof(char));
        mazeButton = (QPushButton **)malloc(row * column * sizeof(QPushButton *));
        //判断是否分配到内存
        if (mazeButton == NULL){
            qDebug()<<"mazeButton error";
        }
        for (int i = 0; i < row; i++){
            for (int j = 0; j < column; j++){
                mazeButton[i * column + j] = (QPushButton *)malloc(sizeof(QPushButton));
            }
        }
        this->resize(WIDTH * column + 4 * WIDTH, WIDTH * row);
        this->setWindowTitle("maze");
        initMazeUser();
        setupUI();
        setupInitConnections();
        hidePlay();
    }
    ~PlayUserSetWidget(){
        if (mazeUser) {
            free(mazeUser);
        }
        if (mazeWithUserWay) {
            free(mazeWithUserWay);
        }
        if (mazeWithWay){
            free(mazeWithWay);
        }
        if (mazeButton) {
            for (int i = 0; i < row; i++){
                for (int j = 0; j < column; j++){
                    free(mazeButton[i * column + j]);
                }
            }
            free(mazeButton);
        }
    }

private:
    void initMazeUser();
    void setupUI();
    void setupInitConnections();
    void breakMazeButtonConnections();
    void paintMaze(char *maze,int nowPosition[2]);
    void showUserSet();
    void hideUserSet();
    void showPlay();
    void hidePlay();

private slots:
    void onButtonClickedSetMaze(int row, int column);
    void onButtonClickedSetStart(int row, int column);
    void onButtonClickedSetEnd(int row, int column);

    void setupBarrierConnections();
    void setupStartConnections();
    void setupEndConnections();

    void resetUserSetMaze();
    void finishUserSetmaze();

    void backUserSet();
    void resetUserWay();
    void solveMazeByA();
    void backModePage();

private:
    QPushButton **mazeButton = NULL;        //需要申请空间row * column
    QPushButton *startSetButton;
    QPushButton *endSetButton;
    QPushButton *barrierSetButton;
    QPushButton *finishButton;
    QPushButton *resetButton;
    QPushButton *backModePageButton;

    QPushButton *backUserSetButton;
    QPushButton *resetUserWayButton;
    QPushButton *sloveMazeByAButton;

    QLabel *labelWaining;
    QLabel *labelMoveNum;
    QLabel *labelExplanation[6];
    char *mazeUser = NULL;          //需要申请空间row * column
    char *mazeWithUserWay = NULL;   //需要申请空间row * column
    char *mazeWithWay = NULL;       //用于存储求解的路径
    int nowPosition[2];
    int nextPosition[2];
    int start[2];
    int end[2];
    bool isSetStart = false;
    bool isSetEnd = false;
    bool isEnableMove = false;
    bool isShowBestWay = false;
    bool isShowUserWay = false;
    bool isPlayMaze = false;
    QMetaObject::Connection conn[MAX * MAX];
    int moveNumber;

protected:
    void paintEvent(QPaintEvent * event) override;
    void keyPressEvent(QKeyEvent *event);
};

#endif
