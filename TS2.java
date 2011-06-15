
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.Random;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.LinkedHashSet;
import java.awt.geom.Point2D;
import java.util.Collections;
import java.text.*;
import java.lang.Double;

/*****************************************************************************/


class NodeDisplay extends JPanel {
	private Point2D[] Cities;
	private int [] path;
	public void updateNodes(Point2D[] ps, int[] p){
       Cities = ps; path=p;
	}
   
   /* Draw map of nodes and connections */
	public void paint(Graphics g) {
		if( Cities==null) return;
		g.setColor(Color.BLUE);
		/* Draw the nodes*/
		for(int i=0;i<path.length;i++){
               g.fillOval((int)Cities[i].getX(), (int)Cities[i].getY(), 10,10);
		}
		if( path==null) return;
		g.setColor(Color.BLACK);
		
		for(int i=0;i<path.length-1;i++){
			g.drawLine((int)Cities[path[i]].getX()+5,(int)Cities[path[i]].getY()+5,
				(int)Cities[path[i+1]].getX()+5,(int)Cities[path[i+1]].getY()+5);
		}
		g.drawLine((int)Cities[path[path.length-1]].getX()+5,(int)Cities[path[path.length-1]].getY()+5,
			(int)Cities[path[0]].getX()+5,(int)Cities[path[0]].getY()+5);
	}
}


/*****************************************************************************/


class GraphDisplay extends JPanel {
	private List<Double> bestgraph;
	private List<Double> avegraph;

	public void updateGraph(List<Double>bg, List<Double> ag){
		this.bestgraph=bg;
		this.avegraph=ag;
	}

   /* Draw graph of fitness over generations */
   public void paint(Graphics graph) {
		if(bestgraph == null | avegraph == null || bestgraph.isEmpty() || avegraph.isEmpty())return;
		double max = bestgraph.get(avegraph.size()-1);
      Graphics2D g = (Graphics2D)graph;
		/*Scale*/
		g.setColor(Color.BLACK);
		g.drawLine(60, 10, 60, 410);
		DecimalFormat df = new DecimalFormat("#.#####");

		for(int i=0;i<10;i++){
			g.drawLine(60, 40*i+10, 70, 40*i+10);
			g.drawString(df.format((max*(1.0-0.1*i))), 8, 40*i+15);
			g.drawLine(60, 40*i+30, 65, 40*i+30);
		}
		g.drawString(Integer.toString(0), 42, 415);

		g.drawLine(60, 410, 430, 410);
		g.drawString("Number of generations", 20, 440);
		if(bestgraph == null) return;
	    int length=bestgraph.size()-1;
		
		/*for(int i=0;i<11;i++){
			g.drawLine(30+40*i, 410,30+40*i , 400);
			g.drawString(Integer.toString((int)(avegraph.size()/10.0*i)), 28+40*i, 425);
		}*/
		g.drawLine(60, 410,60 , 400);
		g.drawString(Integer.toString((0)), 58, 425);
		
		g.drawLine(245, 410, 245 , 400);
		g.drawString(Integer.toString(length/2), 243, 425);
		
		g.drawLine(430, 410,430 , 400);
		g.drawString(Integer.toString(length), 428, 425);
		
		/*From the origin*/
	    g.setColor(Color.RED);
		
		for(int i=0;i<length;i++){
			g.drawLine((int)(60+370.0/length*(i)), (int)(410-bestgraph.get(i)/max*400), (int)(60+370.0/length*(i+1)), (int)(410-bestgraph.get(i+1)/max*400));
		}
		g.drawString("Best", 438, (int)(10));
		
		g.setColor(Color.BLUE);
		for(int i=0;i<length;i++){
			g.drawLine((int)(60+370.0/length*(i)), (int)(410-avegraph.get(i)/max*400), (int)(60+370.0/length*(i+1)), (int)(410-avegraph.get(i+1)/max*400));
		}
		g.drawString("Average", 438, (int)(410-avegraph.get(length)/max*400));
   }
}
/*****************************************************************************/


