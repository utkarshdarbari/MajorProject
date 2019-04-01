#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<sstream>
#include<algorithm>
#include<set>
#include <iterator> 
#include <map>

bool check_Csv_File_Extention(std::string file_Name){
  int l = file_Name.length();
  if(l>4)
  {
    if(file_Name[l-4]=='.'&&file_Name[l-3]=='c'&&file_Name[l-2]=='s'&&file_Name[l-1]=='v')
    return 0;
    else
    {
    std::cout << "\nERROR: The entered file name is not a CSV file, Please include extention too\n\n";
    return 1; 
    }
  }
  else
    {
      std::cout << "\nERROR: The entered filename too short\n\n";
      return 1;
    }
}

std::string get_Csv_File_Name() {
  bool is_csv=1;
  std::string file_Name="";
  while(is_csv){
  std::cout << "Enter the Name of the CSV File along with extention\n";
  std::cin >> file_Name;
  is_csv=check_Csv_File_Extention(file_Name);
  };
  return file_Name;
}

void store_To_File(std::string file_Name)
{
  std::string line;
  std::ifstream reader;
  std::ofstream writer;
  reader.open(file_Name);
  writer.open("Module1.csv");
  while(reader.good())
  {
    getline(reader,line,'\n');
    writer<<line<<"\n";
  };
  reader.close();
  writer.close();
}

int get_Total_Rows(std::string file_Name)
{
  int count=0;
  std::string line;
  std::ifstream reader;
  reader.open(file_Name);
  while(reader.good())
  {
    getline(reader,line,'\n');
    count++;
  };
  reader.close();
  return count-1;
}


int get_Total_Cols(std::string file_Name)
{
  int count=0;
  std::ifstream reader;
  std::string line;
  reader.open(file_Name);
  getline(reader,line,'\n');
  for (int i = 0; i < line.length(); i++)
  {
    if(line[i]==',')
      count++;
  }
  reader.close();
  return count+1;
}

int set_Target_Col(int total_Cols) {
  bool is_valid_target=1;
  int target;
  while(is_valid_target){
  std::cout << "Enter the target Column by Column number\n";
  std::cin >> target;
  if(target<=total_Cols&&target>0)
  is_valid_target=0;
  else
    std::cout<<"Target Not in Range\nTarget range should be between 1 and "<<total_Cols<<"\n\n";
  };
  return target;
}


int remove_Cols(std::set < int > & remove_Col_Idx, int target_Col,int total_Cols)
{
  int idx, remove_Size;
  bool target_Remove = 0;
  std::cout << "Enter Number of Columns to remove?\n";
  std::cin >> remove_Size;
  for (int i = 0; i < remove_Size; i++) {
    std::cout << "Enter Column Number in digits\n";
    std::cin >> idx;
    if (idx == target_Col) {
      std::cout << "ERROR: You cannot remove the target column\n";
      target_Remove = 1;
    } else if(idx>total_Cols||idx<1)
    {
       std::cout << "ERROR: Column not in range\n";
    }
    else
    {
      if(remove_Col_Idx.find(idx)==remove_Col_Idx.end())
      remove_Col_Idx.insert(idx);
      else
        std::cout<<"WARNING: The current row is already queued to be removed\n";
    }
  }
   return remove_Col_Idx.size();
}


void store_To_Vector(std::string file_Name,
  std::set < int > & remove_Col_Idx,
  std::vector < std::string > * data,
  int total_Cols,
  int target_Col,
  int remaining_Cols,
  int * empty_Values_Per_Col) {
  int i, j;
  std::ifstream reader;
  std::string line = "";
  reader.open(file_Name);
  std::set < int > ::iterator itr;
  while (reader.good()) {
    int count = 0;
    itr = remove_Col_Idx.begin();
    for (i = 1, j = 0; i < total_Cols; i++) {
      count++;
      getline(reader, line, ',');
      if ( * itr == i) {
        itr++;
      } else if (i == target_Col) {
        data[remaining_Cols - 1].push_back(line);
        if (line == "")
          empty_Values_Per_Col[remaining_Cols - 1]++;
      } else {
        data[j].push_back(line);
        if (line == "")
          empty_Values_Per_Col[j]++;
        j++;
      }
    }
    getline(reader, line, '\n');
    if (target_Col - 1 == count) {
      data[remaining_Cols - 1].push_back(line);
      if (line == "")
        empty_Values_Per_Col[remaining_Cols - 1]++;
    } else {
      data[j].push_back(line);
      if (line == "")
        empty_Values_Per_Col[j]++;
    }
  }
  remove_Col_Idx.clear();
  reader.close();
}



