#include "maze_solve.h"

int row = 15;
int column = 15;

/* 迷宫生成函数 */ 
/**
 * @brief 生成迷宫
 * @param maze 二维数组存储迷宫信息
 * @return 返回0表示成功生成一个迷宫，返回1表示生成迷宫失败
 */
int createMaze(char *maze,int start[2],int end[2]){
    srand((unsigned int)time(NULL));

    int randNum = 0;
    int now[2] = {0,0};     //存储生成迷宫时当前走到的坐标
    int next[2] = {0,0};     //下一步的坐标
    int init_1 = ((row / 2) * (column / 2));    //初始点阵中'1'的个数
    int num_1 = 1;          //已经被确认的初始'1'的个数
    int noNext = 0;         //是否走,0表示下一步，4表示附近的'1'都是'!'
    int parent[row * column] = {0};    //用于记录变为'!'的'1'节点的父节点（上一个节点），解决随机回退的超时或死循环问题
    //生成一个初始点阵
    initMaze(maze, row, column);

    //随机生成一个起点（在迷宫边缘）和终点(选在起点的对边)
    createStartAndEnd(start, end, row, column);

    //当前位置设在起点
    now[0] = start[0];
    now[1] = start[1];
    next[0] = now[0];
    next[1] = now[1];
    maze[now[0] * column + now[1]] = '!';

    //走到下一个'1'，被走到过的'1'和途径的'0'标记为'!'
    while (num_1 < init_1)  //标记完所有'1'
    {
        randNum = rand() % 4;
        //寻找有效方向,即下一个为'1'的方向
        noNext = isDrawStep(randNum,noNext,now,next,maze,parent); 
        if (noNext != 0){
            do{
                randNum++;
                noNext = isDrawStep(randNum % 4,noNext,now,next,maze,parent);
            } while (noNext > 0 && noNext < 4);
        }
        //无有效方向，回上一个'!'
        if (noNext == 4){
            /* 最初的回退是随机的，可能会出现来回反复的随机陷入死循环或超时 */
            /* 利用记录每个变为'!'的'1'节点的父节点坐标来回退，避免死循环或超时 */
            noNext = isBackStep(randNum % 4,noNext,now,next,maze,parent);
            now[0] = next[0];
            now[1] = next[1];
        }
        //有有效方向，走到下一个位置
        else if (noNext == 0) {
            maze[next[0] * column + next[1]] = '!';
            maze[(now[0] + next[0]) / 2 * column + (now[1] + next[1]) / 2] = '!';
            now[0] = next[0];
            now[1] = next[1];
            num_1++;
        }
        //异常情况
        else {
            qDebug()<<"Create maze error!";
            return 1;
        }
    }

    //标记起点和终点
    maze[start[0] * column + start[1]] = '@';
    maze[end[0] * column + end[1]] = '*';
    
    return 0;
}

/**
 * @brief 生成初始迷宫阵列
 * @param maze 存储迷宫的数组
 * @param row 行数
 * @param column 列数
 */
void initMaze(char *maze, int row, int column){
    int i = 0, j = 0;
    for (i=0;i<row;i++){
        for (j=0;j<column;j++){
            maze[i * column + j] = '0';
        }
    }
    i = 1;
    j = 1;
    for (i=1;i<row;i+=2){
        for (j=1;j<column;j+=2){
            maze[i * column + j] = '1';
        }
    }
}

/**
 * @brief 在迷宫边缘生成起点和终点
 * @param start 存储起点位置
 * @param end 存储终点位置
 * @param row 行数
 * @param column 列数
 */
void createStartAndEnd(int start[2], int end[2], int row, int column){
    //随机生成一个起点（在迷宫边缘）
    srand((unsigned int)time(NULL));
    int randNum = rand() % 4;
    if (randNum == 0){
        start[0] = 1;
        start[1] = (rand() % (column / 2)) * 2 + 1; //随机生成一个奇数
    }
    else if(randNum == 1){
        start[0] = row - 2;
        start[1] = (rand() % (column / 2)) * 2 + 1;
    }
    else if(randNum == 2){
        start[1] = 1;
        start[0] = (rand() % (row / 2)) * 2 + 1;
    }
    else {
        start[1] = column - 2;
        start[0] = (rand() % (row / 2)) * 2 + 1;
    }
    //生成终点(选在起点的对边)
    randNum = rand();   //新的随机数
    if (start[0] == 1){
        end[0] = row - 2;
        end[1] = (randNum % (column / 2)) * 2 + 1;
    }
    else if (start[0] == (row - 2) ){
        end[0] = 1;
        end[1] = (randNum % (column / 2)) * 2 + 1;
    }
    else if (start[1] == 1){
        end[0] = (randNum % (row / 2)) * 2 + 1;
        end[1] = column - 2;
    }
    else if (start[1] == (column - 2) ) {
        end[0] = (randNum % (row / 2)) * 2 + 1;
        end[1] = 1;
    }
}

