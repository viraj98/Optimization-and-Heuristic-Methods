#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>

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

float Probability(float f1,float f2,float temp)
{
	float Prob = exp(-(f2-f1)/temp);
	return Prob;
}

int main()
{
	float InitialTemp,FinalTemp,TempDec,TempMul;
	int itterations;
	InitialTemp = 500;
	FinalTemp = 5;
	TempMul = 0.95;
	TempDec = 3;
	itterations = 100;

	int x1 = rand()%255;
	int x2 = rand()%255;

	float Temp,NewSol_x1,NewSol_x2,prob,rnd,f1,f2;
	int w,w1;
	Temp = InitialTemp;
	w = 55;
	w1 = 10;

	while(Temp>=FinalTemp)
	{
		for(int it=1;it<=itterations;it++)
		{

			NewSol_x1 = x1 + ((float)(rand()%100)/100-0.5)*(rand()%w);
			NewSol_x2 = x2 + ((float)(rand()%100)/100-0.5)*(rand()%w);

			if(NewSol_x1>255)
			{
				NewSol_x1 = 255;
				NewSol_x1 = x1 - ((float)(rand()%100)/100)*(rand()%w1);
			}
			if(NewSol_x2>255)
			{
				NewSol_x2 = 255;
				NewSol_x1 = x1 - ((float)(rand()%100)/100)*(rand()%w1);
			}
			if(NewSol_x1<0)
			{
				NewSol_x1 = 0;
				NewSol_x1 = x1 + ((float)(rand()%100)/100)*(rand()%w1);
			}
			if(NewSol_x2<0)
			{
				NewSol_x2 = 0;
				NewSol_x2 = x2 + ((float)(rand()%100)/100)*(rand()%w1);
			}

			f1 = Objective(x1,x2);
			f2 = Objective(NewSol_x1,NewSol_x2);

			if(f2>=f1)
			{
				prob = Probability(f1,f2,Temp);
				rnd = (float)(rand()%1000)/1000;

				if(prob>rnd)
				{
					x1 = NewSol_x1;
					x2 = NewSol_x2;
				}
			}
			else
			{
				x1 = NewSol_x1;
				x2 = NewSol_x2;
			}
		}
		Temp = TempMul*Temp;
// 		Temp = Temp - TempDec;
	}

	// Print Solution
	cout << endl;
	cout << "X1 = " << x1 << endl;
	cout << "X2 = " << x2 << endl;
	cout << "Best Solution: " << Objective(x1,x2) << endl;

	return 0;
}