#ifndef GAME_H
#define GAME_H
#include <QBrush>
#include <QFont>
#include <QPen>
#include "game.h"
#include "usp.h"
#include "ball.h"
//#include "vector.h"
QT_BEGIN_NAMESPACE
class QPainter;
class QPaintEvent;
QT_END_NAMESPACE

//! [0]


class Game
{
private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    Ball *balls[10000];
    double width,height;
    USP *usp;
public:
    Game();
     void paint(QPainter *painter, QPaintEvent *event, int elapsed);

};

#endif // GAME_H