void store_Vector_To_File(std::vector < std::string > * data,
  int total_Rows,
  int remaining_Cols,
  int limit,
  int * empty_Values_Per_Col) {
  std::ofstream writer;
  writer.open("Module1.csv");
  bool target_Above_Limit = 0;
  bool fill_started = 0;
  for (int i = 0; i < total_Rows; i++) {
    fill_started = 0;
    for (int j = 0; j < remaining_Cols; j++) {
      if (empty_Values_Per_Col[j] > limit) {
        if (j == remaining_Cols - 1) {
          writer << "," << data[j][i];
          target_Above_Limit = 1;
        }
      } else {
        if (fill_started == 1)
          writer << "," << data[j][i];
        else {
          writer << data[j][i];
          fill_started = 1;
        }
      }
    }
    writer << "\n";
  }
  for (int j = 0; j < remaining_Cols; j++) {
    data[j].clear();
  }
  writer.close();
  if (target_Above_Limit)
    std::cout << "WARNING: Too many missing values in Target Column\n";
  //system("./showCSVfileModule1.sh");
}


void show_Labels(std::string file_Name,int total_Cols)
{
  int i;
  std::string line="";
  std::ifstream reader;
  reader.open(file_Name);
  for (i=1;i<total_Cols;i++) 
      {
        std::cout<<i<<" -> ";
        getline(reader,line,',');
        std::cout<<line<<"\n";
      }
    std::cout<<i<<" -> ";
    getline(reader,line,'\n');
    std::cout<<line<<"\n";
    reader.close();
}

void check_Data_Type(int i, std::string line, int * col_Data_Type) {
  bool is_Integer = 1;
  int count_Decimal = 0;
  for (int j = 0; j < line.length(); j++) {
    if (line[j] <= '9' && line[j] >= '0') {
      is_Integer = 1;
    } else if (line[j] == '.')
      count_Decimal++;
    else {
      is_Integer = 0;
      break;
    }
  }
  if (is_Integer == 0)
    col_Data_Type[i] = 0;
  else if (is_Integer == 1 && count_Decimal < 2)
    col_Data_Type[i] = 1;
  else if (is_Integer == 1 && count_Decimal > 2)
    col_Data_Type[i] = 0;
  else
    col_Data_Type[i] = 2;
}

int find_Data_Type(std::string file_Name, int * col_Data_Type) {
  int remaining_Cols = get_Total_Cols(file_Name);
  std::ifstream reader;
  reader.open(file_Name);
  std::string line = "";
  getline(reader,line,'\n');
  int count_Decimal, i;
  bool is_Integer;
  for (i = 0; i < remaining_Cols - 1; i++) {
    getline(reader, line, ',');
    check_Data_Type(i, line, col_Data_Type);
  }
  getline(reader, line, '\n');
  check_Data_Type(i, line, col_Data_Type);
  reader.close();
  return remaining_Cols;
}


void find_Mode(std::vector < std::string > * data,
  std::string * mode, int total_Rows,
  int remaining_Cols) {
  int val;
  std::string line, most_used_line;
  std::multimap < std::string, int > mode_Finder;
  for (int i = 0; i < remaining_Cols; i++) {
    mode_Finder.clear();
    int max = 0;
    most_used_line = data[i][0];
    for (int j = 0; j < total_Rows; j++) {
      line = data[i][j];
      if (line == "")
      ;
      else if (mode_Finder.find(line) == mode_Finder.end()) {
        mode_Finder.insert(std::make_pair(line, 1));
        //std::cout << "  Insert\n";
      } else {
        mode_Finder.find(line) -> second++;
        val = mode_Finder.find(line) -> second;
        //std::cout<<"  Add count\n";
        if (val > max) {
          max = val;
          most_used_line = line;
        }
      }
    }
    mode[i] = most_used_line;
  }
}

