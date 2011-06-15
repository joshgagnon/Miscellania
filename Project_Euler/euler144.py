from Tkinter import *
import math

def mapx(x):
	return x*12.5 +250

def mapy(y):
	return -y*12.5 +250
	
def crossprod(v1,v2):
	return Vector(v1.dx+v2.dx,v1.dy+v2.dy)

def dotprod(v1,v2):
	return v1.dx*v2.dx + v1.dy * v2.dy

def magnitude(v):
	return math.sqrt(v.dx**2+v.dy**2)

def vectorMultiply(v,s):
	return Vector(v.dx*s,v.dy*s)

	#double a = 4*ray.dx*ray.dx +ray.dy*ray.dy;
	#double b = 2*4*ray.x*ray.dx + 2*ray.y*ray.dy;
	#double c = -100 + (4*ray.x*ray.x) + ray.y*ray.y;

	#double t1 = (-b - sqrt(b*b - (4 * a * c)))/(2*a);
	#double t2 = (-b + sqrt(b*b - (4 * a * c)))/(2*a);
	#double t = (t1>t2)?t1:t2;

	#double x1 =  ray.dx * t + ray.x;
	#double y1 = ray.dy * t + ray.y;
	#double tangent = -4 * x1 / y1;



class Vector:
	def __init__(self,dx,dy):
		self.dx=dx
		self.dy=dy
		
	def getUnit(self):
		self.unit=Vector(dx=self.dx/(abs(self.dx)+abs(self.dy)),
                             dy=self.dy/(abs(self.dx)+abs(self.dy)))           
		return self.unit

	def getNormal(self):
		return Vector(dx=self.dy,dy=-self.dx)

class Ray:
	def __init__(self,x,y,v):
		self.x=x
		self.y=y
		self.v=v

 	def draw(self,canvas,color):
 		canvas.create_line(mapx(self.x),mapy(self.y), mapx( self.x+self.v.dx),mapy(self.y+ self.v.dy),fill=color)
 		#print mapx(self.x),mapy(self.y), mapx(self.v.dx), mapy(self.v.dy)
 
 	def intersect(self,canvas):
 		a = 4*self.v.dx**2 + self.v.dy**2
 		b = 2*4*self.x*self.v.dx + 2*self.y*self.v.dy;
 		
 		c = -100 + (4*self.x**2) + self.y**2;
 		t1 = (-b -math.sqrt(b*b - (4 * a * c)))/(2*a)
 		t2 = (-b +math.sqrt(b*b - (4 * a * c)))/(2*a)
 		t = t1
 		if t2 > t1:
 			t = t2

 		x1 = self.v.dx*t + self.x
 		y1 = self.v.dy*t + self.y
 		print x1
 		if -0.01 <= x1 and 0.01 >= x1 and y1 > 0:
 			exit()
 		tangent = Vector(1,-4 * x1 / y1)
 		normal = tangent.getNormal()
 		Ray(x1,y1,normal.getUnit()).draw(canvas,"blue");
 		Ray(x1,y1,tangent.getUnit()).draw(canvas,"green");
 		
		t = dotprod(tangent,self.v)
		n = dotprod(tangent.getNormal(),self.v)
		vt = vectorMultiply(tangent,t)
		vn = vectorMultiply(tangent.getNormal(),-n)
		er= Vector(dotprod(Vector(1,0),vn)+dotprod(Vector(1,0),vt),
				(dotprod(Vector(0,1),vn)+dotprod(Vector(0,1),vt)))
		self.v.dx = x1 - self.x
		self.v.dy = y1 - self.y 
		self.draw(canvas,"red");
		self.v = er;
		self.x = x1;
		self.y = y1;
		print self.x,self.y,self.v.dy,self.v.dy
		#if(self.v.dy>10e100):
			#self.v.dy=10e100;
		self.v = self.v.getUnit()
		
	
 		
 		
class Go:
	def __init__(self):
		self.tick=1000/60
		self.width=500
		self.height=500
		self.center=250;
		self.root=Tk()        
		self.canvas = Canvas(self.root, width=self.width, height=self.height)
		self.canvas.grid(row=0,column=0)
		self.root.after(0,self.loop);
		self.canvas.create_oval(mapx(-5),mapy(-10),mapx(5),mapy(10))
		self.canvas.create_line(mapx(20),mapy(0),mapx(-20),mapy(0))
		self.canvas.create_line(mapx(0),mapy(20),mapx(0),mapy(-20))
		self.ray = Ray(0,10.1,Vector(1.4,-9.6-10.1))
		self.count=0;
		self.root.mainloop()
		
        

	def loop(self):

		
		self.count+=1;
		print self.count
		self.ray.intersect(self.canvas)
		self.root.after(100,self.loop)
if __name__ == "__main__":
    Go()
