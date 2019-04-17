// Viraj Patel
// 16IM10028

#include <iostream>
#include <vector>
#include <cmath>
// #include <stdlib.h>
// #include <stdio.h>

using namespace std;

void Inputs(int m,int d,int r,double *S,double *CD,double **C_PD,double **FC_PD,double **C_DR,double **FC_DR)
{
	// Suppliers/Plants Capacity.
	cout << "Enter Plant Capacities " ;
	for(int i=0;i<m;i++)
		cin >> S[i];

	// Retailers Demand.
	cout << "Enter Retailers Demand " ;
	for(int i=0;i<r;i++)
		cin >> CD[i];

	// Unit Transportation Cost Matrix between the Plant and the Distributor.
	cout << "Enter Unit Transportation Cost Matrix between Plants and Distributors" ;
	for(int i=0;i<m;i++)
		for(int j=0;j<d;j++)
			cin >> C_PD[i][j];

	// Fixed Cost Matrix for each Distribution between the Plant and the Distributor;
	cout << "Enter Fixed Cost Matrix between Plants and the Distributors" ;	
	for(int i=0;i<m;i++)
		for(int j=0;j<d;j++)
			cin >> FC_PD[i][j];

	// Unit Transportation Cost Matrix between the Distributor and the Retailer.
	cout << "Enter Unit Transportation Cost Matrix between Distributors and Retailers" ;
	for(int i=0;i<d;i++)
		for(int j=0;j<r;j++)
			cin >> C_DR[i][j];

	// Fixed Cost Matrix for each Distribution between the Distributor and the Retailer.
	cout << "Enter Fixed Cost Matrix between Distributors and Retailers" ;
	for(int i=0;i<d;i++)
		for(int j=0;j<r;j++)
			cin >> FC_DR[i][j];		
}

void Probability(int n1,int n2,double Alpha,double Beta,bool *Tabu,double **Cumm_Prob,double **Visibility,double **Pheromone)
{
	double sum;
	double Prob[n2];

	for(int k=0;k<n1;k++)
	{
		sum = 0;
		for(int j=0;j<n2;j++)
		{
			if(Tabu[j]==1)
				sum += (double)pow(Pheromone[k][j],Alpha)*pow(Visibility[k][j],Beta);
			else
				Prob[j] = 0;
		}

		for(int j=0;j<n2;j++)
			if(Tabu[j]==1)
				Prob[j] = (double)pow(Pheromone[k][j],Alpha)*pow(Visibility[k][j],Beta)/sum;

		Cumm_Prob[k][0] = Prob[0];

		for(int j=1;j<n2;j++)
			Cumm_Prob[k][j] = Cumm_Prob[k][j-1] + Prob[j];
	}
}

void Pheromone_Update(int r,int d,int m,double Evap_Rate,double Q,double **Pheromone_RD,double **Pheromone_DP,double **Best_Sol_RD,double **Best_Sol_DP,double &BestSolution)
{
	double con;
	con = (double)Q/BestSolution;

	for(int k=0;k<r;k++)
		for(int j=0;j<d;j++)
			Pheromone_RD[k][j] = Evap_Rate*Pheromone_RD[k][j];

	for(int j=0;j<d;j++)
		for(int i=0;i<m;i++)
			Pheromone_DP[j][i] = Evap_Rate*Pheromone_DP[j][i];

	for(int k=0;k<r;k++)
		for(int j=0;j<d;j++)
			if(Best_Sol_RD[k][j]!=0)
				Pheromone_RD[k][j] += con;

	for(int j=0;j<d;j++)
		for(int i=0;i<m;i++)
			if(Best_Sol_DP[j][i]!=0)
				Pheromone_DP[j][i] += con;
}

void BestSolutionSoFarUpdate(int r,int d,int m,double **Best_Sol_RD,double **Best_Sol_DP,double **Sol_RD,double **Sol_DP,double **C_DR,double **C_PD,double **FC_DR,double **FC_PD,double &BestSolution)
{
	double tc = 0;

	for(int k=0;k<r;k++)
		for(int j=0;j<d;j++)
			if(Sol_RD[k][j]!=0)
				tc = tc + FC_DR[j][k] + Sol_RD[k][j]*C_DR[j][k];

	for(int j=0;j<d;j++)
		for(int i=0;i<m;i++)
			if(Sol_DP[j][i]!=0)
				tc = tc + FC_PD[i][j] + Sol_DP[j][i]*C_PD[i][j];

	if(BestSolution>tc)
	{
		BestSolution = tc;
		for(int k=0;k<r;k++)
			for(int j=0;j<d;j++)
				Best_Sol_RD[k][j] = Sol_RD[k][j];

		for(int j=0;j<d;j++)
			for(int i=0;i<m;i++)
				Best_Sol_DP[j][i] = Sol_DP[j][i];
	}		
}

