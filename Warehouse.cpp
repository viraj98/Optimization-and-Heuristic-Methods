#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <cstring>

using namespace std;

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
    for(int i=0;i<m;i++)
    {
      	cout << "|";
      	cout << "   ";
      	for(int j=0;j<n;j++)
      	{
         	if(j==n-1)
            	cout << "|";
         	else if(V[i][j]==1)
            	cout << "|";
         	else
            	cout << " ";
            cout << S[i][j];
      	}
      	cout << endl;
      	cout << "+";
      	for(int j=0;j<n;j++)
      	{
         	if(i==m-1)
            	cout << "---";
         	else if(H[i][j]==1)
            	cout << "---";
         	else
           	 	cout << "   ";
         	cout << "+";
      	}
      	cout << endl;
    }
}

int main()
{
	int Nb,Na,Ns,L,b,n,Nodes;
	Nb = 3;
	Na = 12;
	Ns = 3;

	L = Ns*Nb + Nb + 1;
	b = Ns + 1;
	n = 3*Na;
	Nodes = (Ns*Nb + Nb + 1)*Na;

   	string **S;
   	S = new string*[L];
   	for(int i=0;i<L;i++)
   		S[i] = new string[n];

    for(int i=0;i<L;i++)
    	for(int j=0;j<n;j++)
    		S[i][j] = "   ";     

    S[12][0] = " S ";

 //    int n1,Start_Node;
 //    Start_Node = 0;
	// cout << "Enter Number of Nodes: ";
	// cin >> n1;
	// vector <int> X;
	// cout << endl;
	// cout << "Randomly Selected " << n1 << " Nodes" << endl;
	// // X.push_back(Start_Node);
	// cout << Nodes << endl;
	// for(int i=0;i<n1;i++)
	// {
	// 	X.push_back(rand()%Nodes);
	// 	cout << X[i] << " ";
	// }
	// cout << endl;
	// cout << "L: " << L << endl;
	// for(int i=0;i<n1;i++)
	// {
	// 	int a1,a2;
	// 	a1 = (int)X[i]/L;
	// 	a2 = (int)X[i]%L;
	// 	a1 = 3*a1;
	// 	cout << a1 << " " << a2 << endl;
	// 	S[a2][a1] = " X ";
	// }
    cout << endl;
    cout << endl;

    PrintWarehouse(Nb,Na,Ns,S);		  

    cout << endl;
    cout << endl;

	return 0;
}