void find_Mean(std::vector < std::string > * data,
  int * col_Data_Type,
  double * mean,
  int total_Rows,
  int remaining_Cols,
  std::string * max,
  std::string * min) {
  for (int i = 0; i < remaining_Cols; i++) {
    if (col_Data_Type[i] != 1) {
      mean[i] = 0;
      max[i] = "0";
      min[i] = "0";
    } else {
      double val1, max1, min1;
      max1 = stod(data[i][0]);
      min1 = max1;
      double sum = 0;
      int counter = 0;
      for (int j = 0; j < total_Rows; j++) {
        if (data[i][j] != "") {
          val1 = stod(data[i][j]);
          sum = sum + val1;
          counter++;
          if (val1 > max1)
            max1 = val1;
          if (val1 < min1)
            min1 = val1;
        }

      }
      mean[i] = sum / counter;
      max[i] = std::to_string(max1);
      min[i] = std::to_string(min1);
    }
  }
}


void fill_Missing_Values(double * mean, std::string * mode, std::vector < std::string > * data, int remaining_Cols, int total_Rows, int * col_Data_Type) {
  for (int i = 0; i < remaining_Cols; i++) {
    for (int j = 0; j < total_Rows; j++) {
      if (data[i][j] == "" && col_Data_Type[i] == 1)
        data[i][j] = std::to_string(mean[i]); //fill mean
      else {
        if (data[i][j] == "")
          data[i][j] = mode[i];
      }
    }
  }
  std::cout<<"Missing Values are filled\n";
}

void display_File() {
  system("./setPermission.sh");
  system("./showCSVfile.sh");
}

void rank_Ordinal(std::vector < std::string > * data,
  int total_Rows, int index) {
  std::multimap < std::string, std::string > mapper;
  mapper.clear();
  std::string line, line2;
  for (int i = 0; i < total_Rows; i++) {
    line = data[index][i];
    if (mapper.find(line) == mapper.end()) {
      std::cout << "Enter rank for " << line << "\n";
      std::cin >> line2;
      mapper.insert(std::make_pair(line, line2));
    }
    data[index][i] = mapper.find(line) -> second;
  }
}

void check_Nominal_Ordinal(std::vector < std::string > * data,
  int * col_Data_Type, int total_Rows,
  int remaining_Cols, bool * sparse_Idx) {
  int idx = 0;
  bool order_exists = 0;
  for (int i = 0; i < remaining_Cols; i++) {
    if (col_Data_Type[i] != 1) {
      std::cout << "\nDoes the Column " << i + 1 << " have an order \nSample of Column " << i + 1 << "\n";
      for (int j = 0; j < 5 && j < total_Rows; j++)
        std::cout << data[i][j] << "\n";
      std::cout << "\nPress 1 if order/rank exist, Press 0 otherwise\n";
      std::cin >> order_exists;
      if (order_exists) {
        sparse_Idx[i] = 0;
        rank_Ordinal(data, total_Rows, i);
      } else {
        sparse_Idx[i] = 1;
      }
    }
  }
}

void min_Max_Normalization(std::string * min,
  std::string * max,
  std::vector < std::string > * data,
  int * col_Data_Type,
  int remaining_Cols,
  int total_Rows) {
  double max1, min1, val, ans;
  for (int i = 0; i < remaining_Cols; i++) {
    max1 = stod(max[i]);
    min1 = stod(min[i]);
    if (col_Data_Type[i] == 1) {
      for (int j = 0; j < total_Rows; j++) {
        val = stod(data[i][j]);
        ans = (val - min1) / (max1 - min1);
        data[i][j] = std::to_string(ans);
      }
    }
  }
}

void store_Outlier_Rows(int current_Col,
  int total_Rows,
  double lower_Bound1,
  double upper_Bound1,
  std::vector < std::string > * data,
  std::set < int > & outlier_Row_Idx) {
  //std::cout<<"entered\n";
  double val;
  for (int i = 0; i < total_Rows; i++) {
    val = stod(data[current_Col][i]);
    //std::cout<<val<<" ";
    if (val > upper_Bound1 || val < lower_Bound1) {
      outlier_Row_Idx.insert(i);
    }
  }

}

