#include <QtGui>
#include "ball.h"
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>

#define RANDDOUBLE (rand()/(float(RAND_MAX)+1))
#define GRAV (0.0005)
#define MAXGRAV (300)
Ball::Ball(){
    struct timeval tv;
    gettimeofday(&tv,0);
    srand((long)tv.tv_usec);
    x=RANDDOUBLE*1000;
    y=RANDDOUBLE*600;
    width = height =2+RANDDOUBLE*10;
    radius = width/2.0;
    mass = 4.0/3.0 * 3.1415 * radius *radius*radius;
    vector = new Vector(RANDDOUBLE*2.0-1,RANDDOUBLE*2.0-1);
    QRadialGradient gradient(QPointF(500, 300), 300);
    gradient.setColorAt(0.0, Qt::red);
    gradient.setColorAt(0.5, QColor(0xa6, 0xce, 0x39));
    gradient.setColorAt(1.0, Qt::white);
    //std::cout<<x<<std::endl;
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
}

Ball::~Ball(){
    delete(vector);
}

void Ball::move(double elapsed){
   // std::cout<<x<<" "<<y<<" "<<vector->getX()<<" "<<vector->getY()<<std::endl;
    x+=vector->getX();
    y+=vector->getY();
    //std::cout<<x<<" "<<y<<" "<<vector->getX()<<" "<<vector->getY()<<std::endl;
}

void Ball::draw(QPainter *painter){
    painter->setBrush(circleBrush);
    painter->setPen(circlePen);
    //std::cout<<x<<" "<<y<<std::endl;
    painter->drawEllipse(QRectF(x,y,width,height));
}

double Ball::getX(){
    return x;
}

double Ball::getY(){
    return y;
}

void Ball::setX(double newx){
    x = newx;
}

void Ball::setY(double newy){
    y = newy;
}


double Ball::getRadius(){
    return radius;
}

Vector* Ball::getVector(){
 return vector;
}

void Ball::setVector(Vector *v){
    delete(vector);
    vector = v;
}


double Ball::getMass(){
    return mass;
}

double Ball::getCenterX(){
    return x+radius;
}

double Ball::getCenterY(){
    return y+radius;
}

double Ball::distance(Ball *b){
    return sqrt(((this->getCenterX()-b->getCenterX())*(this->getCenterX()-b->getCenterX()))+
                ((this->getCenterY()-b->getCenterY())*(this->getCenterY()-b->getCenterY())));
}

double Ball::getXDistance(Ball *b){
    return (this->getCenterX()-b->getCenterX());
}

double Ball::getYDistance(Ball *b){
    return (this->getCenterY()-b->getCenterY());
}
bool Ball::ballCollision(Ball *b){
    if(b == this) return false;
    return (radius+b->getRadius())> this->distance(b);
}

void Ball::resolveCollision(Ball *b){
   Vector delta(this->getXDistance(b),this->getYDistance(b));
   double d = delta.getMagnitude();
   double factor =(radius+b->getRadius()-d)/d;
   Vector mtd(delta.getX()*factor,delta.getY()*factor);
   /*Relative mass*/
   double im1 = 1.0/mass;
   double im2 = 1.0/b->getMass();

   /*Minimum movement*/
   this->setX(this->getX()+mtd.getX()*(im1/(im1+im2)));
   this->setY(this->getY()+mtd.getY()*(im1/(im1+im2)));
   b->setX(b->getX()-mtd.getX()*(im2/(im1+im2)));
   b->setY(b->getY()-mtd.getY()*(im2/(im1+im2)));

   Vector *n = delta.getUnitVector();
   Vector v(vector->getX()-b->getVector()->getX(),vector->getY()-b->getVector()->getY());
   double vn = v.dotProduct(n);
   if (vn >= 0) return;
   double restituion = 0.8;
   double i = (-(1.0*restituion)*vn)/(im1+im2);
   Vector impulse(n->getX()*i,n->getY()*i);
   vector->addToVector(impulse.getX()*im1,impulse.getY()*im1);
   b->getVector()->addToVector(-impulse.getX()*im2,-impulse.getY()*im2);
}

void Ball::gravity(Ball *b){
    double dist = this->distance(b);
    if(dist>MAXGRAV) return;
    Vector v(this->getXDistance(b),this->getYDistance(b));
    Vector *init = v.getUnitVector();
    vector->addToVector(-init->getX()/dist*b->getMass()*GRAV,-init->getY()/dist*b->getMass()*GRAV);
    b->getVector()->addToVector(init->getX()/dist*mass*GRAV,init->getY()/dist*mass*GRAV);

}

/*void Ball::getBounds(struct Bounds* b){
    b->topx=x;
    b->topy=y;
    b->botx=x+radius;
    b->boty=y+radius;
}*/
