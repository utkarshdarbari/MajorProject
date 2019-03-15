#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<sstream>
#include<set>
#include <iterator> 

std::string get_Csv_File_Name()
{
std::cout<<"Enter the Name of the CSV File along with extention\n";
std::string file_Name;
std::cin>>file_Name;
return file_Name;
}


int store_File_Get_Total_Rows(std::string file_Name)
{
	std::ifstream reader;
	std::ofstream writer;
	reader.open(file_Name);
	writer.open("Module1.csv");	//save to csv_File.csv
	std::string line="";
	int count=0;
	while(reader.good())
	{
		count++;
		getline(reader,line,'\n');//read line
		writer<<line<<"\n";		//store line
	};
	reader.close();
	writer.close();
	return count-1;	//return No of rows
}


int get_Total_Cols(std::string file_Name)
{
	std::ifstream reader;
	int count=0;
	reader.open(file_Name);
	std::string line="";
	getline(reader,line,'\n');
	for(int i=0;i<line.length();i++)
	{
		if(line[i]==',')
			count++;
	}
	reader.close();
	return count+1;
}


void display_File()
{
	system("./setPermission.sh");
	system("./showCSVfile.sh");
}


int set_Target_Col()
{
	int target;
	std::cout<<"Enter the target Column\n";
	std::cin>>target;
	return target;
}


int get_Col_Remove_Index(std::set<int>&remove_Col_Idx,int target_Col)
{
	int idx,remove_Size;
	bool target_Remove=0;
	std::cout<<"Enter No of cols to remove?\n";
	std::cin>>remove_Size;
	for(int i=0;i<remove_Size;i++)
	{
		std::cout<<"Enter Col Number in digits\n";
		std::cin>>idx;
		if(idx==target_Col)
		{
			std::cout<<"ERROR: You cannot remove the target coloumn\n";
			target_Remove=1;
		}
		else
		remove_Col_Idx.insert(idx);
	}
	if(target_Remove==1)
		return remove_Size-1;
	else
		return remove_Size;
}



void store_To_Vector(	std::string file_Name,
					std::set<int>&remove_Col_Idx,
					std::vector<std::string> *data,
					int total_Cols,
					int target_Col,
					int remaining_Cols,
					int *empty_Values_Per_Col)
{
	int i,j;
	std::ifstream reader;
	std::string line=""; 
	reader.open(file_Name);
	std::set <int> :: iterator itr; 
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
				data[remaining_Cols-1].push_back(line);
				if(line=="")
					empty_Values_Per_Col[remaining_Cols-1]++;
			}
			else
			{
				data[j].push_back(line);
				if(line=="")
					empty_Values_Per_Col[j]++;
				j++;
			}
		}
		getline(reader,line,'\n');
		if(target_Col-1==count)
		{
			data[remaining_Cols-1].push_back(line);
			if(line=="")
				empty_Values_Per_Col[remaining_Cols-1]++;
		}
		else
		{
			data[j].push_back(line);
			if(line=="")
				empty_Values_Per_Col[j]++;
		}
	}
	reader.close();
}


void store_Vector_To_File	(std::vector<std::string> *data,
							int total_Rows,int remaining_Cols,
							int limit,int *empty_Values_Per_Col)
{
	std::ofstream writer;
	writer.open("Module1.csv");
	bool target_Above_Limit=0;
	bool fill_started=0;
	for(int i=0;i<total_Rows;i++)
	{
		fill_started=0;
		for(int j=0;j<remaining_Cols;j++)
		{
			if(empty_Values_Per_Col[j]>limit)
			{
				if(j==remaining_Cols-1)
				{
					writer<<","<<data[j][i];
					target_Above_Limit=1;
				}
			}
			else
			{
				if(fill_started==1)
				writer<<","<<data[j][i];
				else
				{
					writer<<data[j][i];
					fill_started=1;
				}
			}
		}
		writer<<"\n";
	}
		writer.close();
		if(target_Above_Limit)
			std::cout<<"WARNING: Too many missing values in Target Column\n";
		system("./showCSVfileModule1.sh");
}


void check_Data_Type(int i,std::string line,int *col_Data_Type)
{
	bool is_Integer=1;
	int count_Decimal=0;
		for(int j=0;j<line.length();j++)
		{
			if(line[j]<='9'&&line[j]>='0')
			{
				is_Integer=1;
			}
			else if(line[j]=='.')
				count_Decimal++;
			else
			{
				is_Integer=0;
				break;
			}
		}
		if(is_Integer==0)
			col_Data_Type[i]=0;
		else if(is_Integer==1&&count_Decimal<2)
			col_Data_Type[i]=1;
		else if(is_Integer==1&&count_Decimal>2)
			col_Data_Type[i]=0;
		else 
			col_Data_Type[i]=2;
}




int find_Data_Type(std::string file_Name,int *col_Data_Type)
{
	int remaining_Cols=get_Total_Cols(file_Name);
	std::ifstream reader;
	reader.open(file_Name);
	std::string line="";
	int count_Decimal,i;
	bool is_Integer;
	for( i=0;i<remaining_Cols-1;i++)
	{
	getline(reader,line,',');
	check_Data_Type(i,line,col_Data_Type);
	}
	getline(reader,line,'\n');
	check_Data_Type(i,line,col_Data_Type);
	reader.close();
	return remaining_Cols;
}


int main()
{
//MODULE 1

std::string file_Name;
file_Name=get_Csv_File_Name();//read CSV file Name

int total_Rows;
total_Rows=store_File_Get_Total_Rows(file_Name);//get Row size and store scv file to Module1.csv

int total_Cols;
total_Cols=get_Total_Cols(file_Name);//get Number of Cols in CSV file

display_File();//display the CSV file

int target_Col;
target_Col=set_Target_Col();//set the target col

std::set<int> remove_Col_Idx;
int removed_Cols;
removed_Cols=get_Col_Remove_Index(remove_Col_Idx,target_Col);//find index of cols to remove

int remaining_Cols;
remaining_Cols	=	total_Cols - removed_Cols;//remaining cols i.e new file will have remainingCols as No of Cols

std::vector<std::string> data[remaining_Cols+1];
int empty_Values_Per_Col[remaining_Cols+1];
memset(empty_Values_Per_Col,0,sizeof(empty_Values_Per_Col));//initialize array to zero
store_To_Vector(file_Name,  remove_Col_Idx,  data, total_Cols,target_Col,remaining_Cols,empty_Values_Per_Col);
//we now have Empty values per Col stored in the array

int limit= (total_Rows * 75)/100;
store_Vector_To_File(data,total_Rows,remaining_Cols,limit,empty_Values_Per_Col);

//MODULE 1 END;
//MODULE 2 

int col_Data_Type[remaining_Cols+1];
memset(col_Data_Type,0,sizeof(col_Data_Type));//initialize array to zero
remaining_Cols=find_Data_Type("Module1.csv",col_Data_Type);//1 is string, 0 is int, and 2 is null remaining cols are updated

//MODULE 2 END;
//MODULE 3

}