#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>
#include <QString>
#include <stdint.h>


enum DateStringType_en
{
    DateStringType_DAY = 0,
    DateStringType_HOUR = 1,
    DateStringType_MIN = 2,
    DateStringType_SECOND = 3
};


struct unix_date_time
{
    uint16_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint32_t msec;
};

struct Sliphand_Data_st
{
    unix_date_time sliphand_begin_time;
    unix_date_time sliphand_end_time;
    double sliphand_distance;
};


struct Vehicle_DataElem_st
{
    std::string demand;
    std::string vehicle_id;
    unix_date_time vehicle_date;
    unix_date_time vehicle_starttime;
    unix_date_time vehicle_endtime;
    double vehicle_test_min;
    double vehicle_auto_min;
    double vehicle_total_mileage;
    double vehicle_auto_mileage;
    std::vector<Sliphand_Data_st> vehicle_sliphands;
};


struct Vehicle_DataInfo_st
{
    std::string weather_data;
    Vehicle_DataElem_st vehicle_data;
};

inline int get_month_days(int year, int month)
{
    int days = 0;

    switch(month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
        {
            days = 31;
            break;
        }
        case 2:
        {
            if(((year%4==0) && (year%100!=0))|| (year%400==0))
            {
                days = 29;
            } else {
                days = 28;
            }
            break;
        }
        case 4:
        case 6:
        case 9:
        case 11:
        {
            days = 30;
            break;
        }
    }

    return days;
}


inline void date_to_string(unix_date_time tmdate, std::string &strdate, DateStringType_en type)
{
    char temp[20] = {0};
    switch (type)
    {
    // 2020/06/08
    case DateStringType_en::DateStringType_DAY:
        strdate = std::to_string(tmdate.year) + '/' + std::to_string(tmdate.mon) + '/' + std::to_string(tmdate.day);
        break;
    // 2020-06-08-21
    case DateStringType_en::DateStringType_HOUR:
        strdate = std::to_string(tmdate.year) + '-' + std::to_string(tmdate.mon) + '-' + std::to_string(tmdate.day) + '-' + std::to_string(tmdate.min);
        break;
    // 2020.06.08 21:30
    case DateStringType_en::DateStringType_MIN:
        sprintf(temp, "%d.%02d.%02d %02d:%02d", tmdate.year, tmdate.mon, tmdate.day, tmdate.hour, tmdate.min);
        strdate = temp;
//        strdate = std::to_string(tmdate.year) + '.' + std::to_string(tmdate.mon) + '.' + std::to_string(tmdate.day) + ' ' + std::to_string(tmdate.hour) + ':' + std::to_string(tmdate.min);
        break;
    // 2020-06-08-21-30-30
    case DateStringType_en::DateStringType_SECOND:
        strdate = std::to_string(tmdate.year) + '-' + std::to_string(tmdate.mon) + '-' + std::to_string(tmdate.day) + '-' + std::to_string(tmdate.hour) + '-' + std::to_string(tmdate.min) + '-' + std::to_string(tmdate.sec);
        break;
    }
}

inline void split_to_string(const std::string str, const std::string delim, std::vector<std::string> &strelems)
{
    strelems.clear();
    size_t pos = 0;
    size_t len = str.length();
    size_t delim_len = delim.length();
    if (delim_len == 0)
    {
        return ;
    }

    while (pos < len)
    {
        size_t find_pos = str.find(delim, pos);
        if (find_pos != std::string::npos)
        {
            strelems.push_back(str.substr(pos, len - pos));
            break;
        }
        strelems.push_back(str.substr(pos, find_pos - pos));
        pos = find_pos + delim_len;
    }
}

inline std::string& replace_str(std::string &str,const std::string &old_value,const std::string &new_value)
{
    while(true)
    {
        std::string::size_type pos(0);
        if((pos=str.find(old_value))!= std::string::npos)
        {
            str.replace(pos,old_value.length(),new_value);
        } else {
            break;
        }
    }

    return str;
}

inline void unix_date_time_to_qstring(const unix_date_time &t, QString &str)
{
    char temp[20] = {0};
    sprintf(temp, "%04d.%02d.%02d %02d:%02d", t.year, t.mon, t.day, t.hour, t.min);
    str = temp;
}

#endif // COMMON_H
