#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <vector>
#include <QMap>
#include <QColor>
const int BLOCK_SIZE=100; //单个方块的边长
const int MARGIN=5;//场景边距
const int N=4; //N*N 宫格

namespace Ui {
class Widget;
}

//方向
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void init();
    void BLOCKMove(Direction dir);
    void addRandom();
    void popFail();
    void popSuccess();
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    std::vector<std::vector<int>> vecs;
    QMap<int, QColor> map;
    int emptyLocation;
    Ui::Widget *ui;
};

#endif // WIDGET_H
