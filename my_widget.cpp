#include "my_widget.h"

/* -------------------------MainUI的函数-----------------------------*/
//设置菜单UI界面
void MainUI::setupMainWindow(){
    QGridLayout *layout = new QGridLayout(this);

    labelTitle = new QLabel("迷宫",this);
    labelTitle->setAlignment(Qt::AlignCenter);

    autoMazeButton = new QPushButton("自动随机生成迷宫", this);
    userSetButton = new QPushButton("用户自定义迷宫", this);
    //helpButton = new QPushButton("帮助",this);
    exitButton = new QPushButton("退出", this);

    autoMazeButton->setFixedSize(WIDTH * 4, WIDTH); // 宽度4*WIDTH像素，高度1*WIDTH像素
    userSetButton->setFixedSize(WIDTH * 4, WIDTH);
    //helpButton->setFixedSize(WIDTH * 4, WIDTH);
    exitButton->setFixedSize(WIDTH * 4, WIDTH);
    labelTitle->setFixedSize(WIDTH * 4, WIDTH);

    layout->addWidget(labelTitle,0,0);
    layout->addWidget(autoMazeButton,1,0);
    layout->addWidget(userSetButton,2,0);
    //layout->addWidget(helpButton,3,0);
    layout->addWidget(exitButton,4,0);

    QFont font = this->labelTitle->font();
    font.setPointSize(20);
    this->labelTitle->setFont(font);

    // 设置水平和垂直间隔
    layout->setHorizontalSpacing(0); // 水平间隔
    layout->setVerticalSpacing(0);   // 垂直间隔
    layout->setContentsMargins(0, 0, 0, 0); // 设置布局边缘的空白

    setLayout(layout);
}
//建立信号与槽的连接
void MainUI::setupConnections(){
    connect(autoMazeButton, &QPushButton::clicked, this, &MainUI::intoAutoMaze);
    connect(userSetButton, &QPushButton::clicked, this, &MainUI::intoUserSetMaze);
    //connect(helpButton, &QPushButton::clicked, this, &MainUI::helpWords);
    connect(exitButton, &QPushButton::clicked, this, &MainUI::exitMaze);
}
//进入自动随机生成迷宫选择界面
void MainUI::intoAutoMaze(){
    AutoWidget *autoMazeWindow = new AutoWidget();
    autoMazeWindow->show();
    this->close();
}
//进入用户自定义迷宫选择界面
void MainUI::intoUserSetMaze(){
    UserSetWidget *userSetMazeWindow = new UserSetWidget();
    userSetMazeWindow->show();
    this->close();
}
// //帮助提示界面(是新开一个界面提供完整的帮助文档，还是跳出一个弹窗简要提示)【待定】
// void MainUI::helpWords(){
//     QMessageBox::information(this, "帮助", "");
// }
//退出游戏
void MainUI::exitMaze(){
    QMessageBox::information(this, "迷宫","成功退出");
    this->close();
}
//绘制
void MainUI::paintEvent(QPaintEvent * event){
    QPainter painter(this);
    QPen pen;           //用于绘制

    pen.setColor(QColor(Qt::blue));//设置笔颜色
    pen.setWidth(WIDTH);//设置笔宽度
    painter.setPen(pen);
    painter.drawPoint(QPoint(0 * WIDTH + WIDTH / 2 , 0 * WIDTH + WIDTH / 2));//画点
    painter.drawPoint(QPoint((column + 3) * WIDTH + WIDTH / 2,0 * WIDTH + WIDTH / 2 ));//画点
    painter.drawPoint(QPoint( 0 * WIDTH + WIDTH / 2 , (row - 1) * WIDTH + WIDTH / 2));//画点
    painter.drawPoint(QPoint((column + 3) * WIDTH + WIDTH / 2, (row - 1) * WIDTH + WIDTH / 2 ));//画点
}
/* -------------------------AutoWidget的函数-----------------------------*/
//设置自动随机生成迷宫的UI界面
void AutoWidget::setupUI(){
    QGridLayout *layout = new QGridLayout(this);

    mazeSizeButton[0] = new QPushButton("9 x 9",this);
    mazeSizeButton[1] = new QPushButton("13 x 13",this);
    mazeSizeButton[2] = new QPushButton("17 x 17",this);
    mazeSizeButton[3] = new QPushButton("21 x 21",this);
    mazeSizeButton[4] = new QPushButton("25 x 25",this);
    backButton = new QPushButton("返回主界面",this);

    mazeSizeButton[0]->setFixedSize(WIDTH * 4, WIDTH);
    mazeSizeButton[1]->setFixedSize(WIDTH * 4, WIDTH);
    mazeSizeButton[2]->setFixedSize(WIDTH * 4, WIDTH);
    mazeSizeButton[3]->setFixedSize(WIDTH * 4, WIDTH);
    mazeSizeButton[4]->setFixedSize(WIDTH * 4, WIDTH);
    backButton->setFixedSize(WIDTH * 4, WIDTH);

    for (int i = 0; i < 5; i++){
        layout->addWidget(mazeSizeButton[i],i,0);
    }
    layout->addWidget(backButton,5,0);

    // 设置水平和垂直间隔
    layout->setHorizontalSpacing(0); // 水平间隔
    layout->setVerticalSpacing(0);   // 垂直间隔
    layout->setContentsMargins(0, 0, 0, 0); // 设置布局边缘的空白

    setLayout(layout);
}
//建立信号与槽的连接
void AutoWidget::setupConnections(){
    for (int i = 0; i < 5; i++){
        connect(mazeSizeButton[i],&QPushButton::clicked,[=](){
            mazeSizeSet(i + 1);
        });
    }
    connect(backButton, &QPushButton::clicked, this, &AutoWidget::backLastPage);
}
//设置迷宫规格的槽函数
void AutoWidget::mazeSizeSet(int size){
    PlayWidget *playWindow = new PlayWidget(nullptr,size);
    playWindow->show();
    this->close();
}
//返回上一页的槽函数
void AutoWidget::backLastPage(){
    MainUI *mainUIWindow = new MainUI();
    mainUIWindow->show();
    this->close();
}

