#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<sstream>
#include<algorithm>
#include<set>
#include <iterator> 
#include <map>

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


int get_Col_Remove_Index(std::set<int>&remove_Col_Idx,
								int target_Col)
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
	remove_Col_Idx.clear();
	reader.close();
}


void store_Vector_To_File	(std::vector<std::string> *data,
							int total_Rows,
							int remaining_Cols,
							int limit,
							int *empty_Values_Per_Col)
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
	for(int j=0;j<remaining_Cols;j++)
		{
			data[j].clear();
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

void find_Mode(std::vector<std::string> *data,
				std::string *mode,int total_Rows,
				int remaining_Cols)
{
	int val;
	std::string line,most_used_line;
	std::multimap<std::string,int> mode_Finder;
	for(int i=0;i<remaining_Cols;i++)
		{
			mode_Finder.clear();
			int max=0;
			most_used_line=data[i][0];
			for(int j=0;j<total_Rows;j++)
			{
				line=data[i][j];
				if(line=="")
					;
				else if(mode_Finder.find(line)==mode_Finder.end())
				{
					mode_Finder.insert (std::make_pair(line,1));
					//std::cout << "  Insert\n";
				}
				else
				{
				    mode_Finder.find(line)->second++;
				    val=mode_Finder.find(line)->second;
				    //std::cout<<"  Add count\n";
				    if(val>max)
				    {
				    	max=val;
				    	most_used_line=line;
				    }
				}	
			}
			mode[i]=most_used_line;
		}
}

void find_Mean(std::vector<std::string> *data,
			int *col_Data_Type,
			double *mean,
			int total_Rows,
			int remaining_Cols,
			std::string *max,
			std::string *min)
{
	for(int i=0;i<remaining_Cols;i++)
	{
		if(col_Data_Type[i]!=1)
		{
			mean[i]=0;
			max[i]="0";
			min[i]="0";
		}
		else
		{
			double val1,max1,min1;
			max1=stod(data[i][0]);
			min1=max1;
			double sum=0;
			int counter=0;
			for (int j = 0; j < total_Rows; j++)
			{
				if(data[i][j]!="")
				{
				val1=stod(data[i][j]);
				sum=sum+val1;
				counter++;
				if(val1>max1)
					max1=val1;
				if(val1<min1)
					min1=val1;
				}

			}
			mean[i]=sum/counter;
			max[i]=std::to_string(max1);
			min[i]=std::to_string(min1);
		}
	}

}


void fill_Missing_Values(double *mean
	,std::string *mode
	,std::vector<std::string> *data
	,int remaining_Cols
	,int total_Rows
	,int *col_Data_Type)
	{
		for (int i = 0; i < remaining_Cols; i++)
		{
			for (int j = 0; j < total_Rows; j++)
			{
				if(data[i][j]==""&&col_Data_Type[i]==1)
					data[i][j]=std::to_string(mean[i]);//fill mean
				else 
				{
					if(data[i][j]=="")
					data[i][j]=mode[i];
				}
			}
		}
	}

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


int check_Nominal_Ordinal(std::vector<std::string> *data,
							int *col_Data_Type,int total_Rows,
							int remaining_Cols,int *sparse_Idx)
{
	int idx=0;
	bool order_exists=0;
	for(int i=0,idx=0;i<remaining_Cols;i++)
	{
		if(col_Data_Type[i]!=1)
		{
			std::cout<<"\nDoes the Column "<<i+1<<" have an order \nSample of Column "<<i+1<<"\n";
			for(int j=0;j<5&&j<total_Rows;j++)
				std::cout<<data[i][j]<<"\n";
			std::cout<<"\nPress 1 if order/rank exist, Press 0 otherwise\n";
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
	//return idx;//returns number of sparse strings
}


void min_Max_Normalization(std::string *min,
							std::string *max,
							std::vector<std::string> *data,
							int *col_Data_Type,
							int remaining_Cols,
							int total_Rows)
{
	double max1,min1,val,ans;
	for (int i = 0; i < remaining_Cols; i++)
	{
		max1=stod(max[i]);
		min1=stod(min[i]);
		if(col_Data_Type[i]==1)
		{
			for(int j = 0; j<total_Rows;j++)
			{
				val=stod(data[i][j]);
				ans=(val-min1)/(max1-min1);
				data[i][j]=std::to_string(ans);
			}
		}
	}
}

void store_Outlier_Rows(int current_Col,
						int total_Rows,
						double lower_Bound1,
						double upper_Bound1,
						std::vector<std::string> *data,
						std::set<int> &outlier_Row_Idx)
{
	//std::cout<<"entered\n";
	double val;
	for(int i=0;i<total_Rows;i++)
	{
		val=stod(data[current_Col][i]);
		//std::cout<<val<<" ";
		if(val>upper_Bound1||val<lower_Bound1)
			{
				outlier_Row_Idx.insert(i);
				std::cout<<i+1<<" ";
			}
	}

}



void outlier_Detection(std::vector<std::string> *data,
							int current_Col,
							int total_Rows,
							double *upper_Bound,
							double *lower_Bound,
							std::set<int>&outlier_Row_Idx)
{
	int median1,median2;
	std::vector<double> sort_Data;
	sort_Data.clear();
	for(int i=0;i<total_Rows;i++)
	{
		sort_Data.push_back(stod(data[current_Col][i]));
	}
	sort(sort_Data.begin(),sort_Data.end());
	for(int i=0;i<sort_Data.size();i++)
	median1=total_Rows/4;
	median2=(total_Rows*3)/4;
	double q1=sort_Data[median1];
	double q3=sort_Data[median2];
	double iqr = q3 - q1;
	lower_Bound[current_Col]=q1-(1.5*iqr);
	upper_Bound[current_Col]=q3+(1.5*iqr);
	//std::cout<<lower_Bound[current_Col]<<" "<<upper_Bound[current_Col]<<"\n";
	store_Outlier_Rows(current_Col,total_Rows,lower_Bound[current_Col],upper_Bound[current_Col],data,outlier_Row_Idx);
}



void check_Outlier(std::vector<std::string> *data,
							int *sparse_Idx, 
							int remaining_Cols,
							int total_Rows,
							double *upper_Bound,
							double *lower_Bound,
							std::set<int>&outlier_Row_Idx)
{
	for (int i = 0,idx=0; i < remaining_Cols; i++)
	{
		if(sparse_Idx[idx]!=i)
			outlier_Detection(data,i,total_Rows,upper_Bound,lower_Bound,outlier_Row_Idx);
		else
			idx++;
	}
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
//std::cout<<remaining_Cols;
//MODULE 2 END;
//MODULE 3
store_To_Vector("Module1.csv",remove_Col_Idx,data,remaining_Cols,remaining_Cols,remaining_Cols,empty_Values_Per_Col);
std::string mode[remaining_Cols];
find_Mode(data,mode,total_Rows,remaining_Cols);
//MODULE 3 END
//MODULE 4
double mean[remaining_Cols];
std::string min[remaining_Cols],max[remaining_Cols];
find_Mean(data,col_Data_Type,mean,total_Rows,remaining_Cols,max,min);
std::cout<<"After Filling Missing Values\n";
fill_Missing_Values(mean,mode,data,remaining_Cols,total_Rows,col_Data_Type);
//MODULE 4 END
//MODULE 5A
memset(empty_Values_Per_Col,0,sizeof(empty_Values_Per_Col));//initialize array to zero
store_Vector_To_File(data,total_Rows,remaining_Cols,limit,empty_Values_Per_Col);
int sparse_Idx[remaining_Cols+1];
int sparse_String_count=0;
memset(sparse_Idx,remaining_Cols+1,sizeof(sparse_Idx));
sparse_String_count=check_Nominal_Ordinal(data,col_Data_Type,total_Rows,remaining_Cols,sparse_Idx);
//memset(empty_Values_Per_Col,0,sizeof(empty_Values_Per_Col));//initialize array to zero
std::cout<<"After Converting Ordinal Strings\n";
store_Vector_To_File(data,total_Rows,remaining_Cols,limit,empty_Values_Per_Col);
//MODULE 5A END
//MODULE 6 
min_Max_Normalization(min,max,data,col_Data_Type,remaining_Cols,total_Rows);
std::cout<<"After normalization\n";
store_Vector_To_File(data,total_Rows,remaining_Cols,limit,empty_Values_Per_Col);
//MODULE 6 END
//MODULE 7
double lower_Bound[remaining_Cols];
double upper_Bound[remaining_Cols];
std::set<int> outlier_Row_Idx;
check_Outlier(data,sparse_Idx,remaining_Cols,total_Rows,upper_Bound,lower_Bound,outlier_Row_Idx);




return 0;
}