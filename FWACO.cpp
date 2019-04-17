#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <stdlib.h>

using namespace std;

// Creates a Graph of Warehouse from Input Parameters.
void CreateGraph(int Nb,int Na,int Ns,int kx,int ky,double **Dist,int **Path,int Nodes)
{
	// b -> Cross Aisle Node Location.
	int b = Ns + 1;
	// L -> Length of a Single Aisle.
	int L = Ns*Nb + Nb + 1;

	double inf = 9999999;

	// Distance Matrix Initialize by Infinity.
	for(int i=0;i<Nodes;i++)
		for(int j=0;j<Nodes;j++)
			Dist[i][j] = inf;
	
	// Predecessors Matrix Initialize by -1.
	for(int i=0;i<Nodes;i++)
		for(int j=0;j<Nodes;j++)
			Path[i][j] = -1;

	// Making Distance from i to i zero,
	for(int i=0;i<Nodes;i++)
		Dist[i][i] = 0;

	// Connecting Adjacent Nodes in each Aisles.
	for(int i=0;i<Nodes;i++)
	{
		if((i+1)%L!=0)
		{
			Dist[i][i+1] = ky;
			Path[i][i+1] = i;
		}
	}

	int i1,j1;
	// Connectng Nodes in Adjacent/Crosss Aisles.
	for(int i=0;i<=Nb;i++)
	{
		for(int j=0;j<Na-1;j++)
		{
			i1 = i*b + j*L;
			j1 = i*b + (j+1)*L;
			Dist[i1][j1] = kx;
			Path[i1][j1] = i1;
		}
	}

	// Making Distance from i to j equals to Distance from j to i.
	for(int i=0;i<Nodes;i++)
	{
		for(int j=0;j<i;j++)
		{
			Dist[i][j] = Dist[j][i];
			if(Path[j][i]!=-1)
				Path[i][j] = i;
		}
	}
}

// Floyd_Warshall Algorithm.
void Floyd_Warshall(double **Dist,int **Path,int Nodes)
{
	double d1,d2;
	for(int k=0;k<Nodes;k++)
	{
		for(int j=0;j<Nodes;j++)
		{
			for(int i=0;i<Nodes;i++)
			{
				d1 = Dist[i][j];
				d2 = Dist[i][k]+Dist[k][j];
				if(d1>d2)
				{
					Dist[i][j] = d2;
					Path[i][j] = Path[k][j];
				}
			}
		}
	}
}

// Return a vector of Nodes that Runs along the Shortst Path from i to j.
vector <int> ReturnPath(int **Path,int i,int j)
{
	vector <int> A;
	A.push_back(j);
	// Traversing the Predecessors/Path Matrix to find Shortest Path.
	while(i!=j)
	{
		j = Path[i][j];
		A.push_back(j);
	}
	reverse(A.begin(),A.end());
	return A;
}

// Function to Print Warehouse Structure.
void PrintWarehouse(int Nb,int Na,int Ns,string **S)
{
	int m,n,L,b;
	L = m = Ns*Nb + Nb + 1;
	n = 3*Na;
	b = Ns + 1;

	bool **H;
   	H = new bool*[L];
   	for(int i=0;i<L;i++)
   		H[i] = new bool[n];

	bool **V;
   	V = new bool*[L];
   	for(int i=0;i<L;i++)
   		V[i] = new bool[n];

   	for(int i=0;i<L;i++)
      	for(int j=0;j<n;j++)
         	H[i][j] = 1;

    for(int i=0;i<L;i++)
      	for(int j=0;j<n;j++)
         	V[i][j] = 1;

    for(int j=1;j<n;j+=3)
    	for(int i=0;i<L;i++)
    		H[i][j] = 0;

    for(int i=0;i<=Nb;i++)
    	for(int j=0;j<n;j++)
    		V[i*b][j] = 0;     

    cout << "+";
    for(int j=0;j<n;j++)
      	cout << "---+";
    cout << endl;
    for(int i=m-1;i>=0;i--)
    {
      	cout << "|";
      	cout << "   ";
      	for(int j=0;j<n;j++)
      	{
         	if(j==n-1)
            	cout << "|";
         	else if(V[m-1-i][j]==1)
            	cout << "|";
         	else
            	cout << " ";
            cout << S[i][j];
      	}
      	cout << endl;
      	cout << "+";
      	for(int j=0;j<n;j++)
      	{
         	if(i==0)
            	cout << "---";
         	else if(H[m-1-i][j]==1)
            	cout << "---";
         	else
           	 	cout << "   ";
         	cout << "+";
      	}
      	cout << endl;
    }
}

