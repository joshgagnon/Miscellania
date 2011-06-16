#include <QtGui>
#include "game.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <vector>
#define NUMBALLS 300

Game::Game()
{
  width=1000;
  height=600;
  std::cout<<"Initializing"<<std::endl;
  //*ball = (Ball*)malloc(100*sizeof *ball);
  for(int i =0;i<NUMBALLS;i++){
    balls[i] = new Ball();
  }


  background = QBrush(QColor(64, 32, 64));

  textPen = QPen(Qt::white);
  textFont.setPixelSize(25);
  std::cout<<"Starting"<<std::endl;
  usp = new USP();
}

void Game::paint(QPainter *painter, QPaintEvent *event, int elapsed)
{
  painter->fillRect(event->rect(), background);
  painter->save();
  double minx=std::numeric_limits<double>::max();
  double maxx=std::numeric_limits<double>::min();
  double miny=minx,maxy=maxx;
  double centroidx=0,centroidy=0;
    
  for(int i=0;i<NUMBALLS;i++){
    if(minx>balls[i]->getCenterX()) minx=balls[i]->getCenterX();
    if(maxx<balls[i]->getCenterX()) maxx=balls[i]->getCenterX();
    if(miny>balls[i]->getCenterY()) miny=balls[i]->getCenterY();
    if(maxy<balls[i]->getCenterY()) maxy=balls[i]->getCenterY();
    centroidx+=balls[i]->getCenterX();
    centroidy+=balls[i]->getCenterY();
  }
  centroidx/=NUMBALLS;
  centroidy/=NUMBALLS;
 
  usp->updateUSP(balls,NUMBALLS,minx,miny,maxx,maxy);
  std::vector<Ball*>* v;
  while((v=usp->getNext())!=NULL){
    for(int i=0;i<v->size();i++){
      for(int j=i+1;j<v->size();j++){
	if(v->at(i)->ballCollision(v->at(j))){
	  v->at(i)->resolveCollision(v->at(j));
	}
      }
    }
  }

  for(int i =0;i<NUMBALLS;i++){
    balls[i]->move(elapsed);
    balls[i]->draw(painter);
  }
  painter->restore();
}

