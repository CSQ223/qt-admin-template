#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include <string>
#include <vector>
#include "csvoperator.h"
#include "rtmcodec.h"

#include "common.h"

struct Vehicle_DataInfos_st
{
    std::string weather_data;
    Vehicle_DataElem_st sh_data;
    Vehicle_DataElem_st ga_data;
};



class csv_handler
{
public:
    enum class DateStringType_en
    {
        DateStringType_DAY = 0,
        DateStringType_HOUR = 1,
        DateStringType_MIN  = 2,
        DateStringType_SECOND
    };

    csv_handler();
    csv_handler(const std::string str);

    ~csv_handler();

    void convert_from_csv(const std::string &str, Vehicle_DataInfos_st &datainfo);
private:

    CSVOperator csv_;

    void date_to_string(unix_date_time tmdate, std::string &strdate, DateStringType_en type);
    void string_to_date(std::string strdate, unix_date_time &tmdate, DateStringType_en type);
    void hex_to_string(uint8_t *data_ptr, uint16_t data_len, std::string &str);
};

#endif // CSV_HANDLER_H
