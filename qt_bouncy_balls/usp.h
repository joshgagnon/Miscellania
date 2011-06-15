#ifndef USP_H
#define USP_H

#include <vector>
#include <ball.h>

#define USP_COLS (100)
#define USP_ROWS (60)

class USP
{
private:
    std::vector<Ball*> usp[USP_COLS][USP_ROWS];
    int nextX,nextY;
public:
    USP();
    void updateUSP(Ball*[],int,double,double,double,double);
    std::vector<Ball*> getObjects(int,int);
    std::vector<Ball*>* getNext();

};

#endif // USP_H