void outlier_Detection(std::vector < std::string > * data,
  int current_Col,
  int total_Rows,
  double * upper_Bound,
  double * lower_Bound,
  std::set < int > & outlier_Row_Idx) {
  int median1, median2;
  std::vector < double > sort_Data;
  sort_Data.clear();
  for (int i = 0; i < total_Rows; i++) {
    sort_Data.push_back(stod(data[current_Col][i]));
  }
  sort(sort_Data.begin(), sort_Data.end());
  for (int i = 0; i < sort_Data.size(); i++)
    median1 = total_Rows / 4;
  median2 = (total_Rows * 3) / 4;
  double q1 = sort_Data[median1];
  double q3 = sort_Data[median2];
  double iqr = q3 - q1;
  lower_Bound[current_Col] = q1 - (1.5 * iqr);
  upper_Bound[current_Col] = q3 + (1.5 * iqr);
  store_Outlier_Rows(current_Col, total_Rows, lower_Bound[current_Col], upper_Bound[current_Col], data, outlier_Row_Idx);
}

void check_Outlier(std::vector < std::string > * data,
  bool * sparse_Idx,
  int remaining_Cols,
  int total_Rows,
  double * upper_Bound,
  double * lower_Bound,
  std::set < int > & outlier_Row_Idx) {
  for (int i = 0; i < remaining_Cols; i++) {
    if (sparse_Idx[i] != 1)
      outlier_Detection(data, i, total_Rows, upper_Bound, lower_Bound, outlier_Row_Idx);
  }
}


int find_number_of_Sparse(bool * sparse_Idx, int remaining_Cols) {
  int count = 0;
  for (int i = 0; i < remaining_Cols; i++) {
    if (sparse_Idx[i] == 1)
      count++;
  }
  return count;
}

int convert_Sparse_Cols(std::vector < std::string > * data,
  int remaining_Cols, int total_Rows,
  bool * sparse_Idx,
  std::vector < int > * sparse_Matrix_Guide) {
  int count = 0;
  int sparse_Editing_Idx = 0;
  std::multimap < std::string, int > mapper;
  for (int j = 0; j < remaining_Cols; j++) {
    int idx = 0;
    if (sparse_Idx[j] == 1) {
      mapper.clear();
      //sparse_Matrix_Guide.clear();
      std::string line;
      for (int i = 0; i < total_Rows; i++) {
        line = data[j][i];
        if (mapper.find(line) == mapper.end()) {
          int val = idx + sparse_Editing_Idx;
          mapper.insert(std::make_pair(line, val));
          idx++;
        }
        sparse_Matrix_Guide[count].push_back(mapper.find(line) -> second);
      }
      sparse_Editing_Idx = sparse_Editing_Idx + idx;
      count++;
    }
  }
  return sparse_Editing_Idx;
}

int store_To_Final_File(std::vector < std::string > * data,
  int remaining_Cols, int total_Rows,
  std::set < int > & outlier_Row_Index,
  std::vector < int > * sparse_Matrix_Guide,
  int no_Of_Sparse,
  bool * sparse_Idx,
  int total_New_Columns) {
  int remaining_Rows = 0;
  std::ofstream writer;
  writer.open("Module1.csv");
  std::set < int > ::iterator itr = outlier_Row_Index.begin();
  for (int i = 0, idx = 0; i < total_Rows; i++) {
   // std::cout<<*itr<<" ";
    bool fill_started = 0;
    if ( * itr != i||total_New_Columns==0) {
      for (int j = 0; j < remaining_Cols - 1; j++) {
        if (sparse_Idx[j] != 1) {
          if (fill_started)
            writer << "," << data[j][i];
          else {
            writer << data[j][i];
            fill_started = 1;
          }
        }
      }
      for (int j = 0, idx = 0; j < total_New_Columns; j++) {
        if (sparse_Matrix_Guide[idx][i] == j) {
          writer << ",1";
          idx++;
          if (idx > no_Of_Sparse)
            idx = no_Of_Sparse - 1;
        } else
          writer << ",0";
      }
      if (sparse_Idx[remaining_Cols - 1] != 1) {
        writer << "," << data[remaining_Cols - 1][i] << "\n";
      } else {
        writer << "\n";
      }
      remaining_Rows++;
    } else
      itr++;
  }
  writer.close();
  return remaining_Rows;
}