/*****************************************************************************/

class Competitor {

    private int[] path;
	private int[][] adjac;
    private int N;
    private double fitness;
    Random random;
	
    public Competitor(int nodes){
		this.random = new Random();
		this.N = nodes;
        this.path = newPermutation(nodes);		
        this.adjac= createAdjac(this.path);
        this.fitness =-1;
    }

    public Competitor(Competitor p){
		this.random = new Random();	
        N = p.getN();
        path = p.getPath();
		adjac = p.getAdjac();
        fitness = p.getFitness();
    }
    
    public int[] getPath(){
        return path.clone();
    }
    
    public int getPosition(int i){
        return path[i];
    }
    
    public int getN(){
        return N;
    }
    
	public double getFitness(){
		return fitness;
	}
	
    public int[][] getAdjac(){
		return adjac.clone();
	}
	
    private void setPath(int [] p){
        this.path = p.clone();
		this.N=p.length;		
		this.adjac = createAdjac(this.path);
		//System.out.println(adjac.length);
        this.fitness =-1;		
    }
 
    public void setAdjac(int [][] a){	
       this.adjac = a.clone();
        this.fitness =-1;
    }
	
    private void setPosition(int i,int p){
        this.path[i] = p;
		this.adjac = createAdjac(this.path);        
        this.fitness =-1;
    }
    
	public void setFitness(double f){
		this.fitness =f;
	}
	
	public Competitor addNewNode(){
		//System.out.print(path.length +" is now " );
		int[] newpath = new int[this.getN()+1];
		List<Integer> list = new ArrayList<Integer>(newpath.length);
		for(int i=0;i<newpath.length-1;i++) list.add(this.getPosition(i));
		list.add(random.nextInt(newpath.length),newpath.length-1);
		for(int i=0;i<newpath.length;i++) newpath[i]=list.get(i);
		Competitor c = new Competitor(newpath.length);
		c.setPath(newpath);
		return c;
	}
	
	
	private int[] newPermutation(int size){
		List<Integer> pool = new ArrayList<Integer>(size);
		for(int i = 0;i<size;i++) pool.add(i);
		Collections.shuffle(pool, random);
		int[] poolarray = new int[size];
		for(int i = 0;i<size;i++) poolarray[i]=pool.get(i);		
		return poolarray;
	}
	
	private int[][] createAdjac(int [] seq){
        int[][] adjac = new int[seq.length][2];
        for(int i=0;i<seq.length;i++){    
            int j=i-1,k=i+1; 
            if(j<0) j = seq.length-1;
            if(k>=seq.length) k=0;            
            adjac[seq[i]][0]=seq[j];
            adjac[seq[i]][1]=seq[k];        
        }
        return adjac;	
	}
	
    public void printPath(){
        for(int i = 0; i < N; i++){    
            System.out.print(path[i]+ " ");
        }
        System.out.println();
    }
    
    public void swap(int a, int b){
        int x = getPosition(a);
        setPosition(a,getPosition(b));
        setPosition(b,x);
    }
    
    public double getFitness(Point2D [] Cities){
        if (getFitness() == -1){
            double D = 0;
			int size = this.getN();
            for(int i = 0; i < size-1; i++) D += getDistance(Cities[path[i]],Cities[path[i+1]]);
            D += getDistance(Cities[path[size-1]],Cities[path[0]]);
			D=1/(D/(size*size));
			this.setFitness(D);
        }
        return this.getFitness();
    }
    
