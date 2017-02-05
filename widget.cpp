#include <QKeyEvent>
#include <QMessageBox>
#include <time.h>
#include <iostream>
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //调整窗口尺寸布局
    resize(BLOCK_SIZE*N+MARGIN*(N+1), BLOCK_SIZE*N+MARGIN*(N+1));
    vecs = std::vector<std::vector<int>>(N, std::vector<int>(N,0));
    emptyLocation=N*N;
    addRandom();
    init();
}

Widget::~Widget()
{
    delete ui;
}

//绘制游戏界面
void Widget::init() {
    map[0] = QColor(255,255,255);
    map[2] = QColor(255,255,0);
    map[4] = QColor(255,204,0);
    map[8] = QColor(255,153,0);
    map[16] = QColor(255,102,0);
    map[32] = QColor(255,51,0);
    map[64] = QColor(255,0,0);
    map[128] = QColor(255,0,102);
    map[256] = QColor(255,0,153);
    map[1024] = QColor(255,0,204);
    map[2048] = QColor(255,0,255);
    update();
}

void Widget::paintEvent(QPaintEvent *event) {
    QFont font("Arial",20,QFont::Bold,true);
    QPainter painter(this);
    painter.setFont(font);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            int num = vecs[i][j];
            QRectF rectangle(j*(BLOCK_SIZE+MARGIN)+MARGIN, i*(BLOCK_SIZE+MARGIN)+MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            painter.setPen(Qt::blue);
            painter.setBrush(map[num]);
            painter.drawRoundedRect(rectangle, 15.0, 15.0);
            painter.drawText(rectangle, Qt::AlignCenter, QString::number(num));
        }
    }
}

//capture the keymove
void Widget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Up:
        BLOCKMove(UP);
        break;
    case Qt::Key_Down:
        BLOCKMove(DOWN);
        break;
    case Qt::Key_Left:
        BLOCKMove(LEFT);
        break;
    case Qt::Key_Right:
        BLOCKMove(RIGHT);
        break;
    case Qt::Key_Space:
        BLOCKMove(SPACE);
    }
}

void Widget::BLOCKMove(Direction dir) {
    std::vector<std::vector<int>> tmp(std::vector<std::vector<int>>(N, std::vector<int>(N,0)));
    switch(dir) {
    case UP:
        for(int j=0;j<N;j++) { //列
            std::vector<int> arr;
            for(int i=0;i<N;i++) { //行
                if(vecs[i][j] != 0) {
                   arr.push_back(vecs[i][j]);
                }
            }
            int pos=0;
            //将隔壁相同的数字合并
            for(int k = 0; k<arr.size(); k++) {
                if(pos>=1 && arr[k]==arr[k-1]) {
                    tmp[pos-1][j] += arr[k];
                    arr[k] =0;
                    if(tmp[pos-1][j]==2048) popSuccess();
                    emptyLocation++;
                } else {
                    tmp[pos][j] = arr[k];
                    pos++;
                }
            }
        }
        break;
    case DOWN:
        for(int j=0;j<N;j++) { //列
            std::vector<int> arr;
            for(int i=N-1;i>=0;i--) { //行
                if(vecs[i][j] != 0) {
                   arr.push_back(vecs[i][j]);
                }
            }
            int pos=N-1;
            //将隔壁相同的数字合并
            for(int k = 0; k<arr.size(); k++) {
                if(pos<N-1 && arr[k]==arr[k-1]) {
                    tmp[pos+1][j] += arr[k];
                    arr[k] =0;
                    if(tmp[pos+1][j]==2048) popSuccess();
                    emptyLocation++;
                } else {
                    tmp[pos][j] = arr[k];
                    pos--;
                }
            }
        }
        break;
    case LEFT:
        for(int i=0;i<N;i++) { //行
            std::vector<int> arr;
            for(int j=0;j<N;j++) { //列
                if(vecs[i][j] != 0) {
                   arr.push_back(vecs[i][j]);
                }
            }
            int pos=0;
            //将隔壁相同的数字合并
            for(int k = 0; k<arr.size(); k++) {
                if(pos>=1 && arr[k]==arr[k-1]) {
                    tmp[i][pos-1]+= arr[k];
                    arr[k] =0;
                    if(tmp[i][pos-1]==2048) popSuccess();
                    emptyLocation++;
                } else {
                    tmp[i][pos] = arr[k];
                    pos++;
                }
            }
        }
        break;
    case RIGHT:
        for(int i=0;i<N;i++) { //列
            std::vector<int> arr;
            for(int j=N-1;j>=0;j--) { //行
                if(vecs[i][j] != 0) {
                   arr.push_back(vecs[i][j]);
                }
            }
            int pos=N-1;
            //将隔壁相同的数字合并
            for(int k = 0; k<arr.size(); k++) {
                if(pos<N-1 && arr[k]==arr[k-1]) {
                    tmp[i][pos+1] += arr[k];
                    arr[k] =0;
                    if(tmp[i][pos+1]==2048) popSuccess();
                    emptyLocation++;
                } else {
                    tmp[i][pos] = arr[k];
                    pos--;
                }
            }
        }
        break;
    case SPACE:
        break;
    }
    vecs = tmp;
    addRandom(); //在空的位置上随机选择一个填上2
    update();
}

void Widget::addRandom() {
    if(emptyLocation==0) {
        popFail();
        return;
    }
    time_t t;
    srand((unsigned) time(&t));
    int index = rand() % emptyLocation;
    srand((unsigned) time(&t));

    for(int i=0;i<N;i++) {
        for(int j=0;j<N;j++) {
            if(vecs[i][j]==0) {
                if(index==0) {
                    vecs[i][j] =(rand()%2+1)*2;
                    emptyLocation--;
                    return;
                }
                index--;
            }
        }
    }
}

void Widget::popFail() {
QMessageBox::information(this,"Information",tr("Fail"));
}

void Widget::popSuccess() {
QMessageBox::information(this,"Information",tr("Success"));
}


