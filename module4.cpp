#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<vector>
#include<sstream>
#include<set>
#include <iterator> 
#include <map>
using namespace std;

void find_Mean(std::vector<std::string> *data,
			int *col_Data_Type,
			double *mean,
			int Total_Rows,
			int remaining_Cols)
	{
		for(int i=0;i<remaining_Cols;i++)
		{
			if(col_Data_Type[i]!=0)
			{
				mean[i]=0;
			}
			else
			{
				double sum=0;
				int counter=0;
				for (int j = 0; j < Total_Rows; j++)
				{
					if(data[i][j]!="")
					{
					sum=sum+stod(data[i][j]);
					counter++;
					}
				}
				mean[i]=sum/counter;
			}
		}

	}


int main ()
{
//std::cout<<"Check 1";
std::vector<std::string> data[3];
data[0].push_back("Ajay");
data[0].push_back("Aditya");
data[0].push_back("Ajay");
data[1].push_back("1");
data[1].push_back("1.1");
data[1].push_back("1.2");
data[2].push_back("5");
data[2].push_back("6");
data[2].push_back("7");
double mean[3];
int Total_Rows=3,remaining_Cols=3;
int col_Data_Type[3]={1,0,0};


find_Mean(data,col_Data_Type,mean,Total_Rows,remaining_Cols);
for(int i=0;i<3;i++)
std::cout<<mean[i]<<"\t";
return 0;
}

