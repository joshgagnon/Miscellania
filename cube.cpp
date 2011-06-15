
/* Cube puzzle solver.  Uses pretty much the method as SOMA (including copypasta of their rotation matrix */
/* Generates all valid permutations of a piece, and turns each into a bit mask.  Then it is just a matter */
/* of finding a set of permutations (one from each shape) that doesn't self-interect (bit-wise).          */

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#define SIZE 3

using namespace std;
typedef long long mask_t;
// All 24 right angle rotations
int m[24][3][3] = { { {1,0,0},{0,1,0},{0,0,1}   },  
		    { {0,0,1},{0,1,0},{-1,0,0}  },
		    { {0,0,1},{0,-1,0},{1,0,0}  },
		    { {0,0,1},{1,0,0},{0,1,0}   },
		    { {0,0,1},{-1,0,0},{0,-1,0} },
		    { {0,0,-1},{0,1,0},{1,0,0}  },
		    { {0,0,-1},{0,-1,0},{-1,0,0}},
		    { {0,0,-1},{1,0,0},{0,-1,0} },
		    { {0,0,-1},{-1,0,0},{0,1,0} },
		    { {0,1,0},{0,0,1},{1,0,0}   },
		    { {0,1,0},{0,0,-1},{-1,0,0} },
		    { {0,1,0},{1,0,0},{0,0,-1}  },
		    { {0,1,0},{-1,0,0},{0,0,1}  },
		    { {0,-1,0},{0,0,1},{-1,0,0} },
		    { {0,-1,0},{0,0,-1},{1,0,0} },
		    { {0,-1,0},{1,0,0},{0,0,1}  },
		    { {0,-1,0},{-1,0,0},{0,0,-1}},
		    { {1,0,0},{0,0,1},{0,-1,0}  },
		    { {1,0,0},{0,0,-1},{0,1,0}  },
		    { {1,0,0},{0,-1,0},{0,0,-1} },
		    { {-1,0,0},{0,0,1},{0,1,0}  },
		    { {-1,0,0},{0,0,-1},{0,-1,0}},
		    { {-1,0,0},{0,1,0},{0,0,-1} },
		    { {-1,0,0},{0,-1,0},{0,0,1} }, };

class Shape{
public:
	Shape(vector< vector<int> > coords, int index) :  label(index) {
		for(int r=0;r<24;r++) // For every rotation
			for(int x=0;x<SIZE;x++) // For every x translation
				for(int y=0;y<SIZE;y++) // Ditto y 
					for(int z=0;z<SIZE;z++){ // Ditto z
						mask_t position = 0; // Mask to translate to
						bool valid = true;
						for(int i=0;i<coords.size();i++){ // for every coord
							int tmp[3] = {0,0,0}; 	  // new coord						
							for(int j=0;j<3;j++)      // do rotation
								for(int k=0;k<3;k++)
									tmp[j] += coords[i][k] * m[r][j][k];

							tmp[0] += x; tmp[1] += y; tmp[2] += z; // do translation 
							if(tmp[0] < 0 || tmp[0] >= SIZE || tmp[1] < 0 || tmp[1] >= SIZE 
							   || tmp[2] < 0 || tmp[2] >= SIZE ) valid = false;
							else position |= 1<<(tmp[0]*SIZE*SIZE+tmp[1]*SIZE+tmp[2]);				
						}
						if(valid) positions.insert(position);	
					}
		reset();		
	}

	void reset(){	iter = positions.begin(); end = positions.end();} 
	int label;
	set<mask_t> positions; // this shape in all its valid positions
	set<mask_t>::iterator iter, end;
};

bool compare(Shape *a, Shape *b){ return a->positions.size()<b->positions.size(); }

int main(int argc, char **argv){
	int i=0, size, read_buf;
	vector<Shape*> shapes;
	vector< vector <int> > shape_buf;
	vector<int> coord_buf;
	mask_t result=0, goal = ( 1 << (SIZE*SIZE*SIZE) ) -1;

	while(cin >> size){	// read in
		shape_buf.clear();
		for(int k=0;k<size;k++){
			for(int j=0;j<3 && cin >> read_buf;j++) coord_buf.push_back(read_buf);
			shape_buf.push_back(coord_buf);
			coord_buf.clear();
		}			
		shapes.push_back(new Shape(shape_buf,shapes.size()));
	}

	sort(shapes.begin(), shapes.end(), compare);
		  	 		
	while(i<shapes.size()){ //depth first loop
		if(i<shapes.size() && !(*shapes[i]->iter & result))
			result |= *shapes[i++]->iter;
		else{	      
			while(++(shapes[i]->iter) == shapes[i]->end && i>0){				
				shapes[i--]->reset(); //exhausted this shape
				result &= ~*shapes[i]->iter; //remove shape from result
			}
			if(i==0) break; // exhausted all shapes, no solution
		}
	}

	if(result == goal){  //print out puzzle
		cout<<"Completed puzzle"<<endl;
		for(int j=0;j<SIZE*SIZE*SIZE;j++){  
			int k = 1 << j;
			for(int s=0; s<shapes.size(); s++){
				if(*shapes[s]->iter & k) cout<<shapes[s]->label;
			}
			if (j%SIZE == SIZE-1) cout<<" ";
			if (j%(SIZE*SIZE) == SIZE*SIZE-1) cout<<endl;
		}
	}
	else if(i==shapes.size()) cout<<"Fitted all pieces, still room left"<<endl;
	else cout<<"Couldn't fit pieces"<<endl;
}
