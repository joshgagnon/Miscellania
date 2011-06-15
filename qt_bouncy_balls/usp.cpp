#include "usp.h"
#define USP_COLS (100)
#define USP_ROWS (60)

USP::USP(){

}

#include <iostream>
void USP::updateUSP(Ball* balls[],int len, double minx, double miny, double maxx, double maxy){
    double wd = (maxx-minx)/USP_COLS;
    double hi = (maxy-miny)/USP_ROWS;

    for(int i=0;i<USP_COLS;i++){
        for(int j=0;j<USP_ROWS;j++){
            usp[i][j].clear();
        }
    }
    //struct Bounds* bounds = (struct Bounds*)malloc(sizeof *bounds);
    double x,y,radius;
    for(int i=0;i<len;i++){
        //balls[i]->getBounds(bounds);
        x = balls[i]->getCenterX();
        y = balls[i]->getCenterY();
        radius = balls[i]->getRadius();
        //for(int n=(int)((x-minx-0.0001)/wd)
        usp[(int)((x-minx-0.0001)/wd)][(int)((y-miny-0.0001)/hi)].push_back(balls[i]);
    }
    //free(bounds);
    nextX=0;
    nextY=0;

}

std::vector<Ball*>* USP::getNext(){
    if(nextY>=USP_ROWS){
        nextY=0;
        nextX++;
    }
    while(nextX<USP_COLS){
        if(usp[nextX][nextY].size()>1){
            return &usp[nextX][nextY++];
        }
        nextY++;
        if(nextY>=USP_ROWS){
        nextY=0;
        nextX++;
        }
    }
    return (std::vector<Ball*>*)NULL;
}
