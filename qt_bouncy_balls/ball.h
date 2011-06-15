#ifndef BALL_H
#define BALL_H
#include <QBrush>
#include <QFont>
#include <QPen>
#include "vector.h"

/*struct Bounds{
    double centerx,centery,radius;
};*/

class Ball
{
public:
    Ball();
    Ball(double,double,double,double,Vector*);
    ~Ball();
    void move(double);
    void draw(QPainter *);
    bool ballCollision(Ball* b);
    double getX();
    double getY();
    void setX(double);
    void setY(double);
    double getRadius();
    double getMass();
    double getCenterX();
    double getCenterY();
    Vector* getVector();
    void setVector(Vector*);
    double distance(Ball*);
    double getXDistance(Ball*);
    double getYDistance(Ball*);
    void resolveCollision(Ball*);
    void gravity(Ball*);
    //void getBounds(struct Bounds*);
private:
    double x,y,width,height,radius,mass;
    Vector *vector;
    double angle;
    QBrush circleBrush;
    QPen circlePen;
};

#endif // BALL_H
