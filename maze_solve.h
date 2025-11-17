#ifndef MAZE_SLOVE_H
#define MAZE_SOLVE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <QDebug>

// #define ROW 19          //迷宫行数
// #define COLUMN 19       //迷宫列数
#define WIDTH 36        //一个点的边长（像素）
#define STDLEN 10       //求解迷宫时认为的单位长度
#define MAX 30      //最大行列数

extern int row;
extern int column;

typedef struct Position{
    int x;
    int y;
} Position;

typedef struct Node{
    Node *parent;
    Position location;
    int F;
    int G;
    int inOpen; //0表示不在openList中，其他表示该节点在openList中的位置
    bool isInClose;
} Node;

int createMaze(char *maze,int start[2],int end[2]);
void initMaze(char *maze, int row, int column);
void createStartAndEnd(int start[2], int end[2], int row, int column);
int isDrawStep(int randNum,int noNext,int now[2],int next[2],char *maze,int *parent);
int isBackStep(int randNum,int noNext,int now[2],int next[2],char *maze, int *parent);
int showMaze(char *maze);

int searchPath(char *mazeWithWay,char *maze,int start[2],int end[2]);
void initAllNode(Node *mazeNode);
int calculateF(Node *node, int end[2]);
void writeNodeInfm(Node *node, Node *parent, int F, int G, int inOpen,bool isInClose);
Node *findMinOpen(Node **openList, int openListLen);
Node *travelArround(Node *nodeCurrent,Node *mazeNode,Node **openList,int *openListLen,Node **closeList,char *maze,int end[2]);
Node *addressNode(Node *travelNode, Node *nodeCurrent, Node *mazeNode, Node **openList,int *openListLen, Node **closeList, char *maze,int end[2]);
void showWay(char *mazeWithWay,Node *isFindEnd,char *maze);
void bubbleSort(Node **openList, int openListLen);

int checkMove(char *maze,int nowPosition[2],int nextPosition[2]);

void createMazeNatural(char *maze, int start[2], int end[2], int row, int column);
void mackArround0(char *maze, int now[2], int *num_0, int *listChoose0);
void randChooseArround(char *maze, int now[2],int last[2], int num_0, int *listChoosed0);
void sortListChoosed(int *listChoosed0, int num_0);

int createMazeBlock(char *maze,int start[2],int end[2], int row, int column);
int createBlock(char *maze, int *blockPosition ,int *blockSize, int size, int *blockPositionLen);
int isBuildBridge(char *maze,int bolckBdrRow, int blockBdrColumn, int size, int nodeNum);

int createMazeBlockNew(char *maze,int start[2],int end[2], int row, int column);

#endif