    private double getDistance(Point2D a, Point2D b){
        return Math.pow(Math.pow(a.getX()-b.getX(),2) + Math.pow(a.getY()-b.getY(),2),0.5);
    }
    
	
	public Competitor transpose(double transposeRate, double pointRate){
        if (random.nextDouble() > transposeRate) return this;
		int[] newpath = this.getPath();
		List<Integer> newpathlist = new ArrayList<Integer>(newpath.length);
		for(int i=0;i<newpath.length;i++) newpathlist.add(newpath[i]);
		int from = random.nextInt(newpath.length-2)+1;
		int size = random.nextInt(newpath.length-from);
		List<Integer> line = new ArrayList<Integer>(size);
		for(int i=0;i<size;i++) line.add(newpathlist.remove(from)); 
		int to = from;
		while(to == from) to = random.nextInt(newpathlist.size());
		if(random.nextInt(2)==0)Collections.reverse(line);
		newpathlist.addAll(to,line);
		for(int i=0;i<newpath.length;i++) newpath[i]=newpathlist.get(i);
		for(int i=0;i<newpath.length;i++){
			if(random.nextDouble() < pointRate){
			 int dest = random.nextInt(newpath.length);
			 int temp = newpath[i];
			 newpath[i] = newpath[dest];
			 newpath[dest] = temp;
			}
		
		}
		this.setPath(newpath);
		return this;
	}
	

	private  int[][] combineAdjac(int[][] a, int[][] b){
		int[][] comb = new int[a.length][];
		int len; int[] temp = new int[4];
		//System.out.println(a.length + " " + b.length);
		for(int i=0;i<a.length;i++){
			List<Integer> line = new ArrayList<Integer>(2);	
			line.add(a[i][0]);	line.add(a[i][1]); 
			//System.out.println(a.length[i] + " " + b.length[i]);
			if(!line.contains(b[i][0])) line.add(b[i][0]); 
			if(!line.contains(b[i][1])) line.add(b[i][1]);
			comb[i]= new int[line.size()+1];
			comb[i][0]= line.size();
			Collections.shuffle(line,random);
			//System.out.println(line.toString());			
			for(int j=0;j<line.size();j++) comb[i][j+1]=line.get(j);
		}
		
		
		return comb;
	}

	private int removeFromNeighbours(int[][] combAdjac,int node){
		int shortest=5;
		combAdjac[node][0]=0;
		for(int i=0;i<combAdjac.length;i++){
			for(int j=1;j<combAdjac[i][0]+1;j++){ //cheick
				if(combAdjac[i][j]==node){
					combAdjac[i][0]--;
					if(shortest>combAdjac[i][0]) {
						shortest=combAdjac[i][0];
					}
					for(int k=j;k<combAdjac[i][0]+1;k++) combAdjac[i][k]=combAdjac[i][k+1];
					break;
				}
			}
		}	
		if(shortest!=0 && shortest !=5){
			int next = random.nextInt(combAdjac.length);
			while(combAdjac[next][0]!=shortest) next = ++next % combAdjac.length;
			return next;
		}
		//int next = random.nextInt(combAdjac.length);
		//while(combAdjac[next][0]==0) next = ++next % combAdjac.length;
		return -1;
	}
	
    public Competitor edgeRecombination(Competitor parent){
        int[] child = new int[this.getN()];
        int i=0;
        int node = random.nextInt(2) == 1 ? this.getPosition(0) : parent.getPosition(0); 
		int[][] combAdjac = combineAdjac(this.getAdjac(),parent.getAdjac());
        while(i<this.getN()-2){		
			
			child[i]=node;
            node= removeFromNeighbours(combAdjac,node);
			if(node==-1) node = getUniqueNode(child,i+1,this.getN());
            i++;
        }
		addUniqueNode(child,i++,this.getN());
		addUniqueNode(child,i,this.getN());
        Competitor offspring = new Competitor(N);
        offspring.setPath(child);
        return offspring;
    }
	
	private int getUniqueNode(int [] seq, int size,int pool){
		boolean[] list = new boolean[pool];
		int offset = random.nextInt(5393);
		for(int i=0;i<pool;i++) list[i]=true;
		for(int i=0;i<size;i++) list[seq[i]]=false;
		for(int i=0;i<pool;i++) if(list[(i+offset)%pool]==true) return (i+offset)%pool;
		return -1;
		}
	