int main() {
  //MODULE 1
  std::string file_Name = get_Csv_File_Name();
  store_To_File(file_Name);
  int total_Rows=get_Total_Rows(file_Name);
  int total_Cols=get_Total_Cols(file_Name);
  show_Labels(file_Name,total_Cols);
  int target_Col=set_Target_Col(total_Cols);
  std::set <int> remove_Col_Idx;
  int removed_Cols=remove_Cols(remove_Col_Idx,target_Col,total_Cols);
  int remaining_Cols=total_Cols-removed_Cols;
  int empty_Values_Per_Col[remaining_Cols+1];
  std::vector < std::string > data[remaining_Cols + 1];
  memset(empty_Values_Per_Col,0,sizeof(empty_Values_Per_Col));
  store_To_Vector(file_Name, remove_Col_Idx, data, total_Cols, target_Col, remaining_Cols, empty_Values_Per_Col);
  int limit = (total_Rows * 75) / 100;
  store_Vector_To_File(data, total_Rows, remaining_Cols, limit, empty_Values_Per_Col);
  //MODULE 1 END
  int col_Data_Type[remaining_Cols + 1];
  memset(col_Data_Type, 0, sizeof(col_Data_Type)); //initialize array to zero
  remaining_Cols = find_Data_Type("Module1.csv", col_Data_Type);
  std::string mode[remaining_Cols];
  find_Mode(data, mode, total_Rows, remaining_Cols);
  double mean[remaining_Cols];
  std::string min[remaining_Cols], max[remaining_Cols];
  find_Mean(data, col_Data_Type, mean, total_Rows, remaining_Cols, max, min);
  fill_Missing_Values(mean, mode, data, remaining_Cols, total_Rows, col_Data_Type);
  memset(empty_Values_Per_Col, 0, sizeof(empty_Values_Per_Col)); //initialize array to zero
  store_Vector_To_File(data, total_Rows, remaining_Cols, limit, empty_Values_Per_Col);
  display_File();
  bool sparse_Idx[remaining_Cols];
  memset(sparse_Idx, 0, sizeof(sparse_Idx));
  check_Nominal_Ordinal(data, col_Data_Type, total_Rows, remaining_Cols, sparse_Idx);
  min_Max_Normalization(min, max, data, col_Data_Type, remaining_Cols, total_Rows);
  std::cout << "Convertion of Ordinal Strings: COMPLETE\n";
  std::cout << "MIN MAX Normalization: COMPLETE\n";
  store_Vector_To_File(data, total_Rows, remaining_Cols, limit, empty_Values_Per_Col);
  display_File();
  double lower_Bound[remaining_Cols];
  double upper_Bound[remaining_Cols];
  std::set < int > outlier_Row_Idx;
  check_Outlier(data, sparse_Idx, remaining_Cols, total_Rows, upper_Bound, lower_Bound, outlier_Row_Idx);
  std::cout << "Number of Outliers Rows removed: "<<outlier_Row_Idx.size()<<"\n";
  std::cout << "Convertion of Sparse Matrix: COMPLETE\n";
  int no_Of_Sparse = find_number_of_Sparse(sparse_Idx, remaining_Cols);
  std::vector < int > sparse_Matrix_Guide[no_Of_Sparse];
  int total_New_Columns = convert_Sparse_Cols(data, remaining_Cols, total_Rows, sparse_Idx, sparse_Matrix_Guide);
  int remaining_Rows = store_To_Final_File(data, remaining_Cols, total_Rows,
    outlier_Row_Idx, sparse_Matrix_Guide, no_Of_Sparse,
    sparse_Idx, total_New_Columns);
  display_File();
  return 0;
}

/*

  //MODULE 8 END
  //MODULE 9
  create_Test_Train_Split(remaining_Rows);

  system("./machinelearning");
  return 0;
}
*/