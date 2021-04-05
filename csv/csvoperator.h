/*
 *Copyright (C) 2020 Tusimple.  All rights reserved. 
 */
#pragma once
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <list>
#include <string.h>
#include <stdint.h>


class CSVOperator
{

public:
    CSVOperator(){};
    ~CSVOperator(){};
    CSVOperator(std::string filename);

    bool load(std::string filename);
    bool save(std::string filename);


    bool getInt(uint32_t column, uint32_t row, int &value);
    bool getFloat(uint32_t column, uint32_t row, double &value);
    bool getString(uint32_t column, uint32_t row, std::string &value_compare);
    bool setNumber(uint32_t column, uint32_t row, int value);
    bool setNumber(uint32_t column, uint32_t row, float value);
    bool setString(uint32_t column, uint32_t row, std::string value);

    void emptyCSVData() {data_map_.clear();};
    std::map<uint32_t, std::map<uint32_t, std::string>> &getCSVMap() { return data_map_; }

    bool load_from_StdString(const std::string &str);

protected:
    std::string filename_;
    std::map<uint32_t, std::map<uint32_t, std::string>> data_map_;
};