	private void addUniqueNode(int [] seq, int pos, int size){
		boolean[] list = new boolean[size];
		int offset = random.nextInt(5393);
		for(int i=0;i<size;i++) list[i]=true;
		for(int i=0;i<pos;i++) list[seq[i]]=false;
		for(int i=0;i<size;i++) if(list[(i+offset)%size]==true) seq[pos]=(i+offset)%size;
	}

}

/*****************************************************************************/
public class TS2 implements WindowListener {
   private JFrame window1, window2;
   private GraphDisplay gd = new GraphDisplay();
   private NodeDisplay nd = new NodeDisplay();
   private int openWindows;
   
   private int N = 50;
   private int Width = 500;
   private int Height = 500;
   private int Population = 500;
   private int citycount = 50;
   private double transposeRate = 0.9;
   private double pointRate = 0.1;   
   private Competitor[] competitors;
   private int maxGen = 100000;
   private List<Double> bestgraph;
   private List<Double> avegraph;
   private Point2D[] Cities;
   public Random random;
   
   public static void main(String [] args) {
      TS2 s = new TS2(args[0]);
   }
    public int geneticAction(){    
        double topFitness = -1, sumFitness = 0, aveFitness, sumAboveAverage=0,alltimebest=0;
		Competitor alltimebestCompetitor = new Competitor(N);
        int topIndex=0,x=0, countAboveAve;
        competitors = new Competitor[Population]; 
		bestgraph = new ArrayList<Double>(1);
		avegraph = new ArrayList<Double>(1);
		Random rand = new Random();
		Cities = new Point2D[N];

		boolean stable = false;
		int turnsStable=0;

		for(int i=0;i<N;i++){
			Cities[i] = new Point(random.nextInt((Width-100)/10)*10+25,random.nextInt((Height-100)/10)*10+25);		
		}
		int count=0;
		/*for(int i=0;i<10;i++){	
			for(int j=0;j<10;j++){
				Cities[count++] = new Point(50*i,50*j);
			
			}
		}*/
		
        for(int j=0;j<Population;j++) {
            competitors[j] = new Competitor(citycount);
        }
        while(x < maxGen){
        

            
            sumFitness=0;
            topFitness=-1;

			
			/*if((x+1)%100==0 && citycount<N){
				for(int i=0;i<Population;i++) {  
					 competitors[i]  = competitors[i].addNewNode();
				}			
				citycount++;
				alltimebest=0;
			}*/

			
			//alltimebest=0;
			

            for(int i=0;i<Population;i++) {           
                sumFitness += competitors[i].getFitness(Cities);
                if(topFitness < competitors[i].getFitness(Cities)){
                    topFitness = competitors[i].getFitness(Cities);
                    topIndex = i;
					//alltimebestCompetitor = new Competitor(competitors[i]);
					if(alltimebest<competitors[i].getFitness(Cities)){
					alltimebest = competitors[i].getFitness(Cities);
					alltimebestCompetitor = new Competitor(competitors[i]);
					}
                }
            }
            aveFitness = sumFitness/Population;   
			bestgraph.add(topFitness);
			avegraph.add(aveFitness);
			
            /*if(bestgraph.size() > 1000){
            	if(new Double(bestgraph.get(bestgraph.size()-1)).compareTo(new Double(bestgraph.get(bestgraph.size()-2)))==0){
            		turnsStable++;
            		if(turnsStable>500){
            			System.out.println("Mixing things up!");
            			stable=true;
            		}
            	}
            	else{
            		turnsStable=0;
            	}
            }*/
            
			if(!stable){                     
            Competitor[] theNextGeneration = new Competitor[Population];     
            sumAboveAverage=0; countAboveAve=0;
            
           for(int i=0;i<Population;i++) { 
                if(competitors[i].getFitness()>= topFitness*(0.8)){
                    sumAboveAverage+=competitors[i].getFitness(Cities)*competitors[i].getFitness(Cities);
                    countAboveAve=0;
                }
            }       
            /* Reproductive success is proportional to relative fitness */
            /* This is so we can having crossing over later */
            /* Lets take only those greater than the average */
            int k = 0;
            for(int i=0;i<Population;i++) { 
                if(competitors[i].getFitness()>=topFitness*(0.8)){
                    for(int j=0;j<(int)(competitors[i].getFitness(Cities)*competitors[i].getFitness(Cities)/sumAboveAverage*Population )
                        && k< Population;j++){
                        theNextGeneration[k++] = new Competitor(competitors[i]).transpose(transposeRate,pointRate);
                    }
                }
            }
            int m =k;
            while(k<Population) theNextGeneration[k++] = theNextGeneration[rand.nextInt(m)].edgeRecombination(theNextGeneration[rand.nextInt(m)]);
            theNextGeneration[0] = alltimebestCompetitor;
            competitors = theNextGeneration;
            }
            
            else{
            	turnsStable=0;
            	stable=false;
            	competitors[0] = alltimebestCompetitor;            	
               	for(int i=1;i<Population;i++) { 
               		competitors[i] = competitors[i].edgeRecombination(new Competitor(citycount));
               	
               	}
            
            }
            
            

            
            x++;			
			nd.updateNodes(Cities,alltimebestCompetitor.getPath()); 
			window1.repaint(); 
			
			gd.updateGraph(bestgraph,avegraph);
			window2.repaint();
			
			
			
			//OLD STRATS
			//while(k<Population) theNextGeneration[k++] = random.nextInt(2) == 1? new Competitor(alltimebestCompetitor).transpose(mutationRate) : new Competitor(citycount);
			//while(k<Population) theNextGeneration[k++] = new Competitor(citycount);
			/*for(int i=0;i<Population/4;i++) { 
                theNextGeneration[rand.nextInt(Population)] = theNextGeneration[rand.nextInt(Population)].edgeRecombination(theNextGeneration[rand.nextInt(Population)]);
            }*/
			//while(k<Population){theNextGeneration[k] = theNextGeneration[rand.nextInt(k)].edgeRecombination(theNextGeneration[rand.nextInt(k)]);k++;}
			//for(int i=0;i<Population/100;i++) theNextGeneration[rand.nextInt(Population)] = new Competitor(N);
            //competitors[topIndex].printPath();
            
        }
    System.out.println(x + " generations");
    return topIndex;
    }

