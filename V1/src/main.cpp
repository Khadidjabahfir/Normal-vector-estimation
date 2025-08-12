#include <iostream>
#include <string>
#include "loading_files.cpp"

vector<vector<float>> ReadFromWebInput()
{
    string input;
    std::getline(std::cin, input);

    vector<vector<float>> data;
    vector<float> row;
    string num;
    bool insideNumber = false;

    for (char c : input)
    {
        if ((c >= '0' && c <= '9') || c == '.' || c == '-' || c == 'e')
        {
            num += c;
            insideNumber = true;
        }
        else
        {
            if (insideNumber)
            {
                row.push_back(std::stof(num));
                num.clear();
                insideNumber = false;
            }
            if (c == ']')
            {
                if (!row.empty())
                {
                    data.push_back(row);
                    row.clear();
                }
            }
        }
    }

    return data;
}
int main()
{
    vector<vector<float>> data = ReadFromWebInput();
    if (data.empty())
    {
        std::cerr << "No data provided." << std::endl;
        return 1;
    }
}