/**
 * @brief 判断下一步是否有效，即下一步是否为'1'
 * @param randNum 随机的具体方向，一共四种方向（右0，上1，左2，下3）
 * @param noNext 是否不下一步，0表示下一步
 * @param now 当前位置
 * @param next 下一步的位置
 * @param maze 迷宫的二维矩阵
 * @return 正常，返回noNext的值；异常，返回-1
 */
int isDrawStep(int randNum,int noNext,int now[2],int next[2],char *maze,int *parent){
    noNext++;
    //右走
    if (randNum == 0){
        if (now[1] + 2 < column){
            if (maze[now[0] * column + (now[1] + 2)] == '1'){
                next[1] = now[1] + 2;
                parent[next[0] * column + next[1]] = now[0] * column + now[1];
                noNext = 0;
            }
        }
    }
    else if (randNum == 1){
        if (now[0] - 2 > 0){
            if (maze[(now[0] - 2) * column + now[1]] == '1'){
                next[0] = now[0] - 2;
                parent[next[0] * column + next[1]] = now[0] * column + now[1];
                noNext = 0;
            }
        }
    }
    else if (randNum == 2){
        if (now[1] - 2 > 0){
            if (maze[now[0] * column + (now[1] - 2)] == '1'){
                next[1] = now[1] - 2;
                parent[next[0] * column + next[1]] = now[0] * column + now[1];
                noNext = 0;
            }
        }
    }
    else if (randNum == 3){
        if (now[0] + 2 < row){
            if (maze[(now[0] + 2) *column + now[1]] == '1'){
                next[0] = now[0] + 2;
                parent[next[0] * column + next[1]] = now[0] * column + now[1];
                noNext = 0;
            }
        }
    }
    else {
        printf("Is_draw_step error!\n");
        return -1;
    }
    return noNext;
}

/**
 * @brief 回退
 * @param randNum 随机的具体方向，一共四种方向（右0，上1，左2，下3）
 * @param noNext 是否不下一步，0表示下一步
 * @param now 当前位置
 * @param next 下一步的位置
 * @param maze 迷宫的二维矩阵
 * @return 正常，返回noNext的值；异常，返回-1（返回值在利用父节点回退机制下无作用）
 */
int isBackStep(int randNum,int noNext,int now[2],int next[2],char *maze, int *parent){
    /* 现父节点回退机制 */
    next[0] = parent[now[0] * column + now[1]] / column;
    next[1] = parent[now[0] * column + now[1]] - next[0] * column;
    noNext = 0;
    return noNext;
}

/**
 * @brief 显示迷宫
 * @param maze0 二维数组存储迷宫信息
 * @return 返回0表示成功显示迷宫，返回1表示显示迷宫失败
 */
int showMaze(char *maze0){
    int i = 0, j = 0;
    for (i=0;i<row;i++){
        for (j=0;j<column;j++){
            printf("%c",maze0[i * column + j]);
        }
        printf("\n");
    }
    return 0;
}

/**
 * @brief A*算法计算路径
 * @param mazeWithWay 存储A*算法中间过程的数据
 * @param maze 存储迷宫的数组
 * @param start 起点
 * @param end 终点
 * @return 找到路径，返回1;未找到路径，返回0;异常，返回-1
 */
int searchPath(char *mazeWithWay,char *maze,int start[2],int end[2]){
    Node *mazeNode = (Node *)malloc(row * column * sizeof(Node));       //存储节点信息
    if (mazeNode == NULL){
        return -1;
    }
    Node **openList = (Node **)malloc(row * column * sizeof(Node *));   //已计算过F值的节点
    if (openList == NULL){
        return -1;
    }
    Node **closeList = (Node **)malloc(row * column * sizeof(Node *));  //已取过的节点
    if (closeList == NULL){
        return -1;
    }
    int openListLen = 0;
    int closeListLen = 0;
    Node *isFindEnd = NULL;     //若找到终点，则置为终点节点的指针
    initAllNode(mazeNode);
    //从起点开始
    Node *nodeCurrent = NULL;
    writeNodeInfm(&mazeNode[start[0] * column + start[1]],NULL,abs(start[0] - end[0]) + abs(start[1] - end[1]),0,0,false); //写入起点信息
    openList[0] = &mazeNode[start[0] * column + start[1]];
    nodeCurrent = openList[0];
    nodeCurrent->inOpen = 1;
    openListLen++;

    while (openListLen > 0 ){
        nodeCurrent =  findMinOpen(openList,openListLen);   //找到openList中F值最小的节点
        openList[nodeCurrent->inOpen - 1] = NULL;
        openListLen--;
        nodeCurrent->inOpen = 0;
        isFindEnd = travelArround(nodeCurrent,mazeNode,openList,&openListLen,closeList,maze,end);   //遍历nodeCurrent四周的节点，并处理
        //将nodeCurrent导入closeList
        closeList[closeListLen] = nodeCurrent;
        closeListLen++;
        nodeCurrent->isInClose = 1;
        bubbleSort(openList, openListLen);
        if (isFindEnd != NULL) { break; }
    }
    //找到路径
    if (isFindEnd != NULL) {
        showWay(mazeWithWay,isFindEnd, maze);   //载入求解的路径
        free(mazeNode);
        free(openList);
        free(closeList);
        return 1;
    }
    //未找到路径
    else {
        free(mazeNode);
        free(openList);
        free(closeList);
        qDebug()<<"Fail to find way!";
        return 0;
    }

}

