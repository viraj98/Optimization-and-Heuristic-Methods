#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;

void Objective(vector <float> &Xcurr,vector <float> &Fcurr)
{
	for(int i=0;i<Xcurr.size();i++)
		Fcurr[i] = 1 - Xcurr[i]*Xcurr[i] + 2*Xcurr[i];
}

void UpdatePosition(vector <float> &Xcurr,vector <float> &Vcurr)
{
	for(int i=0;i<Xcurr.size();i++)
		Xcurr[i] = Xcurr[i] + Vcurr[i];
}

void UpdateVelocity(float w,float c1,float c2,float gbest,vector <float> &pbestPosition,vector <float> &Xcurr,vector <float> &Vcurr)
{
	for(int i=0;i<Vcurr.size();i++)
		Vcurr[i] = w*Vcurr[i] + c1*((rand()%10)/10)*(pbestPosition[i]-Xcurr[i]) + c2*((rand()%10)/10)*(gbest-Xcurr[i]);
}

int main()
{
	float w,c1,c2,itt,sos,m1,pbest;
	pbest = 0;w = 0.7;c1 = 0.2;c2 = 0.6;sos = 10;itt = 100;
	vector <float> Xcurr;
	vector <float> Vcurr;
	vector <float> Fcurr;
	vector <float> pbestPosition,pbestFitness;

	for(int i=0;i<sos;i++)
	{
		Fcurr.push_back(-10000);
		pbestFitness.push_back(-10000);
	}
	
	float gbest=INT_MIN;
	// Initialization 
	for(int i=0;i<sos;i++)
	{
		Xcurr.push_back((rand()%500)/100-0.5);
		pbestPosition.push_back(Xcurr[i]);
		Vcurr.push_back((rand()%100)/100-0.50);
	}

	Objective(pbestPosition,pbestFitness);
	for(int i=0;i<sos;i++)
	{
		if(pbestFitness[i]>gbest)
			gbest = pbestFitness[i];
		
		pbestPosition.push_back(Xcurr[i]);
	}

	for(int i=0;i<itt;i++)
	{
		Objective(Xcurr,Fcurr);
		for(int i=0;i<sos;i++)
		{
			if(Fcurr[i]>gbest)
			{
				gbest = Fcurr[i];
				pbest = Xcurr[i];
			}
			
			if(Fcurr[i]>pbestFitness[i])
			{
				pbestFitness[i] = Fcurr[i];
				pbestPosition[i] = Xcurr[i];
			}
		}
		UpdateVelocity(w,c1,c2,gbest,pbestPosition,Xcurr,Vcurr);
		UpdatePosition(Xcurr,Vcurr);
	}
	cout << "Global Best Position: ";
	cout << pbest << endl;
	cout << "Global Best Solution: ";
	cout << gbest << endl;

	return 0;
}