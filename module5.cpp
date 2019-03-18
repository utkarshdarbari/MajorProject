#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <stdio.h>     
#include <stdlib.h>
#include <iterator> 
#include <map>

void rank_Ordinal(std::vector<std::string> *data,
					int total_Rows,int index)
{
	std::multimap<std::string,std::string> mapper;
	mapper.clear();
	std::string line,line2;
	for(int i=0;i<total_Rows;i++)
	{
		line=data[index][i];
		if(mapper.find(line)==mapper.end())
		{
			std::cout<<"Enter rank for "<<line<<"\n";
			std::cin>>line2;
			mapper.insert(std::make_pair(line,line2));
		}
			data[index][i]=mapper.find(line)->second;
	}
}


void check_Nominal_Ordinal(std::vector<std::string> *data,
							int *col_Data_Type,int total_Rows,
							int remaining_Cols,int *sparse_Idx)
{
	bool order_exists=0;
	for(int i=0,idx=0;i<remaining_Cols;i++)
	{
		if(col_Data_Type[i]!=1)
		{
			std::cout<<"\nDoes the string "<<i+1<<" have an order \n";
			std::cout<<"Press 1 if order/rank exist, Press 0 otherwise\n";
			std::cin>>order_exists;		
			if(order_exists)
			{
				rank_Ordinal(data,total_Rows,i);
			}
			else
			{
				sparse_Idx[idx]=i;
				idx++;
			}
		}
	}
}
int main()
{
	std::vector<std::string> data[3];
	data[0].push_back("Ajay");
	data[0].push_back("Aditya");
	data[0].push_back("Ajay");
	data[1].push_back("Bravo");
	data[1].push_back("Bro");
	data[1].push_back("Bob");
	data[2].push_back("Carl");
	data[2].push_back("Carl");
	data[2].push_back("Ajay");

	int col_Data_Type[3]={0,0,0};
	int sparse_Convertions=0;
	int remaining_Cols=3,total_Rows=3;
	int sparse_Idx[3];

	check_Nominal_Ordinal(data,col_Data_Type,total_Rows,
		remaining_Cols,sparse_Idx);
	std::cout<<"Exited the function";

}