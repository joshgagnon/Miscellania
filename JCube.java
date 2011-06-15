import java.util.*;

class ShapeCompare implements Comparator<Shape>{
	public int compare(Shape a, Shape b){
		int alength=  a.positions.length;
		int blength = b.positions.length;
		if(alength>blength) return 1;
		if(blength>alength) return -1;
		return 0;
	}
}

class Shape{
	public int m[][][] = { { {1,0,0},{0,1,0},{0,0,1}   },  
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

	public	Shape(ArrayList<ArrayList<Integer>> coords, int name, int SIZE){
		TreeSet<Long> positions_set = new TreeSet<Long>();
		label = name;
		for(int r=0;r<24;r++) // For every rotation
			for(int x=0;x<SIZE;x++) // For every x translation
				for(int y=0;y<SIZE;y++) // Ditto y 
					for(int z=0;z<SIZE;z++){ // Ditto z
						long  position = 0; // Mask to translate to
						boolean valid = true;
						for(int i=0;i<coords.size();i++){ // for every coord
							int tmp[] = {0,0,0}; 	  // new coord						
							for(int j=0;j<3;j++)      // do rotation
								for(int k=0;k<3;k++){
								
									tmp[j] += coords.get(i).get(k) * m[r][j][k];
								}
							tmp[0] += x; tmp[1] += y; tmp[2] += z; // do translation 
							if(tmp[0] < 0 || tmp[0] >= SIZE || tmp[1] < 0 || tmp[1] >= SIZE 
							   || tmp[2] < 0 || tmp[2] >= SIZE ) valid = false;
							else position |= 1<<(tmp[0]*SIZE*SIZE+tmp[1]*SIZE+tmp[2]);				
						}
						if(valid) positions_set.add(new Long(position));	
					}
		reset();
		positions = positions_set.toArray(new Long[0]);
	}
	public void reset(){counter=0;}
	public Long get(){return positions[counter];}
	public int  incr(){return ++counter;}
	public Long positions[];
	public int counter;
	public int label;
	       
}

public class JCube{

	public static final int SIZE = 3;

	public static void main(String [] args){
		int i=0, size;
		ArrayList<Shape> shapes = new ArrayList<Shape>();;
		
		long result=0, goal = ( 1 << (SIZE*SIZE*SIZE) ) -1;
		Scanner scanner = new Scanner(System.in);

	       	while(scanner.hasNextInt()){
			size = scanner.nextInt();
			ArrayList<ArrayList<Integer>> shape_buf = new ArrayList<ArrayList<Integer>>();	
			for(int k=0;k<size;k++){
				shape_buf.add(new ArrayList<Integer>());
				for(int j=0;j<3; j++)
					shape_buf.get(k).add(scanner.nextInt());	
			}
			shapes.add(new Shape(shape_buf,shapes.size(),SIZE));
		}
		

		Collections.sort(shapes, new ShapeCompare());
	
		 	 		
		while(i<shapes.size()){ //depth first loop
			if(i<shapes.size() && 0 == (shapes.get(i).get() & result))
				result |= shapes.get(i++).get();
			else{	      
				while(shapes.get(i).incr() >= shapes.get(i).positions.length && i>0){
					shapes.get(i--).reset(); //exhausted this shape
					result &= ~shapes.get(i).get(); //remove shape from result
				}
				if(i==0) break; // exhausted all shapes, no solution
	
			}
		}
			
		if(result == goal){  //print out puzzle
			System.out.println("Completed puzzle");
			for(int j=0;j<SIZE*SIZE*SIZE;j++){  
				int k = 1 << j;
				for(int s=0; s<shapes.size(); s++){
					if((shapes.get(s).get() & k)!=0) System.out.print(shapes.get(s).label);
				}
				if (j%SIZE == SIZE-1) System.out.print(" ");
				if (j%(SIZE*SIZE) == SIZE*SIZE-1) System.out.println();
			}
		}
		else if(i==shapes.size()) System.out.println("Fitted all pieces, still room left");
		else System.out.println("Couldn't fit pieces");
	}

}