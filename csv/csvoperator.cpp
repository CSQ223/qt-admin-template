/*
 *Copyright (C) 2020 Tusimple.  All rights reserved. 
*/
#include <iostream>
#include <fstream>  
#include <sstream>

#include "csvoperator.h"


//CSV operator
CSVOperator::CSVOperator(std::string filename) : filename_(filename)
{
    
}

bool CSVOperator::load(std::string fullfilename)
{
    uint32_t key = 0;
    std::string lineStr;
    std::ifstream inFile(fullfilename, std::ios::in);

    if (!inFile.is_open())
    {
        std::cerr << "open " << fullfilename << " error!" << std::endl;
        return false;
    }

    while (getline(inFile, lineStr))
    {
        int index = 0;
        std::string value;
        std::stringstream linestrstream(lineStr);
        
        std::map<uint32_t, std::string> valuemap;

        while (getline(linestrstream, value, ','))
        {
            valuemap[index + 1] = value;
            index++;
        }
        data_map_[key++] = valuemap;
    }
    inFile.close();
    filename_ = fullfilename;
    return true;
}

bool CSVOperator::load_from_StdString(const std::string &str)
{
    uint32_t key = 0;
    std::string lineStr;

    std::stringstream strstream(str);
    while (getline(strstream, lineStr, '\n'))
    {
        int index = 0;
        std::string value;
        std::stringstream linestrstream(lineStr);

        std::map<uint32_t, std::string> valuemap;

        while (getline(linestrstream, value, ','))
        {
            valuemap[index + 1] = value;
            index++;
        }
        data_map_[key++] = valuemap;
    }

    return true;
}



bool CSVOperator::getInt(uint32_t column, uint32_t row, int &value)
{
    std::string str;
    if (getString(column, row, str))
    {
        value = atoi(str.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

bool CSVOperator::getFloat(uint32_t column, uint32_t row, double &value)
{
    std::string str;
    if (getString(column, row, str))
    {
        value = atof(str.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

bool CSVOperator::getString(uint32_t column, uint32_t row, std::string &value)
{
    std::map<uint32_t, std::map<uint32_t, std::string>>::iterator iterLine = data_map_.find(column);
    if (iterLine != data_map_.end())
    {
        std::map<uint32_t, std::string> &rStringMap = iterLine->second;
        std::map<uint32_t, std::string>::iterator iterRow = rStringMap.find(row);
        if (iterRow != rStringMap.end())
        {
            value = iterRow->second;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool CSVOperator::setNumber(uint32_t column, uint32_t row, int value)
{
    std::string str;
    if (getString(column, row, str))
    {
        char buffer[100];
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%d", value);
        str.clear();
        str = buffer;
        return true;
    }
    else
    {
        data_map_[column].insert(std::make_pair(row, std::to_string(value)));
        return true;
    }
}

bool CSVOperator::setNumber(uint32_t column, uint32_t row, float value)
{
    std::string str;
    if (getString(column, row, str))
    {
        char buffer[100];
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%f", value);
        str.clear();
        str = buffer;
        return true;
    }
    else
    {
        data_map_[column].insert(std::make_pair(row, std::to_string(value)));
        return true;
    }
}

bool CSVOperator::setString(uint32_t column, uint32_t row, std::string value)
{
    std::string str;
    if (getString(column, row, str))
    {
        str.clear();
        str = value;
        return true;
    }
    else
    {
        data_map_[column].insert(std::make_pair(row, value));
        return false;
    }
}

bool CSVOperator::save(std::string fullfilename)
{
    if (!fullfilename.empty())
    {
        filename_ = fullfilename;
    }

    std::ofstream outFile;
    outFile.open(filename_, std::ios::out);

    std::cout << "save to csv file:" << fullfilename <<std::endl;
    
    if (!outFile.is_open())
    {
        std::cerr << "open " + filename_ + " error!" <<std::endl;
        return false;
    }

    for (auto iter : data_map_)
    {
        bool begin = true;
        std::map<uint32_t, std::string> &value_map = iter.second;

        for (auto iter1 : value_map)
        {
            if (begin)
            {
                outFile << iter1.second;
                begin = false;
            } else
            {
                outFile << ',' << iter1.second;
            }
        }
        outFile << std::endl;
    }
    outFile.close();
    data_map_.clear();
    return true;
}

