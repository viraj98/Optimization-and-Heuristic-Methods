#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdlib.h>

using namespace std;

double Objective(vector <bool> &A)
{
	double ans,sum1=0,sum2=0;
	int x1,x2;
	for(int i=0;i<8;i++)
		x1 += A[i]*pow(2,i);
	for(int i=0;i<8;i++)
		x2 += A[i+8]*pow(2,i);

	for(int i=1;i<=5;i++)
		sum1 += i*cos((i+1)*x1+i);
	for(int i=1;i<=5;i++)
		sum2 += i*cos((i+1)*x2+i);
	ans = sum1*sum2;
	return ans;
}

void CrossOver(vector < vector <bool> > &Mating_Pool,int i1,int j1)
{
	int d1 = Mating_Pool.size();
	int temp,k1,k2;
	k1 = rand()%d1 ;
	k2 = rand()%d1 ;
	if(k1>k2)
		swap(k1,k2);
	for(int i=k1;i<=k2;i++)
	{
		temp = Mating_Pool[i1][i];
		Mating_Pool[i1][i] = Mating_Pool[j1][i];
		Mating_Pool[j1][i] = temp;
	}
}

void Mutation(vector <bool> &A)
{
	int d = A.size();
	int k1 = rand()%d ;
	A[k1] = !A[k1];
}

void Tournment_Selection(vector < vector <bool> > &Initial_Pop,vector < vector <bool> > &Mating_Pool,int size)
{
	for(int i=0;i<size;i++)
	{
		int k1 = rand()%size,k2 = rand()%size;
		double s1 = Objective(Initial_Pop[k1]);
		double s2 = Objective(Initial_Pop[k2]);
		if(s1<s2)
			Mating_Pool[i] = Initial_Pop[k1];
		else
			Mating_Pool[i] = Initial_Pop[k2];
	}
}

int main()
{
	int Pop_size=100,itt=1000,I=1;
	vector < vector <bool> > Initial_Pop(Pop_size);
	vector < vector <bool> > Mating_Pool(Pop_size);

	// Initialize Population
	for(int i=0;i<Pop_size;i++)
		for(int j=0;j<16;j++)
			Initial_Pop[i].push_back(rand()%2);

	for(int i=0;i<Pop_size;i++)
		Mating_Pool[i] = Initial_Pop[i];

	// Itteration
	float CrossProb=0.9,MutationProb=0.4;
	double mn = 10000;
	// for(int I=0;I<itt;I++)
	while(I<=itt)
	{
		// Tournment Selection
		Tournment_Selection(Initial_Pop,Mating_Pool,Pop_size);
		// cout << "Itteration "  << I << endl;
		vector <float> CrossP,MutationP;
		vector <bool> IfCross,IfMut;

		for(int i=0;i<Pop_size;i++)
		{
			IfCross.push_back(((rand()%10)/10<CrossProb?1:0));
			IfMut.push_back(((rand()%10)/10<MutationProb?1:0));
		}
		int i1=0,j1=Pop_size-1;
		// CrossOver
		while(i1<j1)
		{
			while(IfCross[i1]==0)
				i1++;
			while(IfCross[j1]==0)
				j1--;
			CrossOver(Mating_Pool,i1,j1);
			i1++;
			j1--;
		}
		// Mutation
		for(int i=0;i<Pop_size;i++)
		{
			if(IfMut[i]==1)
				Mutation(Mating_Pool[i]);
		}
		// Minimum Objective Until Now
		for(int i=0;i<Pop_size;i++)
		{
			double s1 = Objective(Mating_Pool[i]);
			mn = min(mn,s1);
			Initial_Pop[i] = Mating_Pool[i];
		}
		// cout << "Minimum Objective Value Until Now : " << mn << endl;
		I++;
	}
	cout << endl;
	cout << "Minimum Objective Value : ";
	cout << mn << endl;
	return 0;
}