/**
 * @brief 初始化所有迷宫节点
 * @param mazeNode 迷宫节点的二维数组
 * @return 两节点的曼哈顿距离
 */
void initAllNode(Node *mazeNode){
    int i = 0, j = 0;
    for (i=0;i<row;i++){
        for (j=0;j<column;j++){
            mazeNode[i * column + j].parent = NULL;
            mazeNode[i * column + j].location.x = j;
            mazeNode[i * column + j].location.y = i;
            mazeNode[i * column + j].F = 0;
            mazeNode[i * column + j].G = 0;
            mazeNode[i * column + j].inOpen = 0;
            mazeNode[i * column + j].isInClose = 0;
        }
    }
}

/**
 * @brief 计算F值
 * @param node 节点1的地址
 * @param end 终点的坐标数组
 * @return F值
 */
int calculateF(Node *node, int end[2]){
    return abs(node->location.x - end[1]) * STDLEN + abs(node->location.y - end[0]) * STDLEN + node->parent->G + STDLEN;
}

/**
 * @brief 写入节点信息
 * @param node 节点地址
 * @param parent 节点的父节点
 * @param positionX 节点的x坐标
 * @param positionY 节点的y坐标
 * @param F 节点的F值
 * @param G 节点的G值
 * @param inOpen 节点的inOpen值，0表示不在openList中，其他表示该节点在openList中的位置
 * @param isInClose 节点是否在closeList中
 */
void writeNodeInfm(Node *node, Node *parent, int F, int G, int inOpen,bool isInClose){
    node->parent = parent;
    node->F = F;
    node->G = G;
    node->inOpen = inOpen;
    node->isInClose = isInClose;
}

/**
 * @brief 找到openList中F值最小的节点
 * @param openList openList的指针地址
 * @param openListLen openList的有效长度 
 * @return F值最小的节点的地址
 */
Node *findMinOpen(Node **openList, int openListLen){
    return openList[openListLen-1];
}

/**
 * @brief 遍历nodeCurrent四周的节点
 * @param nodeCurrent 当前节点地址
 * @param mazeNode 迷宫节点二维数组
 * @param openList openList的指针数组地址
 * @param openListLen openList的有效长度 
 * @param closeList closeList的指针数组地址
 * @param maze 迷宫二维数组
 * @param end 终点的坐标数组
 * @return 
 */
Node *travelArround(Node *nodeCurrent,Node *mazeNode,Node **openList,int *openListLen,Node **closeList,char *maze,int end[2]){
    int nowPositionX = nodeCurrent->location.x;
    int nowPositionY = nodeCurrent->location.y;
    Node *isFindEnd = NULL;
    //从右边开始遍历，逆时针方向
    Node *travelNode = &mazeNode[nowPositionY * column + (nowPositionX + 1)];
    isFindEnd = addressNode(travelNode, nodeCurrent, mazeNode, openList,openListLen, closeList, maze, end);
    if (isFindEnd != NULL){
        return isFindEnd;
    }
    //上边
    travelNode = &mazeNode[(nowPositionY - 1) * column + nowPositionX];
    isFindEnd = addressNode(travelNode, nodeCurrent, mazeNode, openList,openListLen, closeList, maze, end);
    if (isFindEnd != NULL){
        return isFindEnd;
    }
    //左边
    travelNode = &mazeNode[nowPositionY * column + (nowPositionX - 1)];
    isFindEnd = addressNode(travelNode, nodeCurrent, mazeNode, openList,openListLen, closeList, maze, end);
    if (isFindEnd != NULL){
        return isFindEnd;
    }
    //下边
    travelNode = &mazeNode[(nowPositionY + 1) * column + nowPositionX];
    isFindEnd = addressNode(travelNode, nodeCurrent, mazeNode, openList,openListLen, closeList, maze, end);
    if (isFindEnd != NULL){
        return isFindEnd;
    }
    return NULL;
}

