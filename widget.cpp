#include <QKeyEvent>
#include <time.h>
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
    time_t t;
    srand((unsigned) time(&t));
    int i = rand() % N;
    int j = rand() % N;
    vecs[i][j]=2;
    i = rand() % N;
    j = rand() % N;
    vecs[i][j]=2;
    i = rand() % N;
    j = rand() % N;
    vecs[i][j]=2;
    i = rand() % N;
    j = rand() % N;
    vecs[i][j]=2;
    init();
}

Widget::~Widget()
{
    delete ui;
}

//绘制游戏界面
void Widget::init() {
    update();
}

void Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            QRectF rectangle(j*(BLOCK_SIZE+MARGIN)+MARGIN, i*(BLOCK_SIZE+MARGIN)+MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            painter.drawRoundedRect(rectangle, 15.0, 15.0);
            painter.drawText(rectangle, Qt::AlignCenter, QString::number(vecs[i][j]));
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
    update();
}