int main()
{
	// Initialization.
	long N,itterations,INF = 999999999;
	double Alpha,Beta,Evap_Rate,Initial_Pheromone,Q;
	itterations = 10;
	N = 100;
	Alpha = 2;
	Beta = 5;
	Evap_Rate = 0.9;
	Q = 500;
	Initial_Pheromone = 0.5;

	// m -> Manufacturing Plant || d -> Distributor || r -> Retailers
	int m,d,r;
	cout << "Enter Number of Manufacturing Plants: ";
	cin >> m;
	cout << "Enter Number of Distributors: ";
	cin >> d;
	cout << "Enter Number of Retailers: ";
	cin >> r;

	// Memory Allocation for Inputs

	// S -> Supplier Capacity.
	double *S;
	S = new double[m];

	// CD -> Retailers Demand
	double *CD;
	CD = new double[r];

	// Unit Transportation Cost Matrix between Plants and Distributors C_PD[i][j]
	double **C_PD;
	C_PD = new double*[m];
	for(int i=0;i<m;i++)
		C_PD[i] = new double[d];

	// Fixed cost matrix for each distribution route representing the fixed cost for each route, FC_PD[i][j]
	// between the plant and the distributor;
	double **FC_PD;
	FC_PD = new double*[m];
	for(int i=0;i<m;i++)
		FC_PD[i] = new double[d];

	// Unit transportation cost matrix between the distributor and the retailer C_DR[i][j]
	double **C_DR;
	C_DR = new double*[d];
	for(int i=0;i<d;i++)
		C_DR[i] = new double[r];

	// Fixed cost matrix for each distribution route representing the fixed cost for each route, FC_DR[j][k]
	// between the distributor and the retailer.
	double **FC_DR;
	FC_DR = new double*[d];
	for(int i=0;i<d;i++)
		FC_DR[i] = new double[r];


	// Function to Read Inputs
	Inputs(m,d,r,S,CD,C_PD,FC_PD,C_DR,FC_DR);


	// Initializing Important and Useful Matrices.

	// Pheromone from Retailers to Distributors.
	double **Pheromone_RD;
	Pheromone_RD = new double*[r];
	for(int i=0;i<r;i++)
    	Pheromone_RD[i] = new double[d];

	for(int i=0;i<r;i++)
		for(int j=0;j<d;j++)
			Pheromone_RD[i][j] = Initial_Pheromone;

	// Pheromone from Distributorsto Plants.
	double **Pheromone_DP;
	Pheromone_DP = new double*[d];
	for(int i=0;i<d;i++)
    	Pheromone_DP[i] = new double[m];

	for(int i=0;i<d;i++)
		for(int j=0;j<m;j++)
			Pheromone_DP[i][j] = Initial_Pheromone;

	// Expected Allocation Cost from Retailers to Distributors.
	double **EAC_RD;
	EAC_RD = new double*[r];
	for(int i=0;i<r;i++)
    	EAC_RD[i] = new double[d];

    for(int k=0;k<r;k++)
    	for(int j=0;j<d;j++)
    		EAC_RD[k][j] = FC_DR[j][k] + C_DR[j][k]*CD[k];

    // Expected Allocation Cost from Distributors to Plants.
    double **EAC_DP;
	EAC_DP = new double*[d];
	for(int i=0;i<d;i++)
    	EAC_DP[i] = new double[m];

    // Visibility from Retailers to Distributors.
	double **Visibility_RD;
	Visibility_RD = new double*[r];
	for(int i=0;i<r;i++)
    	Visibility_RD[i] = new double[d];

    // Visibility from Distributors to Plants.
    double **Visibility_DP;
	Visibility_DP = new double*[d];
	for(int i=0;i<d;i++)
    	Visibility_DP[i] = new double[m];
	
	// Tabu List of Distributors.
    bool *Tabu_D;
	Tabu_D = new bool[d];

	// Tabu List of Plants.
	bool *Tabu_P;
	Tabu_P = new bool[m];

	// Cummulative Probability from Retailers to Distributors.
	double **Cumm_Prob_RD;
	Cumm_Prob_RD = new double*[r];
	for(int i=0;i<r;i++)
    	Cumm_Prob_RD[i] = new double[d];

    // Cummulative Probability from Distributors to Plants. 
	double **Cumm_Prob_DP;
	Cumm_Prob_DP = new double*[d];
	for(int i=0;i<d;i++)
    	Cumm_Prob_DP[i] = new double[m];

    // Best Solution from Retailers to Distributors.
	double **Best_Sol_RD;
	Best_Sol_RD = new double*[r];
	for(int i=0;i<r;i++)
		Best_Sol_RD[i] = new double[d];

	for(int k=0;k<r;k++)
		for(int j=0;j<d;j++)
			Best_Sol_RD[k][j] = INF;

	// Best Solution from Distributors to Plants.
	double **Best_Sol_DP;
	Best_Sol_DP = new double*[d];
	for(int i=0;i<d;i++)
		Best_Sol_DP[i] = new double[m];

	for(int j=0;j<d;j++)
		for(int i=0;i<m;i++)
			Best_Sol_DP[j][i] = INF;

	// Current Solution from Retailers to Distributors.
	double **Sol_RD;
	Sol_RD = new double*[r];
	for(int i=0;i<r;i++)
		Sol_RD[i] = new double[d];

	// Current Solution from Distributors to Plants.
	double **Sol_DP;
	Sol_DP = new double*[d];
	for(int i=0;i<d;i++)
		Sol_DP[i] = new double[m];

	// Distributors Capacity List.
	double *Distributor_Cap;
	Distributor_Cap = new double[d];

	for(int i=0;i<d;i++)
		Distributor_Cap[i] = 0;

	// Plants Capacity List.
	double *Plant_Cap;
	Plant_Cap = new double[m];

	int IT=1;
	vector <int> Retailer,Distributor;
	int Random_Retailer,Random_Distributor,Random_no,DistToGo,PlantToGo;
	double Prob_Next,BestSolution = INF;
	
	while(IT<=itterations)
	{
		for(int ants=0;ants<N;ants++)
		{
			// Stage 1
			for(int k=0;k<r;k++)
				for(int j=0;j<d;j++)
					Sol_RD[k][j] = 0;

			for(int j=0;j<d;j++)
				for(int i=0;i<m;i++)
					Sol_DP[j][i] = 0;

			for(int i=0;i<d;i++)
				Distributor_Cap[i] = 0;

			for(int i=0;i<d;i++)
				Tabu_D[i] = 1;

			for(int k=0;k<r;k++)
				for(int j=0;j<d;j++)
					Visibility_RD[k][j] = (double)1/EAC_RD[k][j];

			Probability(r,d,Alpha,Beta,Tabu_D,Cumm_Prob_RD,Visibility_RD,Pheromone_RD);

			for(int i=0;i<r;i++)
				Retailer.push_back(i);

			while(Retailer.size())
			{
				Random_no = rand()%Retailer.size();
				Random_Retailer = Retailer[Random_no];
				Retailer.erase(Retailer.begin()+Random_no);
				Prob_Next = (double)(rand()%1000000)/1000000;
				DistToGo = 0;
				while(Prob_Next>Cumm_Prob_RD[Random_no][DistToGo])
					DistToGo++;
				Distributor_Cap[DistToGo] += CD[Random_Retailer];
				Sol_RD[Random_Retailer][DistToGo] = CD[Random_Retailer];
			}


			// Stage 2
			for(int j=0;j<d;j++)
    			for(int i=0;i<m;i++)
    				EAC_DP[j][i] = FC_PD[i][j] + C_PD[i][j]*Distributor_Cap[j];

    		for(int i=0;i<m;i++)
    		{
    			Tabu_P[i] = 1;
    			Plant_Cap[i] = S[i];
    		}

    		for(int j=0;j<d;j++)
    			for(int i=0;i<m;i++)
    				Visibility_DP[j][i] = (double)1/EAC_DP[j][i];

    		Probability(d,m,Alpha,Beta,Tabu_P,Cumm_Prob_DP,Visibility_DP,Pheromone_DP);

			for(int i=0;i<d;i++)
				Distributor.push_back(i);

			while(Distributor.size())
			{
				Random_no = rand()%Distributor.size();
				Random_Distributor = Distributor[Random_no];
				Prob_Next = (double)(rand()%1000000)/1000000;
				PlantToGo = 0;

				while(Prob_Next>Cumm_Prob_DP[Random_no][PlantToGo])
					PlantToGo++;

				if(Plant_Cap[PlantToGo]>=Distributor_Cap[Random_Distributor])
				{
					Plant_Cap[PlantToGo] = Plant_Cap[PlantToGo] - Distributor_Cap[Random_Distributor];
					Sol_DP[Random_Distributor][PlantToGo] = Distributor_Cap[Random_Distributor];
					Distributor_Cap[Random_Distributor] = 0;
				}
				else
				{
					Distributor_Cap[Random_Distributor] = Distributor_Cap[Random_Distributor] - Plant_Cap[PlantToGo];
					Sol_DP[Random_Distributor][PlantToGo] = Plant_Cap[PlantToGo];
					Plant_Cap[PlantToGo] = 0;
					Tabu_P[PlantToGo] = 0;
				}

				if(Distributor_Cap[Random_Distributor]==0)
					Distributor.erase(Distributor.begin()+Random_no);

				Probability(d,m,Alpha,Beta,Tabu_P,Cumm_Prob_DP,Visibility_DP,Pheromone_DP);

			}
			BestSolutionSoFarUpdate(r,d,m,Best_Sol_RD,Best_Sol_DP,Sol_RD,Sol_DP,C_DR,C_PD,FC_DR,FC_PD,BestSolution);
		}
		Pheromone_Update(r,d,m,Evap_Rate,Q,Pheromone_RD,Pheromone_DP,Best_Sol_RD,Best_Sol_DP,BestSolution);
		IT++;
	}

	//Print Solution
	cout << endl;
	cout << "Plants to Distributors" << endl;
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<d;j++)
			cout << Best_Sol_DP[j][i] << " ";
		cout << endl;
	}
	cout << endl;
	cout << "Distributors to Retailers" << endl;
	for(int j=0;j<d;j++)
	{
		for(int k=0;k<r;k++)
			cout << Best_Sol_RD[k][j] << " ";
		cout << endl;
	}
	cout << endl;
	cout << "Minimum Cost: " << BestSolution << endl;
	return 0;
}