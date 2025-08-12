#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using std::getline;
using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;

vector<vector<float>> LoadXYZFile(const string &filename)
{
    ifstream file(filename);
    vector<vector<float>> data;

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return data;
    }

    string line;
    while (getline(file, line))
    {
        vector<float> row;
        stringstream ss(line);
        float value;
        while (ss >> value)
        {
            row.push_back(value);
        }

        if (!row.empty())
        {
            data.push_back(row);
        }
    }

    return data;
}
vector<vector<float>> LoadTXTFile(const string &filename)
{
    ifstream file(filename);
    vector<vector<float>> data;

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return data;
    }

    string line;
    int i = 0;
    vector<float> row;
    while (getline(file, line))
    {

        stringstream ss(line);
        float value;
        while (ss >> value)
        {
            row.push_back(value);
        }

        if (i == 2)
        {
            data.push_back(row);
            i = 0;
            row.clear();
        }
        else
        {
            i++;
        }
    }

    return data;
}


