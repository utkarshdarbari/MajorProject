#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<vector>
#include<sstream>
#include<set>
#include <iterator> 
#include <map>

void min_Max_Normalization(std::string *min,
							std::string *max,
							std::vector<std::string> *data,
							int *sparse_Idx,int remaining_Cols,
							int total_Rows)
{
	double max1,min1,val,ans;
	for (int i = 0,idx = 0; i < remaining_Cols; i++)
	{
		max1=stod(max[i]);
		min1=stod(min[i]);
		if(i!=sparse_Idx[idx])
		{
			for(int j = 0; j<total_Rows;j++)
			{
				val=stod(data[i][j]);
				ans=(val-min1)/(max1-min1);
				data[i][j]=std::to_string(ans);
			}
		}
		else
			idx++;
	}
}







int main()
{
	std::vector<std::string> data[3];
	data[0].push_back("345");
	data[0].push_back("45");
	data[0].push_back("18");
	data[1].push_back("Bravo");
	data[1].push_back("Bro");
	data[1].push_back("Bob");
	data[2].push_back("1");
	data[2].push_back("3");
	data[2].push_back("5");

	int col_Data_Type[3]={0,0,0};
	std::string min[3]={"18","0","1"};
	std::string max[3]={"345","0","5"};
	int remaining_Cols=3,total_Rows=3;
	int sparse_Idx[3]={1};

	min_Max_Normalization(min,max,data,sparse_Idx,remaining_Cols,total_Rows);
	std::cout<<data[0][0]<<" "<<data[0][1]<<" "<<data[0][2]<<"\n";
	std::cout<<data[1][0]<<" "<<data[1][1]<<" "<<data[1][2]<<"\n";
	std::cout<<data[2][0]<<" "<<data[2][1]<<" "<<data[2][2]<<"\n";
	std::cout<<"Exited the function";

}