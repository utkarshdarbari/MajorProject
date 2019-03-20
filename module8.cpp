void store_To_Final_File(std::vector<std::string> *data,
						int remaining_Cols,int total_Rows,
						std::set<int> &outlier_Row_Index,
						std::vector<int> *sparse_Matrix_Guide,
						int total_New_Columns)
{
	ofstream writer;
	writer.open("Module1.csv");
	std::set<int>::iterator itr=outlier_Row_Index.begin();
	for (int i = 0; i < total_Rows; i++)
	{
	bool fill_started=0;
	if(*itr!=i)
	{
		for(j = 0;j< remaining_Cols-1;j++)
		{
			if(sparse_Idx!=1)
			{
				if(fill_started)
					writer<<","<<data[j][i];
				else
				{
					writer<<data[j][i];
					fill_started=1;
				}
			}
		}
		for(j=0;j<total_New_Columns;j++)
		{
			if(sparse_Matrix_Guide[idx][i]==j)
				{
					writer<<",1";
					idx++;
				}
			else
				writer<<",0";
		}
		writer<<","data[remaining_Cols-1][i]<<"\n";
	}
	else
		itr++;
	}
	writer.close();
	system("./showCSVfileModule1.sh");
}