/**
 * @brief 处理nodeCurrent四周的一个节点
 * @param travelNode 遍历到的节点
 * @param nodeCurrent 当前节点地址
 * @param mazeNode 迷宫节点二维数组
 * @param openList openList的指针数组地址
 * @param openListLen openList的有效长度 
 * @param closeList closeList的指针数组地址
 * @param maze 迷宫二维数组
 * @param end 终点的坐标数组
 * @return 没有到终点，返回NULL；到达终点，返回终点节点指针
 */
Node *addressNode(Node *travelNode, Node *nodeCurrent, Node *mazeNode, Node **openList,int *openListLen, Node **closeList, char *maze,int end[2]){
    //判断是否是有效节点，即是否是路
    if (maze[travelNode->location.y * column + travelNode->location.x] == '!'){
        //travelNode在openList中
        if (travelNode->inOpen != 0 ) {
            if (travelNode->G > nodeCurrent->G + STDLEN){
                travelNode->parent = nodeCurrent;
                travelNode->F = travelNode->F - travelNode->G + nodeCurrent->G + STDLEN;
                travelNode->G = nodeCurrent->G + STDLEN;
            }
        }
        //travelNode不在openList中,也不在closeList中
        else if ( travelNode->isInClose == 0){
            travelNode->parent = nodeCurrent;
            writeNodeInfm(travelNode, nodeCurrent, calculateF(travelNode,end), travelNode->parent->G + STDLEN, *openListLen, 0);
            openList[*openListLen] = travelNode;
            (*openListLen) ++;
        }
        else{

        }
    }
    //处理到达终点
    else if (travelNode->location.y == end[0] && travelNode->location.x == end[1]) {
        travelNode->parent = nodeCurrent;
        writeNodeInfm(travelNode, nodeCurrent, calculateF(travelNode,end), travelNode->parent->G + STDLEN, *openListLen, 0);
        openList[*openListLen] = travelNode;
        (*openListLen) ++;
        return travelNode;
    }
    else {

    }
    return NULL;
}

/**
 * @brief 载入A*找到的路径到迷宫数组中
 * @param mazeWithWay A*算法得到的节点路径
 * @param isFindEnd 终点节点的指针
 * @param maze 存储迷宫的数组
 */
void showWay(char *mazeWithWay,Node *isFindEnd,char *maze){
    Node *nowNode = isFindEnd;
    while ( nowNode != NULL){
        mazeWithWay[nowNode->location.y * column + nowNode->location.x] = '?';
        nowNode = nowNode->parent;
    }
}

/**
 * @brief 冒泡排序
 * @param openList 
 * @param openListLen openList数组的长度
 */
void bubbleSort(Node **openList, int openListLen){
    Node *ptemp = NULL;
    int i = 0,j = openListLen-1;
    for (i = 0;i < openListLen;i++){
        for (j = openListLen - 1;j > i;j--){
            if (openList[j]->F > openList[j-1]->F){
                ptemp = openList[j];
                openList[j] = openList[j-1];
                openList[j-1] = ptemp;
            }
        }
        j = openListLen - 1;
    }
    i = 0;
    for (i = 0;i < openListLen;i++){
        openList[i]->inOpen = i + 1;
    }
}

/**
 * @brief 检查判断移动
 * @param nowPosition 当前位置，数组的指针
 * @param nextPosition 下一步的位置，数组的指针
 * @return 0:遇到障碍; 1:下一步为路，可走; 2:走到终点; -1:其他情况，如超出迷宫边界
 */
int checkMove(char *maze,int nowPosition[2],int nextPosition[2]){
    //下一步超出迷宫范围（超出二维数组的索引范围）
    if (nextPosition[0] < 0 || nextPosition[0] > row - 1){
        //qDebug()<<"out maze";
        return -1;
    }
    else if (nextPosition[1] < 0 || nextPosition[1] > column - 1){
        //qDebug()<<"out maze";
        return -1;
    }
    //障碍不走
    else if (maze[nextPosition[0] * column + nextPosition[1]] == '0'){
        //qDebug()<<"disable";
        return 0;
    }
    //路(包括起点)可走
    else if (maze[nextPosition[0] * column + nextPosition[1]] == '!' || maze[nextPosition[0] * column + nextPosition[1]] == '@'){
        nowPosition[0] = nextPosition[0];
        nowPosition[1] = nextPosition[1];
        return 1;
    }
    //已走过的路(包括求解的路)
    else if (maze[nextPosition[0] * column + nextPosition[1]] == '#' || maze[nextPosition[0] * column + nextPosition[1]] == '?'){
        nowPosition[0] = nextPosition[0];
        nowPosition[1] = nextPosition[1];
        return 1;
    }
    //终点
    else if (maze[nextPosition[0] * column + nextPosition[1]] == '*') {
        nowPosition[0] = nextPosition[0];
        nowPosition[1] = nextPosition[1];
        //qDebug()<<"Success";
        return 2;
    }
    else{
        return -1;
    }
}

