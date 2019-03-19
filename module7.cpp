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



void store_Outlier_Rows(int current_Col,
						int total_Rows,
						double lower_Bound1,
						double upper_Bound1,
						std::vector<std::string> *data,
						std::set<int> &outlier_Row_Idx)
{
	std::cout<<"entered\n";
	double val;
	for(int i=0;i<total_Rows;i++)
	{
		val=stod(data[current_Col][i]);
		std::cout<<val<<" ";
		if(val>upper_Bound1||val<lower_Bound1)
			{
				outlier_Row_Idx.insert(i);
				std::cout<<val;
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
	std::cout<<lower_Bound[current_Col]<<" "<<upper_Bound[current_Col]<<"\n";
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
	std::vector<std::string> data[3];
	data[0].push_back("345");
	data[0].push_back("45");
	data[0].push_back("23");
	data[0].push_back("18555");
	data[0].push_back("200000");
	data[1].push_back("Bravo");
	data[1].push_back("Bro");
	data[1].push_back("Bob");
	data[1].push_back("Bark");
	data[1].push_back("Bristle");
	data[2].push_back("1");
	data[2].push_back("3");
	data[2].push_back("5");
	data[2].push_back("57");
	data[2].push_back("103");

	int col_Data_Type[3]={0,0,0};
	int remaining_Cols=3,total_Rows=5;
	int sparse_Idx[3]={1};
	double lower_Bound[remaining_Cols];
	double upper_Bound[remaining_Cols];
	std::set<int> outlier_Row_Idx;
	check_Outlier(data,sparse_Idx,remaining_Cols,total_Rows,upper_Bound,lower_Bound,outlier_Row_Idx);
	
}