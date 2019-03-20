#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<vector>
#include<sstream>
#include<algorithm>
#include<set>
#include <iterator> 
#include <map>

int convert_Sparse_Cols(std::vector<std::string> *data,
					int remaining_Cols,int totalRows, 
					bool *sparse_Idx,
					std::vector<int> *sparse_Matrix_Guide)
{
int sparse_Editing_Idx=0;
std::multimap<std::string,int> mapper;
	for(int j=0;j<remaining_Cols;j++)
	{
		int idx=0;
		if(sparse_Idx[j]==1)
		{
			mapper.clear();
			std::string line;
			for (int i = 0; i < totalRows; i++)
			{
				line=data[j][i];
				if(mapper.find(line)==mapper.end())
					{
						mapper.insert(std::make_pair
							(line,idx+sparse_Editing_Idx));
						idx++;
					}
				sparse_Matrix_Guide[j].push_back(mapper.find(line)->second);
			}
			sparse_Editing_Idx=sparse_Editing_Idx+idx;
		}
	}
	return sparse_Editing_Idx;
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

	int col_Data_Type[3]={1,1,1};
	int remaining_Cols=3,total_Rows=3;
	bool sparse_Idx[3]={1,1,1};
	int no_Of_Sparse=3;
	std::vector<int> sparse_Matrix_Guide[no_Of_Sparse];
	int total_New_Columns=convert_Sparse_Cols(data,remaining_Cols,total_Rows,sparse_Idx,sparse_Matrix_Guide);
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			std::cout<<sparse_Matrix_Guide[i][j]<<" ";
	}
	std::cout<<"\n"<<total_New_Columns;

}