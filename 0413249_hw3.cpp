#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <limits>
#include <map>
#include <regex>

using days = int;

int hash_(int *arr, int n)
{
	std::string hash_string;
	int value;
	std::stringstream sstm;
	std::string result;

	for (int i = 0; i < n; i++)
	{
		sstm << hash_string << arr[i];
	}
	result = sstm.str();
	value = std::stoi(result);
	return value;
}

static std::map<int, int> cache;

int max_score(days study_days, int **table, int *arr, int n)
{
	int hash_value = hash_(arr, n);
	// check this combination have been calculated
	if (auto iter = cache.find(hash_value); iter != cache.end())
		return iter->second;

	if (study_days == 0)
	{
		int score = 0;
		for (int i = 0; i < n; i++)
		{
			score = score + table[i][arr[i]];
		}
		//std::cout << score << "\n";
		cache.insert({ hash_value, score });
		return score;
	}
	else
	{
		int best = std::numeric_limits<int>::min();

		for (int i = 0; i < n; i++)
		{
			arr[i] = arr[i] + 1;
			best = std::max(best, max_score(study_days - 1, table, arr, n));
			arr[i] = arr[i] - 1;
		}
		//std::cout << best << "\n";
		cache.insert({ hash_value, best });
		return best;
	}
}


int getline_num(std::ifstream &f)
{
	std::string line;
	std::getline(f, line);
	return std::stoi(line);
}

void parse(std::ifstream &f)
{
	std::ofstream out("output.txt");
	int row = 0;
	int column = 0;
	std::string whole_line;
	std::stringstream ss(whole_line);
	std::vector<int> table_number_list;

	std::string entry_line;

	int table_col = 0;
	int **table;

	while (std::getline(f, entry_line))
	{
		std::regex is_table_regex{ "\\s+" };
		if (std::regex_search(entry_line, is_table_regex))
		{
			for (int i = 0; i < column; i++)
			{
				table[i] = new int[row + 1];
				for (int j = 0; j < row + 1; j++)
				{
					table[i][j] = 0;
				}
			}
			/*if (entry_line == "")
			   break;*/
			std::stringstream ss(entry_line);
			table_col = 0;
			for (std::string entry; ss >> entry;)
			{
				table_number_list.push_back(std::stoi(entry));
				table_col++;
			}

			//get row and column of 
			row = table_number_list.size() / table_col;
			column = table_col;

			/////////////////////////////////////
			//convert the table_number_list to 2 diminsion
			//initilze all to 0
			int** a = new int*[row];
			for (int i = 0; i < row; i++)
			{
				a[i] = new int[column];
				for (int j = 0; j < column; j++)
				{
					a[i][j] = 0;
				}
			}
			//get table number
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < column; j++)
				{
					a[i][j] = table_number_list[i*column + j];
				}
			}
			//////////////////////////
			//then rotate the table 90 degree
			//initilze all to 0
			// ==> column * row
			int** rotate_a = new int*[column];
			for (int i = 0; i < column; i++)
			{
				rotate_a[i] = new int[row];
				for (int j = 0; j < row; j++)
				{
					rotate_a[i][j] = 0;
				}
			}

			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < column; j++)
				{
					rotate_a[j][i] = a[i][j];
				}
			}

			///////////////////////////////
			//then let the first column in rotate table be 0
			//now table is  ==> column * (row+1)
			//initilze all to 0
			table = new int*[column];
			for (int i = 0; i < column; i++)
			{
				table[i] = new int[row + 1];
				for (int j = 0; j < row + 1; j++)
				{
					table[i][j] = 0;
				}
			}

			for (int i = 0; i < column; i++)
			{
				for (int j = 0; j < row + 1; j++)
				{
					if (j == 0)
					{
						table[i][j] == 0;
					}
					else
					{
						table[i][j] = rotate_a[i][j - 1];
					}
				}
			}


		}
		else if (entry_line == "")
		{

		}
		else
		{
			int resource = std::stoi(entry_line);
			std::cout << "resource" << resource << std::endl;

			/////////////////////////////
			//coures1 course2 course3 ...........
			//now total number = column (has been rotated)
			int studydays = 0;
			int *course_arr;
			course_arr = new int[column];
			//already study for 1 day
			for (int j = 0; j < column; j++)
			{
				course_arr[j] = 1;
			}

			studydays = resource;
			out << max_score(studydays - column /* total 7 days - each subject must study at least 1 day * 4 subject */,
				table,
				course_arr, column) << std::endl;
			cache.clear();
			delete[] course_arr;

			/*for(int i=0; i<column; i++)
			{
				delete [] table[i];
			}
			delete [] table;*/
		}


	}
	//std::cout << "table column " << table_col << "\n";
	/*for(int i = 0;i < table_number_list.size(); i++)
	{
		std::cout << table_number_list[i];
	}
	std::cout << "\n";*/

	/*int table_col = 0;
	for (std::string entry_line; std::getline(f, entry_line);)
	{
		// read one empty line
		if (entry_line == "")
			break;
		std::stringstream ss(entry_line);
		table_col = 0;
		for (std::string entry; ss >> entry; table_col++)
			table_number_list.push_back(std::stoi(entry));
	}*/
	//std::cout << "table column " << table_col << "\n";
	/*for(int i = 0;i < table_number_list.size(); i++)
	{
		std::cout << table_number_list[i];
	}
	std::cout << "\n";*/
	//get row and column of 


	/*int resource_num;
	std::vector<int> resource_num_arr;
	while(f >> resource_num)
	{
		resource_num_arr.push_back(resource_num);
	}
	for(int i = 0;i < resource_num_arr.size(); i++)
	{
		std::cout << resource_num_arr[i];
		std::cout << "\n";
	} 				*/

}


int main()
{
	std::ifstream input("input.txt");
	parse(input);
}