// Function to Show Path in Warehouse denoted by 'X'.
void MakePath(vector <int> &A,int L,string **S,int Nodes)
{
	int temp;
	temp = A[0];
	for(int i=0;i<A.size();i++)
	{
		int a1,a2,a11;
		a1 = (int)A[i]/L;
		a2 = (int)A[i]%L;
		a1 = 3*a1;
		if(S[a2][a1]=="   ")
			S[a2][a1] = " X ";
		if(temp-A[i]>1)
		{
			a11 = (int)temp/L;
			a11 = 3*a11;
			while(a11!=a1)
			{
				if(S[a2][a11]=="   ")
					S[a2][a11] = " X ";
				a11--;
			}
		}
		if(A[i]-temp>1)
		{
			a11 = (int)temp/L;
			a11 = 3*a11;
			while(a11!=a1)
			{
				if(S[a2][a11]=="   ")
					S[a2][a11] = " X ";
				a11++;
			}
		}
		temp = A[i];
	}
}

// Function to Show Nodes to Visit in Warehouse.
void MarkNodesToVisit(int n,int *A,int L,string **S)
{
	// Marks the Node to be visited as 'O'
	for(int i=0;i<n;i++)
	{
		int a1,a2;
		a1 = (int)A[i]/L;
		a2 = (int)A[i]%L;
		a1 = 3*a1;
		if(S[a2][a1]=="   ")
			S[a2][a1] = " O ";
	}
}

// Finding the Probability used in Ant Colony Optimization.
void Find_Probability(int n,double Alpha,double Beta,double *Prob,double **Visibility,double **Pheromone,bool *Tabu,int present_node)
{
	double sum = 0;
	for(int i=0;i<n;i++)
	{
		if(Tabu[i]==1)
			sum += (double)pow(Pheromone[present_node][i],Alpha)*pow(Visibility[present_node][i],Beta);
		else
			Prob[i] = 0;
	}
	for(int i=0;i<n;i++)
	{
		if(Tabu[i]==1)
			Prob[i] = (double)pow(Pheromone[present_node][i],Alpha)*pow(Visibility[present_node][i],Beta)/sum;
	}
}

// Pheromone Update Function used in Ant Colony Optimization.
void Pheromone_Update(int n,vector <int> &X,vector <int> &Visited_Nodes,double **Pheromone,double **Dist,double Evap_Rate,double Pheromone_Laid,double &Best_Dist,int *Best_Route)
{
	double Lk = 0;

	// Calculating Distance need to be Travel for Current Solution.
	for(int i=1;i<n;i++)
		Lk += Dist[X[Visited_Nodes[i]]][X[Visited_Nodes[i-1]]];

	Lk += Dist[X[Visited_Nodes[0]]][X[Visited_Nodes[n-1]]];

	// Updating Shortest Distance found so far.
	if(Lk<Best_Dist)
	{
		Best_Dist = Lk;
		for(int i=0;i<n;i++)
			Best_Route[i] = X[Visited_Nodes[i]];
	}

	double con;
	con = (double)Pheromone_Laid/Lk;

	// Pheromone Evaporation.
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			Pheromone[i][j] = (1-Evap_Rate)*Pheromone[i][j];

	for(int i=1;i<n;i++)
		Pheromone[Visited_Nodes[i]][Visited_Nodes[i-1]] = Pheromone[Visited_Nodes[i]][Visited_Nodes[i-1]] + con;

	Pheromone[Visited_Nodes[0]][Visited_Nodes[n-1]] = Pheromone[Visited_Nodes[0]][Visited_Nodes[n-1]] + con;
}

