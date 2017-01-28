#ifndef SPLIT_H
#define SPLIT_H

#include<string>
#include<vector>

void split_s(std::string &str, std::vector<std::string> &result, char delim)
{
    std::string::size_type i, j, len = str.size();
    for (i = j = 0; i < len;)
    {
        
        while (i < len&&::isspace(str[i]))
            i++;
        j = i;
    
        while (i < len&&!::isspace(str[i]))
            i++;
        if (j < i)
        {
            result.push_back(str.substr(j, i - j));
            while (i < len&&str[i] == delim)
                i++;
            j = i;
        }
    }
    if (j < len)
    {
        result.push_back(str.substr(j, len - j));
    }
}

void split_i(std::string &str, std::vector<int> &result, char delim)
{
    std::string::size_type i, j, len = str.size();
    for (i = j = 0; i < len;)
    {
        
        while (i < len&&::isspace(str[i]))
            i++;
        j = i;
    
        while (i < len&&!::isspace(str[i]))
            i++;
        if (j < i)
        {
            result.push_back(stoi(str.substr(j, i - j)));
            while (i < len&&str[i] == delim)
                i++;
            j = i;
        }
    }
    if (j < len)
    {
        result.push_back(stoi(str.substr(j, len - j)));
    }
}

#endif