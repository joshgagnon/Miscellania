from Tkinter import *
import math
import random

HEX='0123456789ABCDEF'
GRAV=6.67e-11

def crossprod(v1,v2):
    return Vector(dx=v1.dx+v2.dx,dy=v1.dy+v2.dy)

def dotprod(v1,v2):
    return v1.dx*v2.dx + v1.dy * v2.dy

def magnitude(v):
    return math.sqrt(v.dx**2+v.dy**2)

def vectorMultiply(v,s):
    return Vector(dx=v.dx*s,dy=v.dy*s)


class USP:
    
    def __init__(self, length=100):
        self.length = length
        self.map = {}
        
    def insert(self,key,object):
        if key in self.map:
            if object not in self.map[key]:
                self.map[key].append(object)
        else:
            self.map[key] = []
            self.map[key].append(object)
    
    def erase(self):
        self.map = {}


class Vector:

    def __init__(self,mag=None,dx=0,dy=0):
        if(mag==None):
            self.dx=dx
            self.dy=dy            
        else:
            angle = random.random()*2*math.pi
            self.dx = mag *math.cos(angle);
            self.dy = mag *math.sin(angle);
        self.normal=None
        self.unit=None
    

    def getNormal(self):
        return Vector(dx=self.dy,dy=-self.dx)


    def getUnit(self):
        if abs(self.dx)+abs(self.dy) ==0:
            self.unit=self;
        else:
            self.unit=Vector(dx=self.dx/(abs(self.dx)+abs(self.dy)),
                             dy=self.dy/(abs(self.dx)+abs(self.dy)))           
        return self.unit

    def rotate(self,rot):
        angle = math.atan2(self.dy,self.dx)
        mag = math.sqrt(self.dx**2 + self.dy**2)
        angle+=rot
        self.dx = mag *math.cos(angle);
        self.dy = mag *math.sin(angle);

    def setMag(self,mag):
        angle = math.atan2(self.dy,self.dx)
        self.dx = mag *math.cos(angle);
        self.dy = mag *math.sin(angle);        

    def getDX(self):
        return self.dx

    def getDY(self):
        return self.dy

    def setDX(self,dx):
        self.dx=dx

    def setDY(self,dy):
        self.dy=dy

floor = Vector(dx=1,dy=0)
ceiling = Vector(dx=-1,dy=0)
leftwall = Vector(dx=0,dy=-1)
rightwall = Vector(dx=0,dy=1)
gravity = Vector(dx=0,dy=1)
mouse = Vector()

def setMouse(event):
    mouse.dx = event.x
    mouse.dy = event.y

