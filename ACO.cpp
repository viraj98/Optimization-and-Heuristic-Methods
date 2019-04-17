#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>

using namespace std;

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

void Pheromone_Update(int n,vector <int> &Visited_Nodes,double **Pheromone,double **Dist,double Evp_rate,double Pheromone_Laid,double &Best_Dist,int *Best_Route)
{
	double Lk = 0;
	for(int i=1;i<n;i++)
		Lk += Dist[Visited_Nodes[i]][Visited_Nodes[i-1]];
	
	Lk += Dist[Visited_Nodes[0]][Visited_Nodes[n-1]];

	if(Lk<Best_Dist)
	{
		Best_Dist = Lk;
		for(int i=0;i<n;i++)
			Best_Route[i] = Visited_Nodes[i];
	}

	double con;
	con = (double)Pheromone_Laid/Lk;

	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			Pheromone[i][j] = (1-Evp_rate)*Pheromone[i][j];

	for(int i=1;i<n;i++)
		Pheromone[Visited_Nodes[i]][Visited_Nodes[i-1]] = Pheromone[Visited_Nodes[i]][Visited_Nodes[i-1]] + con;	
	
	Pheromone[Visited_Nodes[0]][Visited_Nodes[n-1]] = Pheromone[Visited_Nodes[0]][Visited_Nodes[n-1]] + con;
}

int main()
{
	int n;
	n = 20;
	vector <double> X{0.0333692,0.6020896,0.1532083,0.3181124,0.1878440,0.9786112,0.7918010,0.5145329,0.5500754,0.3893757,0.9641841,0.7718126,0.7549037,0.2837881,0.3308411,0.7977221,0.3221207,0.9201035,0.2397964,0.3967470};
	vector <double> Y{0.9925079,0.0168807,0.7020444,0.1469288,0.8679120,0.4925364,0.7943144,0.0363478,0.8324617,0.6635483,0.6400201,0.5463923,0.4584584,0.7733415,0.1974851,0.1193149,0.7930478,0.1186234,0.1448552,0.6716172};

	double Alpha,Beta,Best_Dist,Pheromone_Laid,Evp_rate,Initial_Pheromone;
	Initial_Pheromone = 0.5;
	Best_Dist = 10000;
	Alpha = 1;
	Beta = 1;
	Evp_rate = 0.01;
	Pheromone_Laid = 5;

	// int n;
	// cout << "Enter Number of Cities: ";
	// cin >> n;
	// vector <double> X,Y;
	// double x,y;
	// for(int i=0;i<n;i++)
	// {
	// 	cin >> x >> y;
	// 	X.push_back(x);
	// 	Y.push_back(y);
	// }

	double **Dist;
	Dist = new double*[n];
	for(int v=0;v<n;v++)
    	Dist[v] = new double[n];
    cout << endl;
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			Dist[i][j] = pow((pow(X[i]-X[j],2)+pow(Y[i]-Y[j],2)),0.5);

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
				Visibility[i][j] = (1/Dist[i][j]);
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
	int itt,it=1;
	itt = 1000;
	while(it<=itt)
	{
		int present_node = rand()%n;
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
			for(int i=1;i<n;i++)
			{
				if(Prob_Next>=Cumm_Prob[i-1] && Prob_Next<Cumm_Prob[i])
				{
					NextNodeToGo = i;
					break;
				}
			}

			present_node = NextNodeToGo;
			Tabu[present_node] = 0;
			Visited_Nodes.push_back(present_node);
		}

		Pheromone_Update(n,Visited_Nodes,Pheromone,Dist,Evp_rate,Pheromone_Laid,Best_Dist,Best_Route);
		// cout << it << " ==> " << Best_Dist << endl;
		it++;
	}

	cout << "Minimum Distance: "; 
	cout << Best_Dist << endl;
	for(int i=0;i<n;i++)
		cout << Best_Route[i]+1 << " -> ";
	cout << Best_Route[0]+1 << endl;
	cout << endl;
	return 0;
}