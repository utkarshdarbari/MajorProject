#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<set>
#include <iterator> 


int convert_File_Get_Row_Size(std::string file_Name)
{
	std::ifstream reader;
	std::ofstream writer;
	reader.open(file_Name);
	writer.open("csv_File.csv");
	std::string line="";
	int count=0;
	while(reader.good())
	{
		count++;
		getline(reader,line,'\n');
		writer<<line<<"\n";
	};
	return count-1;
}


int get_No_of_Cols(std::string file_Name)
{
	std::ifstream reader2;
	int count=0;
	reader2.open(file_Name);
	std::string line="";
	getline(reader2,line,'\n');
	for(int i=0;i<line.length();i++)
	{
		if(line[i]==',')
			count++;
	}
	reader2.close();
	return count+1;
}


int set_Target_Col()
{
	int target;
	std::cout<<"Enter the target Column\n";
	std::cin>>target;
	return target;
}


void display_File()
{
	system("./setPermission.sh");
	system("./showCSVfile.sh");
}


int remove_Col(std::set<int>&remove_Col_Idx,int target_Col)
{
	int idx,remove_Size;
	std::cout<<"Enter No of cols to remove?\n";
	std::cin>>remove_Size;
	for(int i=0;i<remove_Size;i++)
	{
		std::cout<<"Enter Col Number in digits\n";
		std::cin>>idx;
		if(idx!=target_Col)
		remove_Col_Idx.insert(idx);
	}
	return remove_Size;
}


void store_Col(std::string file_Name,std::set<int>&remove_Col_Idx,std::vector<std::string> *data,int total_Cols,int target_Col,int remaining_Col)
{
	int i,j;
	std::ifstream reader;
	std::string line=""; 
	reader.open(file_Name);
	std::set <int > :: iterator itr; 
	while(reader.good())
	{
		int count=0;
		itr=remove_Col_Idx.begin();
		for(i=1,j=0;i<total_Cols;i++)
		{
			count++;
			getline(reader,line,',');
			if(*itr==i)
			{
				itr++;
			}
			else if(i==target_Col)
			{
				data[remaining_Col-1].push_back(line);
			}
			else
			{
				data[j].push_back(line);
				j++;
			}
		}
		getline(reader,line,'\n');
		if(target_Col-1==count)
		{
			data[remaining_Col-1].push_back(line);
		}
		else
		{
			data[j].push_back(line);
		}
	}
}


void store_Vector_To_File(std::vector<std::string> *data,int total_Rows,int remaining_Col)
{
	std::ofstream writer;
	writer.open("Module1.csv");
	for(int i=0;i<total_Rows;i++)
	{
		for(int j=0;j<remaining_Col;j++)
		{
			writer<<data[j][i]<<",";
		}
		writer<<"\n";
	}
		writer.close();
		system("./showCSVfileModule1.sh");
}


int main()
{
	//module1 
	int total_Rows,total_Cols,removed_Cols,target_Col,remaining_Col;
	std::cout<<"Enter the name of the CSV file\n";
	std::string file_Name="";
	std::cin>>file_Name;
	total_Rows=convert_File_Get_Row_Size(file_Name);
	total_Cols=get_No_of_Cols(file_Name);
	display_File();
	std::cout<<"Checkpoint 1";
	target_Col=set_Target_Col();
	std::cout<<"Checkpoint 2";
	std::cout<<"Current Dimentions are "<< total_Rows<<"*"<<total_Cols<<"\n";
	std::set<int> remove_Col_Idx;
		std::cout<<"Checkpoint 2.4";

	removed_Cols=remove_Col(remove_Col_Idx,target_Col);
	std::cout<<"Checkpoint 2.6";

	remaining_Col=total_Cols - removed_Cols;
	std::vector<std::string> data[remaining_Col+1];
	std::cout<<"Checkpoint 3";

	if(remaining_Col>1)
	store_Col(file_Name,  remove_Col_Idx,  data, total_Cols,target_Col,remaining_Col);
	//store_Vector_To_File(data,total_Rows,remaining_Col);
	for(int i=0;i<total_Rows;i++)
	{
		for(int j=0;j<remaining_Col;j++)
		{
			std::cout<<data[j][i]<<"\t";
		}
		std::cout<<std::endl;
	}
	//module 1 complete
	//module 2
}