/**
 * @brief 随机自动生成自然分岔型的迷宫
 * @param maze 存储迷宫的数组
 * @param start 起点位置
 * @param end 终点位置
 * @param row 迷宫行数，包括边界
 * @param column 迷宫列数，包括边界
 */
void createMazeNatural(char *maze, int start[2], int end[2], int row, int column){
    int now[2] = {0,0};     //存储生成迷宫时当前走到的坐标
    int next[2] = {0,0};     //下一步的坐标
    int last[2] = {0,0};     //上一个的坐标
    int num_0 = 0;          //被标记为')'的'0'的个数
    int listChoosed0[row * column] = {0};   //存储变为')'的'0'的位置，
    //生成一个初始点阵
    initMaze(maze, row, column);

    //随机生成一个起点（在迷宫边缘）和终点(选在起点的对边)
    createStartAndEnd(start, end, row, column);

    //当前位置设在起点
    now[0] = start[0];
    now[1] = start[1];
    next[0] = now[0];
    next[1] = now[1];
    last[0] = -1;
    last[1] = -1;
    maze[now[0] * column + now[1]] = '!';

    do{
        if (last[0] != now[0] || last[1] != now[1]){
            //标记目前位置周围的'0'为')'，除边界的'0'外
            mackArround0(maze, now, &num_0, listChoosed0);
        }
        //在周围随机选一个')'，走一步
        randChooseArround(maze, now, last, num_0, listChoosed0);
        sortListChoosed(listChoosed0, num_0);
        num_0--;
    }while (num_0 > 0);
    //迷宫起点和终点的设置
    maze[start[0] * column + start[1]] = '@';
    maze[end[0] * column + end[1]] = '*';
}

/**
 * @brief 标记当前位置附近的0节点
 * @param maze 存储迷宫的数组
 * @param now 当前位置
 * @param num_0 被标记过的0的总数，指针
 * @param listChoose0 存储被标记过的0的位置的数组
 */
void mackArround0(char *maze, int now[2], int *num_0, int *listChoose0){
    //printf("into mack\n");
    if (now[1] + 1 < (column - 1) && maze[now[0] * column + now[1] + 1] == '0') {
        maze[now[0] * column + now[1] + 1] = ')';
        listChoose0[*num_0] = now[0] * column + now[1] + 1;
        (*num_0)++;
    }
    if (now[0] - 1 > 0 && maze[(now[0] - 1) * column + now[1]] == '0') {
        maze[(now[0] - 1) * column + now[1]] = ')';
        listChoose0[*num_0] = (now[0] - 1) * column + now[1];
        (*num_0)++;
    }
    if (now[1] - 1 > 0 && maze[now[0] * column + now[1] - 1] == '0') {
        maze[now[0] * column + now[1] - 1] = ')';
        listChoose0[*num_0] = now[0] * column + now[1] - 1;
        (*num_0)++;
    }
    if (now[0] + 1 < (row - 1) && maze[(now[0] + 1) * column + now[1]] == '0') {
        maze[(now[0] + 1) * column + now[1]] = ')';
        listChoose0[*num_0] = (now[0] + 1) * column + now[1];
        (*num_0)++;
    }
}

/**
 * @brief 随机选择当前位置附近的有效'1'，并将其变为'!'
 * @param maze 存储迷宫的数组
 * @param now 当前位置
 * @param last 上一步的位置
 * @param num_0 被标记过的0的总数，即listChoose0数组中的有效数据长度，指针
 * @param listChoose0 存储被标记过的0的位置的数组
 */
void randChooseArround(char *maze, int now[2],int last[2], int num_0, int *listChoosed0){
    //printf("into randChoose\n");
    int randNum = rand() % num_0;
    int choosedRow = listChoosed0[randNum] / column;
    int choosedColumn = listChoosed0[randNum] - choosedRow * column;
    last[0] = now[0];
    last[1] = now[1];
    //搜索选中的')'四周未变为'!'的'1'
    if (maze[choosedRow * column + (choosedColumn + 1)] == '1'){
        now[0] = choosedRow;
        now[1] = choosedColumn + 1;
        maze[choosedRow * column + (choosedColumn + 1)] = '!';  //走到的'1'变为'!'
        maze[listChoosed0[randNum]] = '!';                      //途径的')'变为'!'
    }
    else if (maze[(choosedRow - 1) * column + choosedColumn] == '1'){
        now[0] = choosedRow - 1;
        now[1] = choosedColumn;
        maze[(choosedRow - 1)* column + choosedColumn] = '!';  //走到的'1'变为'!'
        maze[listChoosed0[randNum]] = '!';                      //途径的')'变为'!'
    }
    else if (maze[choosedRow * column + choosedColumn - 1] == '1'){
        now[0] = choosedRow;
        now[1] = choosedColumn - 1;
        maze[choosedRow* column + choosedColumn - 1] = '!';  //走到的'1'变为'!'
        maze[listChoosed0[randNum]] = '!';                      //途径的')'变为'!'
    }
    else if (maze[(choosedRow + 1) * column + choosedColumn] == '1'){
        now[0] = choosedRow + 1;
        now[1] = choosedColumn;
        maze[(choosedRow + 1)* column + choosedColumn] = '!';  //走到的'1'变为'!'
        maze[listChoosed0[randNum]] = '!';                      //途径的')'变为'!'
    }
    //四周没有'1'，')'变回'0'
    else {
        maze[listChoosed0[randNum]] = '0';
    }
    listChoosed0[randNum] = 0;      //选到的除出列表
}