int main()
{
	srand((unsigned int)time(NULL));

	clock_t begin, end;
	double time_spent;
	begin = clock();

	// Parameters of Warehouse
	int Nb,Na,Ns,kx,ky,Nodes;
	// Inputs to the Model.
	// Nb -> Number of Blocks.
	Nb = 3;
	// Na -> Number of Aisles.
	Na = 12;
	// Ns -> Number of Storage Location per Aisle Side.
	Ns = 3;
	// kx -> Distance between two Subsequent Aisles.
	kx = 4;
	// ky -> Distance between two Adjacent Picking Positions.
	ky = 2;

	// Parameters of Ant Colony Optimization.
	double Ants,Alpha,Beta,Best_Dist,Pheromone_Laid,Evap_Rate,Initial_Pheromone,iterations;
	int Start_Node;

	Alpha = 1;
	Beta = 1;
	// Number of Ants.
	Ants = 100;
	// Evaporation Rate.
	Evap_Rate = 0.01;
	// Initial Pheromone .
	Initial_Pheromone = 0.5;
	// Pheromone Laid.
	Pheromone_Laid = 2;
	// Number of Iterations.
	iterations  = 1000;
	// Starting Node.
	Start_Node = 0;

	// Nodes ->  Number of Nodes in System.
	Nodes = (Ns*Nb + Nb + 1)*Na;

	// Dist -> Distance Matrix.
	double **Dist;
	Dist = new double*[Nodes];
	for(int i=0;i<Nodes;i++)
		Dist[i] = new double[Nodes];

	// Path -> Predecessors Matrix.
	int **Path;
	Path = new int*[Nodes];
	for(int i=0;i<Nodes;i++)
		Path[i] = new int[Nodes];

	// Creates the Graph of Warehouse.
	CreateGraph(Nb,Na,Ns,kx,ky,Dist,Path,Nodes);

	// Finds Shortest Distance Between each Pair of Nodes.
	Floyd_Warshall(Dist,Path,Nodes);

	int n;

	cout << "Enter Number of Nodes to Visit: ";
	cin >> n;
	vector <int> X;
	cout << endl;
	cout << "Randomly Selected " << n << " Nodes" << endl;
	X.push_back(Start_Node);
	for(int i=1;i<=n;i++)
	{
		X.push_back(rand()%Nodes);
		cout << X[i] << " ";
	}

	n = n + 1;
	cout << endl;

	double **Pheromone;
	Pheromone = new double*[n];
	for(int v=0;v<n;v++)
    	Pheromone[v] = new double[n];

	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			Pheromone[i][j] = Initial_Pheromone;

	double **Visibility;
	Visibility = new double*[n];
	for(int v=0;v<n;v++)
    	Visibility[v] = new double[n];

	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(Dist[i][j]==0)
				Visibility[i][j] = 100000;
			else
				Visibility[i][j] = (double)(1/Dist[X[i]][X[j]]);
		}
	}

	bool *Tabu;
	Tabu = new bool[n];

	double *Prob;
	Prob = new double[n];

	double *Cumm_Prob;
	Cumm_Prob = new double[n];

	int *Best_Route;
	Best_Route = new int[n];

	double Prob_Next;
	int it = 1;
	Best_Dist = 10000000;

	while(it<=iterations)
	{
		for(int ants=0;ants<Ants;ants++)
		{
			int present_node = Start_Node;
			vector <int> Visited_Nodes;

			for(int i=0;i<n;i++)
				Tabu[i] = 1;

			Tabu[present_node] = 0;
			Visited_Nodes.push_back(present_node);

			for(int k=1;k<n;k++)
			{
				Find_Probability(n,Alpha,Beta,Prob,Visibility,Pheromone,Tabu,present_node);

				Prob_Next = (double)(rand()%1000000)/1000000;

				Cumm_Prob[0] = Prob[0];
				for(int i=1;i<n;i++)
					Cumm_Prob[i] = Cumm_Prob[i-1] + Prob[i];

				int NextNodeToGo = 0;
				while(Prob_Next>Cumm_Prob[NextNodeToGo])
					NextNodeToGo++;

				present_node = NextNodeToGo;
				Tabu[present_node] = 0;
				Visited_Nodes.push_back(present_node);
			}
			Pheromone_Update(n,X,Visited_Nodes,Pheromone,Dist,Evap_Rate,Pheromone_Laid,Best_Dist,Best_Route);
		}
		it++;
	}

	// Print Solution
	cout << endl;
	cout << "Best Route" << endl;

	for(int i=0;i<n;i++)
		cout << Best_Route[i] << " -> ";

	cout << Best_Route[0] << endl;
	cout << endl;
	cout << "Best Path" << endl;

	int L,b,n1;

	L = Ns*Nb + Nb + 1;
	b = Ns + 1;
	n1 = 3*Na;

	string **S;
   	S = new string*[L];
   	for(int i=0;i<L;i++)
   		S[i] = new string[n1];

    for(int i=0;i<L;i++)
    	for(int j=0;j<n1;j++)
    		S[i][j] = "   ";

    // Setting Starting Node Representation as 'S'.
    S[0][0] = " S ";	


    MarkNodesToVisit(n,Best_Route,L,S);

	for(int i=0;i<=n;i++)
	{
		vector <int> ShPath;

		if(i==0)
			ShPath = ReturnPath(Path,Start_Node,Best_Route[i]);
		else if(i==n)
			ShPath = ReturnPath(Path,Best_Route[i-1],Start_Node);
		else
			ShPath = ReturnPath(Path,Best_Route[i-1],Best_Route[i]);

		// Makes Path From one Node to Another
		MakePath(ShPath,L,S,Nodes);

		for(int j=0;j<ShPath.size()-1;j++)
			cout << ShPath[j] << " -> ";

		if(i==n)
			cout << ShPath[ShPath.size()-1] << " ";
	}

	cout << endl;
	cout << endl;
	cout << "Minimum Distance : " << Best_Dist << endl;
	cout << endl;

	// Prints Warehouse Structure.
	PrintWarehouse(Nb,Na,Ns,S);
	cout << endl;

	end = clock();
	time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
	cout << "Time Taken to Reach Solution " << time_spent << endl;

	return 0;
}
