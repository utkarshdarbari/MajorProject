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
void Find_Mode(std::vector<std::string> *data,std::string *mean)
{
int val;
std::string line,most_used_line;
std::multimap<std::string,int> mode_Finder;
for(int i=0;i<3;i++)
	{
		mode_Finder.clear();
		int max=0;
		most_used_line=data[i][0];
		for(int j=0;j<3;j++)
		{
			line=data[i][j];
			if(mode_Finder.find(line)==mode_Finder.end())
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
		mean[i]=most_used_line;
	}
}
int main ()
{
//std::cout<<"Check 1";
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
std::string mean[3];
Find_Mode(data,mean);
for(int i=0;i<3;i++)
std::cout<<mean[i]<<"\t";
return 0;
}
