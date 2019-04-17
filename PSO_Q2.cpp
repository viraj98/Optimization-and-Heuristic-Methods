#include <iostream>
#include <vector>
#include <cmath>
#include <limits.h>

using namespace std;

float Objective(float X1,float X2)
{
	float sum1=0,sum2=0;
	for(int i=1;i<=5;i++)
		sum1 += i*cos((i+1)*X1+i);
	for(int i=1;i<=5;i++)
		sum2 += i*cos((i+1)*X2+i);
	float Fcurr = sum1*sum2;
	return Fcurr;
}

float UpdatePosition(float X,float V)
{
	float Xcurr = X + V;
	return Xcurr;
}

float UpdateVelocity(float w,float c1,float c2,float gbest,float pbest,float X,float V)
{
	float Vcurr = w*V + c1*((rand()%100)/100)*(pbest-X) + c2*((rand()%100)/100)*(gbest-X);
	return Vcurr;
}

int main()
{
	float w,c1,c2,itt,sos,pbest1,pbest2,gbest;
	w = 0.7;c1 = 0.2;c2 = 0.6;sos = 10;itt = 1000;gbest=10000;
	vector <float> Xcurr1,Xcurr2;
	vector <float> Vcurr1,Vcurr2;
	vector <float> pbestPosition1,pbestPosition2;
	vector <float> pbestFitness;
	
	// Initialization 
	for(int i=0;i<sos;i++)
	{
		Xcurr1.push_back(rand()%255);
		Xcurr2.push_back(rand()%255);
		Vcurr1.push_back((rand()%100)/100-0.5);
		Vcurr2.push_back((rand()%100)/100-0.5);
	}
	for(int i=0;i<sos;i++)
	{
		pbestPosition1.push_back(Xcurr1[i]);
		pbestPosition2.push_back(Xcurr2[i]);
	}

	for(int i=0;i<sos;i++)
		pbestFitness.push_back(Objective(Xcurr1[i],Xcurr2[i]));

	for(int i=0;i<sos;i++)
	{
		if(pbestFitness[i]<gbest)
			gbest = pbestFitness[i];
	}
	float fitness;

	for(int j=0;j<itt;j++)
	{
		for(int i=0;i<sos;i++)
		{
			fitness = Objective(Xcurr1[i],Xcurr2[i]);
			if(fitness<gbest)
			{
				gbest = fitness;
				pbest1 = Xcurr1[i];
				pbest2 = Xcurr2[i];
			}
			if(fitness<pbestFitness[i])
			{
				pbestFitness[i] = fitness;
				pbestPosition1[i] = Xcurr1[i];
				pbestPosition2[i] = Xcurr2[i];
			}
			Vcurr1[i] = UpdateVelocity(w,c1,c2,gbest,pbest1,Xcurr1[i],Vcurr1[i]);
			Vcurr2[i] = UpdateVelocity(w,c1,c2,gbest,pbest2,Xcurr2[i],Vcurr2[i]);
			Xcurr1[i] = UpdatePosition(Xcurr1[i],Vcurr1[i]);
			Xcurr2[i] = UpdatePosition(Xcurr2[i],Vcurr2[i]);
			if(Xcurr1[i]>255)
				Xcurr1[i] = 255;
			if(Xcurr2[i]>255)
				Xcurr2[i] = 255;
			if(Xcurr1[i]<0)
				Xcurr1[i] = 0;
			if(Xcurr2[i]<0)
				Xcurr2[i] = 0;
		}
	}

	cout << "Global Best Position: ";
	cout << (int)pbest1 << " " << (int)pbest2 << endl;
	cout << "Global Best Solution: ";
	cout << gbest << endl;

	return 0;
}