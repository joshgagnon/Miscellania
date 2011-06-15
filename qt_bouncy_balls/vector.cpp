#include "vector.h"
#include "stdlib.h"

Vector::Vector(){
    x=1;
    y=1;
    mag=-1;
    normal=unit=NULL;
}

Vector::Vector(double newx, double newy){
    x=newx;
    y=newy;
    mag=-1;
    normal=unit=NULL;
}

Vector::Vector(Vector *v){
    x=v->getX();
    y=v->getY();
    mag=-1;
    normal=unit=NULL;
}

Vector::Vector(Vector *v, double d){
    x=v->getX()*d;
    y=v->getY()*d;
    mag=-1;
    normal=unit=NULL;
}

Vector::~Vector(){
 if(unit!=NULL) delete(unit);
 if(normal!=NULL) delete(normal);
}

double Vector::getX(){
    return x;
}

double Vector::getY(){
    return y;
}

Vector* Vector::setX(double newx){
    x = newx;
    mag = -1;
 if(unit!=NULL) delete(unit);
 if(normal!=NULL) delete(normal);
    return this;
}

Vector* Vector::setY(double newy){
    y = newy;
    mag = -1;
     if(unit!=NULL) delete(unit);
 if(normal!=NULL) delete(normal);
    return this;
}

Vector* Vector::addToVector(double a, double b){
    x+=a;
    y+=b;
   mag = -1;
    if(unit!=NULL) delete(unit);
 if(normal!=NULL) delete(normal);
 return this;

}

double Vector::getMagnitude(){
    if(mag!=-1) return mag;
    mag = sqrt(x*x+y*y);
    return mag;
}

Vector* Vector::scalarMultiply(double d){
    x=x*d;
    y=y*d;
    mag=-1;
    if(normal!=NULL) delete(normal);
    if(unit!=NULL) delete(unit);
    return this;
}

Vector* Vector::getUnitVector(){
    if(unit != NULL) return unit;
    double sum = abs(x) + abs(y);
    if(sum==0) unit = new Vector(this);
    else unit = new Vector(x/sum,y/sum);
    return unit;
}

Vector * Vector::getNormal(){
    if(normal != NULL) return normal;
    normal = new Vector(y,-x);
    return normal;
}

double Vector:: dotProduct(Vector *v){
    return x*v->getX() + y*v->getY();
}
