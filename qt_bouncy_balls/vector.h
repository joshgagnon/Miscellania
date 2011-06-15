#ifndef VECTOR_H
#define VECTOR_H
#include <cmath>
class Vector
{
private:
    double x,y,mag;
    Vector *normal,*unit;

public:
    Vector();
    Vector(double, double);
    Vector(Vector*);
    Vector(Vector*,double);
    ~Vector();
    double getX();
    double getY();
    Vector* setX(double);
    Vector* setY(double);
    Vector* addToVector(double,double);    //mutates
    double getMagnitude();
    Vector* setMagnitude(double);
    Vector* getNormal();
    Vector* getUnitVector();
    Vector* crossProduct(Vector*);  //new
    double dotProduct(Vector*);
    Vector* scalarMultiply(double); //mutates

};

#endif // VECTOR_H