   /* Set up windows for displaying nodes and fitness graph */
   public TS2(String size) {
   		citycount = Integer.parseInt(size);
   		N = Integer.parseInt(size);
		random = new Random(N);
		
			
       window1 = new JFrame("Nodes");
      window1.setSize(Width , Height);
      window1.setLocation(0, 0);
      window1.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
      window1.addWindowListener(this);
      window1.getContentPane().add(nd);
      window1.setVisible(true);
	  
      window2 = new JFrame("Fitness Graph");
      window2.setSize(Width , Height);
      window2.setLocation(500, 0);
      window2.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
      window2.addWindowListener(this);
      window2.getContentPane().add(gd);
      window2.setVisible(true);
	  openWindows = 2;
	  geneticAction();
      
		

   }


   
   public void windowClosed(WindowEvent e) {
      /* Closes the program if both windows are closed */
      openWindows--;
      if (openWindows == 1) System.exit(0);
      if (openWindows == 0) System.exit(0);
   }


   /* Included for completeness, currently don't do anything */
   public void windowOpened(WindowEvent e) {
   }

   public void windowClosing(WindowEvent e) {
   }

   public void windowActivated(WindowEvent e) {
   }

   public void windowDeactivated(WindowEvent e) {
   }

   public void windowIconified(WindowEvent e) {
   }

   public void windowDeiconified(WindowEvent e) {
   }
   /**********************************************************/

}