/**
 * @brief 整理存储被标记为')'的数组，将')'变为'!'的存储（即0）后移
 * @param listChoose0 存储被标记过的0的位置的数组
 * @param num_0 被标记过的0的总数，即listChoose0数组中的有效数据长度
 */
void sortListChoosed(int *listChoosed0, int num_0){
    for (int i = 0; i < num_0 - 1; i++){
        if (listChoosed0[i] == 0){
            listChoosed0[i] = listChoosed0[i + 1];
            listChoosed0[i + 1] = 0;
        }
    }
}

/**
 * @brief 生成迷宫(带有块状空路)
 * @param maze 二维数组存储迷宫信息
 * @param start 起点
 * @param end 终点
 * @param row 迷宫行数，包括边界
 * @param column 迷宫列数，包括边界
 * @return 返回0表示成功生成一个迷宫，返回1表示生成迷宫失败
 */
int createMazeBlock(char *maze,int start[2],int end[2], int row, int column){
    srand((unsigned int)time(NULL));

    int i = 0;
    int randNum = 0;
    int now[2] = {0,0};     //存储生成迷宫时当前走到的坐标
    int next[2] = {0,0};     //下一步的坐标
    int init_1 = ((row / 2) * (column / 2));    //初始点阵中'1'的个数
    int num_1 = 1;          //已经被确认的初始'1'的个数
    int noNext = 0;         //是否走,0表示下一步，4表示附近的'1'都是'!'
    int parent[row * column] = {0};    //用于记录变为'!'的'1'节点的父节点（上一个节点），解决随机回退的超时或死循环问题
    int blockNum = 0;
    int blockPosition[10] = {0};        //存储生成的路块的位置
    int blockSize[10] = {0};            //存储生成的路块的大小
    int blockPositionLen = 0;
    int flagOK = 1;                     //判断生成的路块是否能正常生成迷宫，即剩余区域是否仍然是一块区域
    int blockLocation[2] = {0};     //一个路块的坐标
    int blockLocationTemp[2] = {0};
    int position[2] = {0};
    //int blockAStart[2] = {0};
    char *mazeCopy = (char *)malloc(row * column * sizeof(char));       //用于复制迷宫
    if (mazeCopy == NULL){
        qDebug()<<"None mazeCopy";
        return -1;
    }
    char *mazeWayTemp = (char *)malloc(row * column * sizeof(char));       //为了配对写的A*算法参数的临时数组
    if (mazeWayTemp == NULL){
        qDebug()<<"None mazeCopy";
        free(mazeCopy);
        return -1;
    }

    //生成一个初始点阵
    initMaze(maze, row, column);

    //随机生成一个起点（在迷宫边缘）和终点(选在起点的对边)
    createStartAndEnd(start, end, row, column);
    maze[start[0] * column + start[1]] = '!';   //标记起点，防止在生成路块时被标为路块

    do{
        flagOK = 1;
        init_1 = ((row / 2) * (column / 2));
        blockPositionLen = 0;
        initMaze(mazeWayTemp, row, column);
        initMaze(maze, row, column);
        initMaze(mazeCopy, row, column);
        maze[start[0] * column + start[1]] = '!';
        //qDebug()<<"init";
        //生成路块
        if (row == 11) {
            //生成2个3x3的路块
            do{
                if (createBlock(maze, blockPosition, blockSize, 3, &blockPositionLen)){
                    blockNum++;
                    init_1 -= 4;
                }
            } while (blockNum < 2);
            blockNum = 0;
        }
        else if (row == 15) {
            //生成3个3x3的路块
            do{
                if (createBlock(maze, blockPosition, blockSize, 3, &blockPositionLen)){
                    blockNum++;
                    init_1 -= 4;
                }
            } while (blockNum < 3);
            blockNum = 0;
        }
        else if (row == 19) {
            //生成1个5x5和2个3x3的路块
            do{
                if(createBlock(maze, blockPosition, blockSize, 5, &blockPositionLen)){
                    blockNum++;
                    init_1 -= 9;
                }
            } while (blockNum < 1);
            blockNum = 0;
            do{
                if (createBlock(maze, blockPosition, blockSize, 3, &blockPositionLen)){
                    blockNum++;
                    init_1 -= 4;
                }
            } while (blockNum < 2);
            blockNum = 0;
        }
        else if (row == 23) {
            //生成2个5x5和2个3x3的路块
            do{
                if(createBlock(maze, blockPosition, blockSize, 5, &blockPositionLen)){
                    blockNum++;
                    init_1 -= 9;
                }
            } while (blockNum < 2);
            blockNum = 0;
            do{
                if (createBlock(maze, blockPosition, blockSize, 3, &blockPositionLen)){
                    blockNum++;
                    init_1 -= 4;
                }
            } while (blockNum < 2);
            blockNum = 0;
        }
        else if (row == 27) {
            //生成2个5x5和3个3x3的路块
            do{
                if(createBlock(maze, blockPosition, blockSize, 5, &blockPositionLen)){
                    blockNum++;
                    init_1 -= 9;
                }
            } while (blockNum < 2);
            blockNum = 0;
            do{
                if (createBlock(maze, blockPosition, blockSize, 3, &blockPositionLen)){
                    blockNum++;
                    init_1 -= 4;
                }
            } while (blockNum < 3);
            blockNum = 0;
        }
        //qDebug()<<"creaate block";

        for (int i=1;i < row-1;i++){    //将路块视为障碍，为涉及的点视为可走的路
            for (int j=1;j < column-1;j++){
                if (maze[i * column + j] == '!'){
                    mazeCopy[i * column + j] = '0';
                }
                else if (maze[i * column + j] == '1'){
                    mazeCopy[i * column + j] = '!';
                }
                else if (maze[i * column + j] == '0'){
                    mazeCopy[i * column + j] = '!';
                }
                else{
                    qDebug()<<"Block error!";
                }
                mazeCopy[start[0] * column + start[1]] = '@';   //起点不变，依旧视为可走的路
            }
        }
        for (int i=0;i<blockPositionLen;i++){
            blockLocation[0] = blockPosition[i] / column;
            blockLocation[1] = blockPosition[i] - blockLocation[0] * column;
            for (int j=0;j<(blockSize[i] + 1)*4;j++){   //对所有路块周围一圈也置为障碍
                if (j < (blockSize[i] + 1)){
                    blockLocationTemp[0] = blockLocation[0] - 1;
                    blockLocationTemp[1] = blockLocation[1] - 1 + j;
                }
                else if (j < 2 * (blockSize[i] + 1)){
                    blockLocationTemp[0] = blockLocation[0] - 1 + j - (blockSize[i] + 1);
                    blockLocationTemp[1] = blockLocation[1] + blockSize[i];
                }
                else if (j < 3 * (blockSize[i] + 1)){
                    blockLocationTemp[0] = blockLocation[0] + blockSize[i];
                    blockLocationTemp[1] = blockLocation[1] + blockSize[i] - (j - 2 * (blockSize[i] + 1));
                }
                else{
                    blockLocationTemp[0] = blockLocation[0] + blockSize[i] - (j - 3 * (blockSize[i] + 1));
                    blockLocationTemp[1] = blockLocation[1] - 1;
                }
                mazeCopy[blockLocationTemp[0] * column + blockLocationTemp[1]] = '0';
            }
        }

        // //显示变换后的迷宫
        // for (int i=0;i<row;i++){
        //     for (int j=0;j<column;j++){
        //         printf("%c",mazeCopy[i*column+j]);
        //     }
        //     printf("\n");
        // }

        for (int i=1;i<row-1;i++){
            for (int j=1;j<column-1;j++){
                if (i == start[0] && j == start[1]){ continue; }
                if (mazeCopy[i * column + j] == '!'){
                    mazeCopy[i * column + j] = '*';
                    position[0] = i;
                    position[1] = j;
                    if (searchPath(mazeWayTemp, mazeCopy, start, position) == 0){
                        flagOK = 0;
                        mazeCopy[i * column + j] = '!';
                        break;
                    }
                    mazeCopy[i * column + j] = '!';
                }
            }
            if (flagOK == 0) { break; }
        }
    }while(flagOK == 0);
    //qDebug()<<"success";

    //当前位置设在起点
    now[0] = start[0];
    now[1] = start[1];
    next[0] = now[0];
    next[1] = now[1];
    maze[now[0] * column + now[1]] = '!';

    //走到下一个'1'，被走到过的'1'和途径的'0'标记为'!'
    while (num_1 < init_1)  //标记完所有'1'
    {
        //qDebug()<<num_1;
        randNum = rand() % 4;
        //寻找有效方向,即下一个为'1'的方向
        noNext = isDrawStep(randNum,noNext,now,next,maze,parent); 
        if (noNext != 0){
            do{
                randNum++;
                noNext = isDrawStep(randNum % 4,noNext,now,next,maze,parent);
            } while (noNext > 0 && noNext < 4);
        }
        //无有效方向，回上一个'!'
        if (noNext == 4){
            /* 最初的回退是随机的，可能会出现来回反复的随机陷入死循环或超时 */
            /* 利用记录每个变为'!'的'1'节点的父节点坐标来回退，避免死循环或超时 */
            noNext = isBackStep(randNum % 4,noNext,now,next,maze,parent);
            now[0] = next[0];
            now[1] = next[1];
        }
        //有有效方向，走到下一个位置
        else if (noNext == 0) {
            maze[next[0] * column + next[1]] = '!';
            maze[(now[0] + next[0]) / 2 * column + (now[1] + next[1]) / 2] = '!';
            now[0] = next[0];
            now[1] = next[1];
            num_1++;
        }
        //异常情况
        else {
            qDebug()<<"Create maze error!";
            return 1;
        }
    }

    //连接路块与生成的路
    i = 0;
    int result = 0;
    for (i = 0; i < blockPositionLen; i++){
        do {
            randNum = rand() % (4 * (blockSize[i] - 1));
            result += isBuildBridge(maze,blockPosition[i] / column, blockPosition[i] - (blockPosition[i] / column) * column, blockSize[i], randNum);
        } while(result < 2);
        result = 0;
    }

    //标记起点和终点
    maze[start[0] * column + start[1]] = '@';
    maze[end[0] * column + end[1]] = '*';
    
    return 0;
}