/* -------------------------UserSetWidget的函数-----------------------------*/
//设置用户自定义迷宫页面
void UserSetWidget::setupUI(){
    QGridLayout *layout = new QGridLayout(this);

    mazeSizeButton[0] = new QPushButton("9 x 9",this);
    mazeSizeButton[1] = new QPushButton("13 x 13",this);
    mazeSizeButton[2] = new QPushButton("17 x 17",this);
    mazeSizeButton[3] = new QPushButton("21 x 21",this);
    mazeSizeButton[4] = new QPushButton("25 x 25",this);
    backMainButton = new QPushButton("返回主界面",this);

    for (int i = 0; i < 5; i++){
        layout->addWidget(mazeSizeButton[i],i,column);
        mazeSizeButton[i]->setFixedSize(WIDTH * 4, WIDTH);
    }
    layout->addWidget(backMainButton,5,column);
    backMainButton->setFixedSize(WIDTH * 4, WIDTH);

    // 设置水平和垂直间隔
    layout->setHorizontalSpacing(0); // 水平间隔
    layout->setVerticalSpacing(0);   // 垂直间隔
    layout->setContentsMargins(0, 0, 0, 0); // 设置布局边缘的空白

    setLayout(layout);
}
//建立初始化信号与槽的连接
void UserSetWidget::setupInitConnections(){
    connect(backMainButton, &QPushButton::clicked, this, &UserSetWidget::backMainWindow);
    for (int i = 0; i < 5; i++){
        connect(mazeSizeButton[i],&QPushButton::clicked,[=](){
            mazeSizeSet(i + 1);
        });
    }
}
//设置迷宫规格的槽函数
void UserSetWidget::mazeSizeSet(int size){
    row = 4 * size + 7;
    column = 4 * size + 7;
    PlayUserSetWidget *playUserSetWindow = new PlayUserSetWidget();
    playUserSetWindow->show();
    this->close();
}
//返回上一页的槽函数
void UserSetWidget::backMainWindow(){
    MainUI *mainUIWindow = new MainUI();
    mainUIWindow->show();
    this->close();
}
/* -------------------------PlayWidget的函数-----------------------------*/
//初始化迷宫
void PlayWidget::initMaze(){
    //动态分配
    maze = (char *)malloc(row * column * sizeof(char));
    mazeWithUserWay = (char *)malloc(row * column * sizeof(char));
    mazeWithWay = (char *)malloc(row * column * sizeof(char));
    clearMaze();
}
//设置迷宫UI界面
void PlayWidget::setupUI(){
    QGridLayout *layout = new QGridLayout(this);

    labelWaining = new QLabel("准备",this);        //用于警告的标签
    labelMoveNum = new QLabel("步数:0",this);     //用于显示移动的步数
    labelMoveNum->setText(QString("步数:%1").arg(moveNumber));
    labelWaining->setAlignment(Qt::AlignCenter);
    labelMoveNum->setAlignment(Qt::AlignCenter);

    //颜色注释
    labelExplanation[0] = new QLabel("障碍", this);
    labelExplanation[1] = new QLabel("用户", this);
    labelExplanation[2] = new QLabel("起点", this);
    labelExplanation[3] = new QLabel("终点", this);
    labelExplanation[4] = new QLabel("已走路径", this);
    labelExplanation[5] = new QLabel("最佳路径", this);

    backButton = new QPushButton("返回上一页",this);
    sloveMazeByAButton = new QPushButton("显示最佳路径",this);
    resetButton = new QPushButton("重置已走路线",this);
    createButton1 = new QPushButton("创建第一类迷宫",this);
    createButton2 = new QPushButton("创建第二类迷宫",this);
    createButton3 = new QPushButton("创建第三类迷宫",this);
    backModeButton = new QPushButton("返回上一页",this);


    labelWaining->setFixedSize(WIDTH * 4, WIDTH);
    labelMoveNum->setFixedSize(WIDTH * 4, WIDTH);
    backModeButton->setFixedSize(WIDTH * 4, WIDTH);
    sloveMazeByAButton->setFixedSize(WIDTH * 4, WIDTH);
    resetButton->setFixedSize(WIDTH * 4, WIDTH);
    createButton1->setFixedSize(WIDTH * 4, WIDTH);
    createButton2->setFixedSize(WIDTH * 4, WIDTH);
    createButton3->setFixedSize(WIDTH * 4, WIDTH);
    backButton->setFixedSize(WIDTH * 4, WIDTH);
    for (int i=0; i<6; i++){
        labelExplanation[i]->setFixedSize(WIDTH * 2, WIDTH);
    }

    layout->addWidget(labelWaining, 0, column, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(labelMoveNum, 1, column, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(sloveMazeByAButton, 2, column, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(resetButton, 3, column, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(backModeButton, 4, column, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(createButton1, 0, column);
    layout->addWidget(createButton2, 1, column);
    layout->addWidget(createButton3, 2, column);
    layout->addWidget(backButton, 3, column);
    for (int i=0; i<6; i++){
        layout->addWidget(labelExplanation[i], i + 5, column, Qt::AlignRight | Qt::AlignTop);
    }

    QFont font = this->labelWaining->font();
    font.setPointSize(15);
    this->labelWaining->setFont(font);

    // 设置水平和垂直间隔
    layout->setHorizontalSpacing(0); // 水平间隔
    layout->setVerticalSpacing(0);   // 垂直间隔
    layout->setContentsMargins(0, 0, 0, 0); // 设置布局边缘的空白

    setLayout(layout);
}
//建立初始化信号与槽的连接
void PlayWidget::setupInitConnections(){
    //连接信号与槽
    connect(backButton, &QPushButton::clicked, this, &PlayWidget::backLastWindow);
    connect(sloveMazeByAButton, &QPushButton::clicked, this, &PlayWidget::solveMazeByA);
    connect(resetButton, &QPushButton::clicked, this, &PlayWidget::resetMaze);
    connect(createButton1, &QPushButton::clicked, this, &PlayWidget::createMaze_1);
    connect(createButton2, &QPushButton::clicked, this, &PlayWidget::createMaze_2);
    connect(createButton3, &QPushButton::clicked, this, &PlayWidget::createMaze_3);
    connect(backModeButton, &QPushButton::clicked, this, &PlayWidget::backModePage);
}
//显示选择模式的按键
void PlayWidget::showMode(){
    backButton->show();
    createButton1->show();
    createButton2->show();
    createButton3->show();
}
//隐藏选择模式的按键
void PlayWidget::hideMode(){
    backButton->hide();
    createButton1->hide();
    createButton2->hide();
    createButton3->hide();
}
//显示游玩迷宫的界面按键
void PlayWidget::showPlay(){
    sloveMazeByAButton->show();
    resetButton->show();
    backModeButton->show();
    labelMoveNum->show();
    labelWaining->show();
    labelExplanation[0]->show();
    labelExplanation[1]->show();
    labelExplanation[2]->show();
    labelExplanation[3]->show();
    labelExplanation[4]->show();
    labelExplanation[5]->show();
}
//隐藏游玩迷宫的界面按键
void PlayWidget::hidePlay(){
    sloveMazeByAButton->hide();
    resetButton->hide();
    backModeButton->hide();
    labelMoveNum->hide();
    labelWaining->hide();
    labelExplanation[0]->hide();
    labelExplanation[1]->hide();
    labelExplanation[2]->hide();
    labelExplanation[3]->hide();
    labelExplanation[4]->hide();
    labelExplanation[5]->hide();
}
//返回上一页
void PlayWidget::backModePage(){
    showMode();
    hidePlay();
    clearMaze();
    isEnableMove = false;
    isEnablePaint = false;
    isShowBestWay = false;
    update();
}
//创建第一类迷宫，即有一个明显长主线的迷宫
void PlayWidget::createMaze_1(){
    showPlay();
    hideMode();
    createMaze(maze,start,end);
    nowPosition[0] = start[0];
    nowPosition[1] = start[1];
    isEnableMove = true;
    isEnablePaint = true;
    isShowBestWay = false;
    update();
}
//创建第二类迷宫，即有多分叉的迷宫
void PlayWidget::createMaze_2(){
    showPlay();
    hideMode();
    createMazeNatural(maze,start,end,row,column);
    nowPosition[0] = start[0];
    nowPosition[1] = start[1];
    isEnableMove = true;
    isEnablePaint = true;
    isShowBestWay = false;
    update();
}
//创建第三类迷宫，即有空大方块的迷宫
void PlayWidget::createMaze_3(){
    showPlay();
    hideMode();
    createMazeBlock(maze,start,end,row,column);
    nowPosition[0] = start[0];
    nowPosition[1] = start[1];
    isEnableMove = true;
    isEnablePaint = true;
    isShowBestWay = false;
    update();
}
//创建初始迷宫数组
void PlayWidget::clearMaze(){
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            mazeWithUserWay[i * column + j] = '!';
            mazeWithWay[i * column + j] = '!';
            maze[i * column + j] = '!';
        }
    }
    moveNumber = 0;
}
//A*算法寻找最优路线
void PlayWidget::solveMazeByA(){
    searchPath(mazeWithWay,maze,start,end); //寻找起点到终点的路径，并存入有路径的迷宫的二维数组，用'?'表示路径
    if (isShowBestWay == false){
        isShowBestWay = true;
    }
    else{
        isShowBestWay = false;
    }

    update();
}
//返回上一页
void PlayWidget::backLastWindow(){
    row = 15;
    column = 15;
    AutoWidget *AutoMazeWindow = new AutoWidget();
    AutoMazeWindow->show();
    this->close();
}
//重置迷宫，即返回原点
void PlayWidget::resetMaze(){
    //清空步数
    moveNumber = 0;
    labelMoveNum->setText(QString("步数:%1").arg(moveNumber));
    //用户已走路径的数组重置
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            mazeWithUserWay[i * column + j] = '!';
        }
    }
    //当前位置的重置
    nowPosition[0] = start[0];
    nowPosition[1] = start[1];

    update();
}
//绘制迷宫
void PlayWidget::paintMaze(char *maze,int nowPosition[2])
{
    QPainter painter(this);
    QPen pen;           //用于绘制障碍
    QPen penStart;      //用于绘制起点的笔
    QPen penEnd;        //用于绘制终点的笔
    QPen penWay;        //用于绘制路径的笔
    QPen penNow;        //用于绘制现在位置的笔
    QPen penBestWay;    //用于绘制A*求解的路径

    pen.setColor(QColor(Qt::blue));//设置笔颜色
    pen.setWidth(WIDTH);//设置笔宽度
    penStart.setColor(QColor(Qt::yellow));
    penStart.setWidth(WIDTH / 2);
    penEnd.setColor(QColor(Qt::red));
    penEnd.setWidth(WIDTH / 2);
    penWay.setColor(QColor(Qt::green));
    penWay.setWidth(WIDTH / 4);   
    penNow.setColor(QColor(Qt::black));
    penNow.setWidth(WIDTH / 3);   
    penBestWay.setColor(QColor(Qt::cyan));  //青色
    penBestWay.setWidth(WIDTH / 4);

    //颜色注释
    painter.setPen(pen);
    painter.drawPoint(QPoint((column + 1) * WIDTH, 5 * (column * WIDTH / 11) + WIDTH / 2));
    painter.setPen(penNow);
    painter.drawPoint(QPoint((column + 1) * WIDTH, 6 * (column * WIDTH / 11) + WIDTH / 2));
    painter.setPen(penStart);
    painter.drawPoint(QPoint((column + 1) * WIDTH, 7 * (column * WIDTH / 11) + WIDTH / 2));
    painter.setPen(penEnd);
    painter.drawPoint(QPoint((column + 1) * WIDTH, 8 * (column * WIDTH / 11) + WIDTH / 2));
    painter.setPen(penWay);
    painter.drawPoint(QPoint((column + 1) * WIDTH, 9 * (column * WIDTH / 11) + WIDTH / 2));
    painter.setPen(penBestWay);
    painter.drawPoint(QPoint((column + 1) * WIDTH, 10 * (column * WIDTH / 11) + WIDTH / 2));

    for (int i=0;i<row;i++){
        for (int j=0;j<column;j++){
            //障碍
            if (maze[i * column + j] == '0') {
                painter.setPen(pen);
                painter.drawPoint(QPoint(j * WIDTH + WIDTH / 2, i * WIDTH + WIDTH / 2));    //画点
            }
            //路
            else if (maze[i * column + j] == '!') {
                //不绘制，防止覆盖
            }
            //起点
            else if (maze[i * column + j] == '@') {
                painter.setPen(penStart);
                painter.drawPoint(QPoint(j * WIDTH + WIDTH / 2, i * WIDTH + WIDTH / 2));    
            }
            //终点
            else if (maze[i * column + j] == '*') {
                painter.setPen(penEnd);
                painter.drawPoint(QPoint(j * WIDTH + WIDTH / 2, i * WIDTH + WIDTH / 2));    
            }
            //用户路径
            else if (maze[i * column + j] == '#') {
                painter.setPen(penWay);
                painter.drawPoint(QPoint(j * WIDTH + WIDTH / 2, i * WIDTH + WIDTH / 2));    
            }
            //求解的路径
            else if (maze[i * column + j] == '?') {
                painter.setPen(penBestWay);
                painter.drawPoint(QPoint(j * WIDTH + WIDTH / 2, i * WIDTH + WIDTH / 2));    
            }
            else {
                qDebug()<<"maze paint error!";
            }
        }
    }
    //绘制当前位置
    painter.setPen(penNow);
    painter.drawPoint(QPoint(nowPosition[1] * WIDTH + WIDTH / 2, nowPosition[0] * WIDTH + WIDTH / 2));
}
//绘制
void PlayWidget::paintEvent(QPaintEvent * event){
    if (isEnablePaint == true){
        //绘制迷宫
        paintMaze(maze, nowPosition);
        //绘制A*求解的路径
        if (isShowBestWay == true){
            paintMaze(mazeWithWay, nowPosition);
        }
        //绘制用户路径
        paintMaze(mazeWithUserWay, nowPosition);
    }
}
//判断按键按下的函数，用于走迷宫
void PlayWidget::keyPressEvent(QKeyEvent *event){
    if (isEnableMove == true){
        int moveResult = 0;
        switch (event->key())
        {
        case Qt::Key_D:
            nextPosition[0] = nowPosition[0];
            nextPosition[1] = nowPosition[1] + 1;
            moveResult = checkMove(maze,nowPosition,nextPosition);
            break;
        case Qt::Key_W:
            nextPosition[0] = nowPosition[0] - 1;
            nextPosition[1] = nowPosition[1];
            moveResult = checkMove(maze,nowPosition,nextPosition);
            break;
        case Qt::Key_A:
            nextPosition[0] = nowPosition[0];
            nextPosition[1] = nowPosition[1] - 1;
            moveResult = checkMove(maze,nowPosition,nextPosition);
            break;
        case Qt::Key_S:
            nextPosition[0] = nowPosition[0] + 1;
            nextPosition[1] = nowPosition[1];
            moveResult = checkMove(maze,nowPosition,nextPosition);
            break;
        default:
            moveResult = 3;
            break;
        }
        if (moveResult == 0) {
            labelWaining->setText("不可走");
        }
        else if (moveResult == 1){
            mazeWithUserWay[nowPosition[0] * column + nowPosition[1]] = '#';
            labelWaining->setText("移动");
            moveNumber++;
        }
        else if (moveResult == 2){
            labelWaining->setText("成功");
            mazeWithUserWay[nowPosition[0] * column + nowPosition[1]] = '#';
            QMessageBox::information(this, "迷宫","成功");
            moveNumber++;
        }
        else if (moveResult == 3){
            QMessageBox::information(this, "迷宫","上(W)下(S)左(A)右(D)");
            labelWaining->setText("上(W)下(S)左(A)右(D)");
        }
        else {
                labelWaining->setText("移动异常");
            }
        labelMoveNum->setText(QString("步数:%1").arg(moveNumber));
        update();
    }
}

/*---------------------------PlayUserSetWidget的函数------------------------------*/
//初始化自定义迷宫
void PlayUserSetWidget::initMazeUser(){
    //初始化迷宫二维数组
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            //画迷宫边界
            if ( i == 0 || i == row - 1){
                mazeUser[i * column + j] = '0';
            }
            else if ( j == 0 || j == column - 1){
                mazeUser[i * column + j] = '0';
            }
            else{
                mazeUser[i * column + j] = '!';
            }
            mazeWithUserWay[i * column + j] = '!';      //将mazeWithUserWay设置为空路
            mazeWithWay[i * column + j] = '!';          //将mazeWithWay设置为空路
        }
    }
    //初始化起点、终点等存储信息
    nowPosition[0] = -1;
    nowPosition[1] = -1;
    start[0] = -1;
    start[1] = -1;
    end[0] = -1;
    end[1] = -1;
    isSetStart = false;
    isSetEnd = false;
    isEnableMove = false;
    isPlayMaze = false;
    moveNumber = 0;
}
//设置UI界面
void PlayUserSetWidget::setupUI(){
    QGridLayout *layout = new QGridLayout(this);

    labelWaining = new QLabel("准备",this);        //用于警告的标签
    labelMoveNum = new QLabel("步数:0",this);
    labelMoveNum->setText(QString("步数:%1").arg(moveNumber));
    labelWaining->setAlignment(Qt::AlignCenter);
    labelMoveNum->setAlignment(Qt::AlignCenter);

    //颜色注释
    labelExplanation[0] = new QLabel("障碍", this);
    labelExplanation[1] = new QLabel("用户", this);
    labelExplanation[2] = new QLabel("起点", this);
    labelExplanation[3] = new QLabel("终点", this);
    labelExplanation[4] = new QLabel("已走路径", this);
    labelExplanation[5] = new QLabel("最佳路径", this);

    startSetButton = new QPushButton("去设置起点",this);
    endSetButton = new QPushButton("去设置终点",this);
    barrierSetButton = new QPushButton("去设置障碍",this);
    finishButton = new QPushButton("迷宫完成",this);
    resetButton = new QPushButton("重置迷宫",this);
    backModePageButton = new QPushButton("返回上一页",this);

    backUserSetButton = new QPushButton("返回上一页",this);
    resetUserWayButton = new QPushButton("重置已走路线", this);
    sloveMazeByAButton = new QPushButton("显示最佳路径", this);

    //设置按钮在窗口的右上角
    layout->addWidget(labelWaining,0,column,1,2, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(labelMoveNum,1,column,1,2,Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(startSetButton,2,column,1,2, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(endSetButton,3,column,1,2, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(barrierSetButton,4,column,1,2, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(resetButton,5,column,1,2, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(finishButton,6,column,1,2, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(backModePageButton,7,column,1,2, Qt::AlignRight | Qt::AlignTop);

    layout->addWidget(resetUserWayButton,8,column,1,2, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(sloveMazeByAButton,9,column,1,2, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(backUserSetButton,10,column,1,2, Qt::AlignRight | Qt::AlignTop);

    for (int i=0;i<6;i++){
        layout->addWidget(labelExplanation[i],11 + i,column,1,2,Qt::AlignRight | Qt::AlignTop);
        labelExplanation[i]->setFixedSize(WIDTH * 2, WIDTH);
    }

    labelWaining->setFixedSize(WIDTH * 4, WIDTH);
    labelMoveNum->setFixedSize(WIDTH * 4, WIDTH);
    startSetButton->setFixedSize(WIDTH * 4, WIDTH);
    endSetButton->setFixedSize(WIDTH * 4, WIDTH);
    barrierSetButton->setFixedSize(WIDTH * 4, WIDTH);
    finishButton->setFixedSize(WIDTH * 4, WIDTH);
    resetButton->setFixedSize(WIDTH * 4, WIDTH);
    
    resetUserWayButton->setFixedSize(WIDTH * 4, WIDTH);
    sloveMazeByAButton->setFixedSize(WIDTH * 4, WIDTH);
    backUserSetButton->setFixedSize(WIDTH * 4, WIDTH);
    backModePageButton->setFixedSize(WIDTH * 4, WIDTH);

    //显示可设置的迷宫
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            mazeButton[i * column + j] = new  QPushButton(" ",this);
            mazeButton[i * column + j]->setFixedSize(WIDTH, WIDTH);
            layout->addWidget(mazeButton[i * column + j], i , j);
        }
    }

    QFont font = this->labelWaining->font();
    font.setPointSize(15);
    this->labelWaining->setFont(font);

    // 设置水平和垂直间隔
    layout->setHorizontalSpacing(0); // 水平间隔
    layout->setVerticalSpacing(0);   // 垂直间隔
    layout->setContentsMargins(0, 0, 0, 0); // 设置布局边缘的空白

    setLayout(layout);
}
//建立初始信号与槽的连接
void PlayUserSetWidget::setupInitConnections(){
    //连接设置迷宫网盘按键的信号与槽
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            conn[i * column + j] = connect(mazeButton[i * column + j], &QPushButton::clicked, [=](){
                onButtonClickedSetMaze(i, j);
            });
        }
    }
    //连接设置起点、终点、障碍等按键的信号与槽
    connect(startSetButton, &QPushButton::clicked, this, &PlayUserSetWidget::setupStartConnections);
    connect(endSetButton, &QPushButton::clicked, this, &PlayUserSetWidget::setupEndConnections);
    connect(barrierSetButton, &QPushButton::clicked, this, &PlayUserSetWidget::setupBarrierConnections);
    connect(finishButton, &QPushButton::clicked, this, &PlayUserSetWidget::finishUserSetmaze);
    connect(resetButton, &QPushButton::clicked, this, &PlayUserSetWidget::resetUserSetMaze);

    connect(resetUserWayButton, &QPushButton::clicked, this, &PlayUserSetWidget::resetUserWay);
    connect(sloveMazeByAButton, &QPushButton::clicked, this, &PlayUserSetWidget::solveMazeByA);
    connect(backUserSetButton, &QPushButton::clicked, this, &PlayUserSetWidget::backUserSet);
    connect(backModePageButton, &QPushButton::clicked, this, &PlayUserSetWidget::backModePage);
}
//断开设置迷宫按键的信号槽连接
void PlayUserSetWidget::breakMazeButtonConnections(){
    //连接内置信号和槽
    for (int i = 0; i < row; i++){
        for (int j = 0; j< column; j++){
            disconnect(conn[i * column + j]);
        }
    }
}

void PlayUserSetWidget::paintMaze(char *maze,int nowPosition[2])
{
    QPainter painter(this);
    QPen pen;           //用于绘制障碍
    QPen penStart;      //用于绘制起点的笔
    QPen penEnd;        //用于绘制终点的笔
    QPen penWay;        //用于绘制路径的笔
    QPen penNow;        //用于绘制现在位置的笔
    QPen penBestWay;    //用于绘制求解的路径

    pen.setColor(QColor(Qt::blue));//设置笔颜色
    pen.setWidth(WIDTH);//设置笔宽度
    penStart.setColor(QColor(Qt::yellow));
    penStart.setWidth(WIDTH / 2);
    penEnd.setColor(QColor(Qt::red));
    penEnd.setWidth(WIDTH / 2);
    penWay.setColor(QColor(Qt::green));
    penWay.setWidth(WIDTH / 4);   
    penNow.setColor(QColor(Qt::black));
    penNow.setWidth(WIDTH / 3);   
    penBestWay.setColor(QColor(Qt::cyan));
    penBestWay.setWidth(WIDTH / 4);

    //颜色注释
    if (isPlayMaze == true){
        painter.setPen(pen);
        painter.drawPoint(QPoint((column + 1) * WIDTH, 5 * (column * WIDTH / 11) + WIDTH / 2));
        painter.setPen(penNow);
        painter.drawPoint(QPoint((column + 1) * WIDTH, 6 * (column * WIDTH / 11) + WIDTH / 2));
        painter.setPen(penStart);
        painter.drawPoint(QPoint((column + 1) * WIDTH, 7 * (column * WIDTH / 11) + WIDTH / 2));
        painter.setPen(penEnd);
        painter.drawPoint(QPoint((column + 1) * WIDTH, 8 * (column * WIDTH / 11) + WIDTH / 2));
        painter.setPen(penWay);
        painter.drawPoint(QPoint((column + 1) * WIDTH, 9 * (column * WIDTH / 11) + WIDTH / 2));
        painter.setPen(penBestWay);
        painter.drawPoint(QPoint((column + 1) * WIDTH, 10 * (column * WIDTH / 11) + WIDTH / 2));
    }

    for (int i=0;i<row;i++){
        for (int j=0;j<column;j++){
            if (maze[i * column + j] == '0') {
                painter.setPen(pen);
                painter.drawPoint(QPoint(j * WIDTH + WIDTH / 2, i * WIDTH + WIDTH / 2));//画点
            }
            else if (maze[i * column + j] == '!') {

            }
            else if (maze[i * column + j] == '@') {
                painter.setPen(penStart);
                painter.drawPoint(QPoint(j * WIDTH + WIDTH / 2, i * WIDTH + WIDTH / 2));
            }
            else if (maze[i * column + j] == '*') {
                painter.setPen(penEnd);
                painter.drawPoint(QPoint(j * WIDTH + WIDTH / 2, i * WIDTH + WIDTH / 2));
            }
            else if (maze[i * column + j] == '#') {
                painter.setPen(penWay);
                painter.drawPoint(QPoint(j * WIDTH + WIDTH / 2, i * WIDTH + WIDTH / 2));
            }
            else if (maze[i * column + j] == '?') {
                painter.setPen(penBestWay);
                painter.drawPoint(QPoint(j * WIDTH + WIDTH / 2, i * WIDTH + WIDTH / 2));
            }
        }
    }
    //绘制当前位置
    if (nowPosition[0] >= 0 && nowPosition[1] >= 0){
        painter.setPen(penNow);
        painter.drawPoint(QPoint(nowPosition[1] * WIDTH + WIDTH / 2, nowPosition[0] * WIDTH + WIDTH / 2));
    }
}
//显示用户自定义迷宫界面按键
void PlayUserSetWidget::showUserSet(){
    //隐藏迷宫网盘设置按键
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            mazeButton[i * column + j]->show();
        }
    }
    barrierSetButton->show();
    startSetButton->show();
    endSetButton->show();
    finishButton->show();
    resetButton->show();
    backModePageButton->show();
}
//隐藏自定义迷宫界面按键
void PlayUserSetWidget::hideUserSet(){
    //隐藏迷宫网盘设置按键
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            mazeButton[i * column + j]->hide();
        }
    }
    barrierSetButton->hide();
    startSetButton->hide();
    endSetButton->hide();
    finishButton->hide();
    resetButton->hide();
    backModePageButton->hide();
}
//显示游玩迷宫界面的按键
void PlayUserSetWidget::showPlay(){
    labelMoveNum->show();
    backUserSetButton->show();
    resetUserWayButton->show();
    sloveMazeByAButton->show();
    for (int i=0;i<6;i++){
        labelExplanation[i]->show();
    }
}
//隐藏游玩迷宫界面的按键
void PlayUserSetWidget::hidePlay(){
    labelMoveNum->hide();
    backUserSetButton->hide();
    resetUserWayButton->hide();
    sloveMazeByAButton->hide();
    for (int i=0;i<6;i++){
        labelExplanation[i]->hide();
    }
}
//通过设置迷宫按键来自定义迷宫障碍
void PlayUserSetWidget::onButtonClickedSetMaze(int rowTemp, int columnTemp){
    //跨线程调用
    QMetaObject::invokeMethod(this, [this,rowTemp,columnTemp](){
        if (rowTemp == 0 || rowTemp == row - 1){
            labelWaining->setText("这是边界");
        }
        else if (columnTemp == 0 || columnTemp == column - 1){
            labelWaining->setText("这是边界");
        }
        else if (mazeUser[rowTemp * column + columnTemp] == '!'){
            mazeUser[rowTemp * column + columnTemp] = '0';
            labelWaining->setText("障碍设置成功");
        }
        else if (mazeUser[rowTemp * column + columnTemp] == '0'){
            mazeUser[rowTemp * column + columnTemp] = '!';
            labelWaining->setText("恢复");
        }
        else if (mazeUser[rowTemp * column + columnTemp] == '@'){
            labelWaining->setText("这是起点");
        }
        else if (mazeUser[rowTemp * column + columnTemp] == '*'){
            labelWaining->setText("这是终点");
        }
        else {
            labelWaining->setText("异常设置");
        }
    }, Qt::QueuedConnection);
    //重绘迷宫
    update();
}
//通过迷宫按键自定义迷宫起点
void PlayUserSetWidget::onButtonClickedSetStart(int rowTemp, int columnTemp){
    //跨线程调用
    QMetaObject::invokeMethod(this, [this,rowTemp,columnTemp](){
        if (mazeUser[rowTemp * column + columnTemp] == '!'){
            if (isSetStart == false){
                //设置起点
                mazeUser[rowTemp * column + columnTemp] = '@';
                start[0] = rowTemp;
                start[1] = columnTemp;
                isSetStart = true;
                labelWaining->setText("起点设置成功");
            }
            else {
                labelWaining->setText("已有起点");
            }
        }
        else if (mazeUser[rowTemp * column + columnTemp] == '0'){
            labelWaining->setText("这是障碍");
        }
        else if (mazeUser[rowTemp * column + columnTemp] == '@'){
            mazeUser[rowTemp * column + columnTemp] = '!';
            //取消起点
            start[0] = -1;
            start[1] = -1;
            isSetStart = false;
            labelWaining->setText("恢复");
        }
        else if (mazeUser[rowTemp * column + columnTemp] == '*'){
            labelWaining->setText("这是终点");
        }
        else{
            labelWaining->setText("异常设置");
        }
    }, Qt::QueuedConnection);
    //重绘迷宫
    update();
}
//通过迷宫按键自定义终点
void PlayUserSetWidget::onButtonClickedSetEnd(int rowTemp, int columnTemp){
    //跨线程调用
    QMetaObject::invokeMethod(this, [this,rowTemp,columnTemp](){
        if (mazeUser[rowTemp * column + columnTemp] == '!'){
            if (isSetEnd == false){
                //设置终点
                mazeUser[rowTemp * column + columnTemp] = '*';
                end[0] = rowTemp;
                end[1] = columnTemp;
                isSetEnd = true;
                labelWaining->setText("成功设置终点");
            }
            else {
                labelWaining->setText("已有终点");
            }
        }
        else if (mazeUser[rowTemp * column + columnTemp] == '0'){
            labelWaining->setText("这是障碍");
        }
        else if (mazeUser[rowTemp * column + columnTemp] == '@'){
            labelWaining->setText("这是起点");
        }
        else if (mazeUser[rowTemp * column + columnTemp] == '*'){
            mazeUser[rowTemp * column + columnTemp] = '!';
            //取消终点
            end[0] = -1;
            end[1] = -1;
            isSetEnd = false;
            labelWaining->setText("恢复");
        }
        else{
            labelWaining->setText("异常设置");
        }
    }, Qt::QueuedConnection);
    //重绘迷宫
    update();
}
//设置迷宫设置按键与设置障碍槽的连接
void PlayUserSetWidget::setupBarrierConnections(){
    breakMazeButtonConnections();
    //连接内置信号和槽
    for (int i = 0; i < row; i++){
        for (int j = 0; j< column; j++){
            conn[i * column + j] = connect(mazeButton[i * column + j], &QPushButton::clicked, [=](){
                onButtonClickedSetMaze(i, j);
            });
        }
    }
}
//设置迷宫设置按键与设置起点槽的连接
void PlayUserSetWidget::setupStartConnections(){
    breakMazeButtonConnections();
    //连接内置信号和槽
    for (int i = 0; i < row; i++){
        for (int j = 0; j< column; j++){
            conn[i * column + j] = connect(mazeButton[i * column + j], &QPushButton::clicked, [=](){
                onButtonClickedSetStart(i, j);
            });
        }
    }
}
//设置迷宫设置按键与设置终点槽的连接
void PlayUserSetWidget::setupEndConnections(){
    breakMazeButtonConnections();
    //连接内置信号和槽
    for (int i = 0; i < row; i++){
        for (int j = 0; j< column; j++){
            conn[i * column + j] = connect(mazeButton[i * column + j], &QPushButton::clicked, [=](){
                onButtonClickedSetEnd(i, j);
            });
        }
    }
}
//重置迷宫，回到原点
void PlayUserSetWidget::resetUserSetMaze(){
    initMazeUser();
    update();
}
//完成自定义迷宫跳转游玩界面函数
void PlayUserSetWidget::finishUserSetmaze(){
    if (isSetEnd == false){
        QMessageBox::information(this, "迷宫","未设置终点");
        labelWaining->setText("未设置终点");
    }
    else if (isSetStart == false){
        QMessageBox::information(this, "迷宫","未设置起点");
        labelWaining->setText("未设置起点");
    }
    else{
        hideUserSet();
        showPlay();

        labelWaining->setText("迷宫设置完成");
        nowPosition[0] = start[0];
        nowPosition[1] = start[1];
        isEnableMove = true;
        isShowUserWay = true;
        isPlayMaze = true;

        update();
    }
}
//返回上一页
void PlayUserSetWidget::backUserSet(){
    hidePlay();
    showUserSet();
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            mazeWithUserWay[i * column + j] = '!';      //将mazeWithUserWay设置为空路
            mazeWithWay[i * column + j] = '!';          //将mazeWithWay设置为空路
        }
    }
    //将当前位置置为无效位置
    nowPosition[0] = -1;
    nowPosition[1] = -1;
    moveNumber = 0;

    isEnableMove = false;
    isShowUserWay = false;
    isShowBestWay = false;
    isPlayMaze = false;

    update();
}
//重置用户已走线路
void PlayUserSetWidget::resetUserWay(){
    nowPosition[0] = start[0];
    nowPosition[1] = start[1];
    //清空用户的已走路径数组
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            mazeWithUserWay[i * column + j] = '!';
        }
    }
    //步数清零
    moveNumber = 0;
    labelMoveNum->setText(QString("步数:%1").arg(moveNumber));

    update();
}
//A*算法寻路
void PlayUserSetWidget::solveMazeByA(){
    int result = 0;
    result = searchPath(mazeWithWay,mazeUser,start,end); //寻找起点到终点的路径，并存入有路径的迷宫的二维数组，用'?'表示路径
    //未找到可行的路线
    if (result == 0){
        QMessageBox::information(this, "迷宫","没有最佳路径");
    }
    //找到可行的路线
    else{
        if (isShowBestWay == false){
            isShowBestWay = true;
        }
        else{
            isShowBestWay = false;
        }
    }

    update();
}
//返回上一页
void PlayUserSetWidget::backModePage(){
    UserSetWidget *userSetWindow = new UserSetWidget();
    userSetWindow->show();
    this->close();
}
//绘制
void PlayUserSetWidget::paintEvent(QPaintEvent * event){
        //显示迷宫
        paintMaze(mazeUser,nowPosition);
        //是否显示求解的路径
        if (isShowBestWay == true){
            paintMaze(mazeWithWay,nowPosition);
        }
        //显示用户已走的路径
        if (isShowUserWay == true){
            paintMaze(mazeWithUserWay,nowPosition);
        }
}
//通过WSAD实现走动
void PlayUserSetWidget::keyPressEvent(QKeyEvent *event){
    if (isEnableMove == true){
        int moveResult = 0;
        switch (event->key())
        {
        case Qt::Key_D:
            nextPosition[0] = nowPosition[0];
            nextPosition[1] = nowPosition[1] + 1;
            moveResult = checkMove(mazeUser,nowPosition,nextPosition);
            break;
        case Qt::Key_W:
            nextPosition[0] = nowPosition[0] - 1;
            nextPosition[1] = nowPosition[1];
            moveResult = checkMove(mazeUser,nowPosition,nextPosition);
            break;
        case Qt::Key_A:
            nextPosition[0] = nowPosition[0];
            nextPosition[1] = nowPosition[1] - 1;
            moveResult = checkMove(mazeUser,nowPosition,nextPosition);
            break;
        case Qt::Key_S:
            nextPosition[0] = nowPosition[0] + 1;
            nextPosition[1] = nowPosition[1];
            moveResult = checkMove(mazeUser,nowPosition,nextPosition);
            break;
        default:
            labelWaining->setText("上(W)下(S)左(A)右(D)");
            break;
        }
        if (moveResult == 0){
            labelWaining->setText("不可走");
        }
        else if (moveResult == 2){
            labelWaining->setText("成功");
            mazeWithUserWay[nowPosition[0] * column + nowPosition[1]] = '#';
            QMessageBox::information(this, "迷宫","成功");
            moveNumber++;
        }
        else if (moveResult == 1){
            labelWaining->setText("移动");
            mazeWithUserWay[nowPosition[0] * column + nowPosition[1]] = '#';
            moveNumber++;
        }
        else {
            labelWaining->setText("异常移动");
        }
        labelMoveNum->setText(QString("步数:%1").arg(moveNumber));
        //重绘迷宫
        update();
    }
}