class Oval:
    def __init__(self,parent,x=0,y=0,size=10):
        self.canvas = parent.canvas
        self.parent=parent
        self.x=x
        self.y=y
        #self.
        self.vector = Vector(mag=10)
        self.prevx=self.x-self.vector.dx
        self.prevy=self.y-self.vector.dy
        self.size = random.random()*parent.maxSize
        self.mass =((size/2)**3)*math.pi*4.0/3
        self.colour = '#'+''.join([random.choice(HEX) for x in range(6)])
        self.ball = self.canvas.create_oval(self.x,self.y,self.x+self.size,
                                            self.y+self.size,
                                            fill=self.colour)    
    def move(self,balls):
  
        self.testStaticCollisions()
        #if(self.x==self.prevx and self.y==self.prevy):
        #    self.vector=Vector(dx=0,dy=0)
        #self.vector=crossprod(self.vector,gravity)
       
        towardsMouse=Vector(dx=mouse.dx-self.x,dy=mouse.dy-self.y)
        if magnitude(towardsMouse)!=0:
               towardsMouse=vectorMultiply(towardsMouse.getUnit(),1/magnitude(towardsMouse)*100)
        self.vector=crossprod(self.vector,towardsMouse)

        self.prevx=self.x
        self.prevy=self.y
        self.x+=self.vector.getDX()
        self.y+=self.vector.getDY()
        self.canvas.coords(self.ball,self.x,self.y,
                           self.x+self.size,self.y+self.size)   

    def getCoordSet(self,size):
        x=size[0]
        y=size[1]
        self.bins = set()
        self.bins.add((int(self.x/self.parent.width*x),
                              int(self.y/self.parent.height*y)))
        self.bins.add((int(self.x/self.parent.width*x),
                            int((self.y+self.size)/self.parent.height*y)))
        self.bins.add((int((self.x+self.size)/self.parent.width*x),
                             int(self.y/self.parent.height*y)))
        self.bins.add((int((self.x+self.size)/self.parent.width*x+self.size),
                            int((self.y+self.size)/self.parent.height*y)))
   

    def testCollision(self,ball):
        if ball == self:
            return False
        distance = math.sqrt((self.x-ball.x)**2 +
                             (self.y-ball.y)**2)
        return distance <= (self.size/2 + ball.size/2)
                 
        
    def ballCollision(self,ball):
        delta = Vector(dx=self.x+(self.size/2) -( ball.x+(ball.size/2)),
                       dy=self.y+(self.size/2)-(ball.y+(ball.size/2)))
        d = magnitude(delta)
        mtd = vectorMultiply(delta,(self.size/2+ball.size/2-d)/d)
        im1 = 1/self.mass
        im2 = 1/ball.mass
        
        self.x+=mtd.dx*(im1/(im1+im2))
        self.y+=mtd.dy*(im1/(im1+im2))
        ball.x-=mtd.dx*(im2/(im1+im2))
        ball.y-=mtd.dy*(im2/(im1+im2))

        n = delta.getUnit()
        v = Vector(dx=self.vector.dx-ball.vector.dx,
                   dy=self.vector.dy-ball.vector.dy)
        vn = dotprod(v,n)
        if vn >= 0:
            return
        restitution = 0.8
        i = (-(1.0*restitution)*vn)/(im1+im2)
        impulse = vectorMultiply(n,i)

        self.vector.dx += impulse.dx*im1
        self.vector.dy += impulse.dy*im1
        ball.vector.dx -= impulse.dx*im2
        ball.vector.dy -= impulse.dy*im2

    def testStaticCollisions(self):
        if self.parent.height-self.size<=self.y:
            self.y=self.parent.height-self.size
            self.bounceBoundary(floor)
        if 0 >= self.y:    
            self.y=0
            self.bounceBoundary(ceiling)
        if self.parent.width-self.size-1<=self.x:
            self.x = self.parent.width-self.size-1
            self.bounceBoundary(rightwall)
        if 0 >= self.x:
            self.x=0
            self.bounceBoundary(leftwall)

    def bounceBoundary(self,boundary):
        t = dotprod(boundary,self.vector)
        n = dotprod(boundary.getNormal(),self.vector)
        vt = vectorMultiply(boundary,t)
        vn = vectorMultiply(boundary.getNormal(),-n)
        self.vector= Vector(dx=(dotprod(Vector(dx=1,dy=0),vn)+
                          dotprod(Vector(dx=1,dy=0),vt))*0.3,
                      dy=(dotprod(Vector(dx=0,dy=1),vn)+
                          dotprod(Vector(dx=0,dy=1),vt))*0.3)
class Game:
    def __init__(self):
        self.tick=1000/60
        self.width=1000
        self.height=500
        self.ballcount=10
        self.maxspeed=5
        self.maxSize=30
        self.root=Tk()
        self.canvas = Canvas(self.root, width=self.width, height=self.height)
        self.canvas.grid(row=0,column=0)
        self.uspSize=(self.width/(self.maxSize+1),self.height/(self.maxSize+1))
        self.usp = USP()
        self.balls = [Oval(self,x=random.random()*(self.width-30)+10,
                           y=random.random()*(self.height-30)+10) 
                      for i in range(self.ballcount)]
        #self.balls[0]=Oval(self,x=250,y=250,angle=0,vel=0,size=50)
        self.canvas.bind("<Motion>",setMouse);          
        self.root.after(0,self.main_loop);
        self.root.mainloop()
        
    def main_loop(self):
        self.updateAll()
        self.root.after(10,self.main_loop);
                          
    def updateAll(self):
        gravity.rotate(2*math.pi/(self.tick*10))
        self.buildUSP()
        for v in self.usp.map.itervalues():
            for i in range(len(v)):
                for j in range(i+1,len(v)):
                    if v[i].testCollision(v[j]):
                        v[i].ballCollision(v[j]);
                                 
            #ball.resolveCollisions(self.usp,self.uspSize)
            
        for i in range(self.ballcount):
            self.balls[i].move(self.balls)
            

    def buildUSP(self):
        self.usp.erase()
        x = self.uspSize[0]
        y = self.uspSize[1]
        for ball in self.balls:
            self.usp.insert((int(ball.x/self.width*x),
                              int(ball.y/self.height*y)),ball)
            self.usp.insert((int(ball.x/self.width*x),
                             int((ball.y+ball.size)/self.height*y)),ball)
            self.usp.insert((int((ball.x+ball.size)/self.width*x),
                             int(ball.y/self.height*y)),ball)
            self.usp.insert((int((ball.x+ball.size)/self.width*x+ball.size),
                             int((ball.y+ball.size)/self.height*y)),ball)


if __name__ == "__main__":
    Game()