/**
 * @brief 创建路块
 * @param maze 存储迷宫的数组
 * @param blockPosition 记录生成的路块左上角节点的位置的数组
 * @param blockSize 记录生成的路块的边长的数组
 * @param size 路块的边长
 * @param blockPositionLen blockPosition数组的有效长度，同时也是blockSize的有效长度，指针传入
 * @return 该位置的路块与已有路块有重叠，返回0;无重叠，生成的路块有效，返回1
 */
int createBlock(char *maze, int *blockPosition ,int *blockSize, int size, int *blockPositionLen){
    int randRow = (rand() % ((row - size) / 2)) * 2 + 1;
    int randColumn = (rand() % ((row - size) / 2)) * 2 + 1;
    //检查该位置的路块是否与已有的路块重叠
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (maze[(randRow + i) * column + randColumn + j] == '!'){
                return 0;
            }
        }
    }
    //若无重叠，则加入迷宫
    blockPosition[*blockPositionLen] = randRow * column + randColumn;
    blockSize[*blockPositionLen] = size;
    (*blockPositionLen)++;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            maze[(randRow + i) * column + randColumn + j] = '!';
        }
    }
    return 1;
}

/**
 * @brief 将路块与随机生成的路建一个通道
 * @param maze 存储迷宫的数组
 * @param blockBdrRow 路块的左上角节点的行数
 * @param blockBdrColumn 路块的左上角节点的列数
 * @param size 路块的边长
 * @param nodeNum 要建一个通道的位置，路块的左上角为0，绕路块边界顺时针递增
 */
int isBuildBridge(char *maze,int bolckBdrRow, int blockBdrColumn, int size, int nodeNum){
    int result = 0;
    int nodePosition[2];
    if (nodeNum < (size - 1)){
        nodePosition[0] = bolckBdrRow;
        nodePosition[1] = blockBdrColumn + nodeNum;
        if (nodePosition[0] - 2 > 0){
            if (maze[(nodePosition[0] - 2) * column + nodePosition[1]] == '!'){
                maze[(nodePosition[0] - 1) * column + nodePosition[1]] = '!';
                result = 1;
            }
        }
    }
    else if (nodeNum < (size - 1) * 2){
        nodePosition[0] = bolckBdrRow + nodeNum - (size - 1);
        nodePosition[1] = blockBdrColumn + size - 1;
        if (nodePosition[1] + 2 < column - 1){
            if (maze[nodePosition[0] * column + nodePosition[1] + 2] == '!'){
                maze[nodePosition[0] * column + nodePosition[1] + 1] = '!';
                result = 1;
            }
        }
    }
    else if (nodeNum < (size - 1) * 3){
        nodePosition[0] = bolckBdrRow + size - 1;
        nodePosition[1] = blockBdrColumn + 3 * (size - 1) - nodeNum;
        if (nodePosition[0] + 2 < row - 1){
            if (maze[(nodePosition[0] + 2) * column + nodePosition[1]] == '!'){
                maze[(nodePosition[0] + 1) * column + nodePosition[1]] = '!';
                result = 1;
            }
        }
    }
    else{
        nodePosition[0] = bolckBdrRow + 4 * (size - 1) - nodeNum;
        nodePosition[1] = blockBdrColumn;
        if (nodePosition[1] - 2 > 0){
            if (maze[nodePosition[0] * column + nodePosition[1] - 2] == '!'){
                maze[nodePosition[0] * column + nodePosition[1] - 1] = '!';
                result = 1;
            }
        }
    }
    return result;
}
