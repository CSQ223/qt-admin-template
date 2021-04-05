#include <iostream>
#include <QtGlobal>
#include "csv_handler.h"


inline void tm2unix(struct tm tm_time, unix_date_time &unix_time)
{
    if (tm_time.tm_year < 0)
    {
        unix_time.year = 0;
        unix_time.mon = 0;
    } else {
        unix_time.year = static_cast<uint16_t>(tm_time.tm_year + 1900);
        unix_time.mon = static_cast<uint8_t>(tm_time.tm_mon + 1);
    }

    unix_time.day = static_cast<uint8_t>(tm_time.tm_mday);
    unix_time.hour = static_cast<uint8_t>(tm_time.tm_hour);
    unix_time.min = static_cast<uint8_t>(tm_time.tm_min);
    unix_time.sec = static_cast<uint8_t>(tm_time.tm_sec);
}

csv_handler::csv_handler()
{

}

csv_handler::csv_handler(const std::string str)
{
    csv_.load_from_StdString(str);
}

csv_handler::~csv_handler()
{

}

void csv_handler::date_to_string(unix_date_time tmdate, std::string &strdate, DateStringType_en type)
{
    char databuf[30] = {0};
    switch (type)
    {
    // 2020/06/08
    case DateStringType_en::DateStringType_DAY:
        // strdate = std::to_string(tmdate.year) + '/' + std::to_string(tmdate.mon) + '/' + std::to_string(tmdate.day);
        sprintf(databuf, "%04d/%02d/%02d", tmdate.year, tmdate.mon, tmdate.day);
        strdate = databuf;
        break;
    // 2020-06-08-21
    case DateStringType_en::DateStringType_HOUR:
        strdate = std::to_string(tmdate.year) + '-' + std::to_string(tmdate.mon) + '-' + std::to_string(tmdate.day) + '-' + std::to_string(tmdate.hour);
        break;
    // 2020.6.8 21:03
    case DateStringType_en::DateStringType_MIN:
        sprintf(databuf, "%d.%d.%d %02d:%02d", tmdate.year, tmdate.mon, tmdate.day, tmdate.hour, tmdate.min);
        strdate = databuf;
        // strdate = std::to_string(tmdate.year) + '.' + std::to_string(tmdate.mon) + '.' + std::to_string(tmdate.day) + ' ' + std::to_string(tmdate.hour) + ':' + std::to_string(tmdate.min);
        break;
    // 2020-06-08-21-30-30
    case DateStringType_en::DateStringType_SECOND:
        sprintf(databuf, "%04d-%02d-%02d-%02d-%02d-%02d", tmdate.year, tmdate.mon, tmdate.day, tmdate.hour, tmdate.min, tmdate.sec);
        strdate = databuf;
        // strdate = std::to_string(tmdate.year) + '-' + std::to_string(tmdate.mon) + '-' + std::to_string(tmdate.day) + '-' + std::to_string(tmdate.hour) + '-' + std::to_string(tmdate.min) + '-' + std::to_string(tmdate.sec);
        break;
    }
}

void csv_handler::string_to_date(std::string strdate, unix_date_time &tmdate, DateStringType_en type)
{
    struct tm t = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    switch (type)
    {
    case DateStringType_en::DateStringType_DAY:
#ifdef Q_OS_LINUX
        strptime(strdate.c_str(), "%Y/%m/%d", &t);
#endif

#ifdef Q_OS_WIN32
        sscanf(strdate.c_str(), "%d/%d/%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
        t.tm_year -= 1900;
        t.tm_mon -= 1;
#endif

        tm2unix(t, tmdate);
        break;
    case DateStringType_en::DateStringType_HOUR:
#ifdef Q_OS_LINUX
        strptime(strdate.c_str(), "%Y-%m-%d-%H", &t);
#endif

#ifdef Q_OS_WIN32
        sscanf(strdate.c_str(), "%d-%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour);
        t.tm_year -= 1900;
        t.tm_mon -= 1;
#endif
        tm2unix(t, tmdate);
        break;
    case DateStringType_en::DateStringType_MIN:
#ifdef Q_OS_LINUX
        strptime(strdate.c_str(), "%Y.%m.%d %H:%M", &t);
#endif

#ifdef Q_OS_WIN32
        sscanf(strdate.c_str(), "%d.%d.%d %d:%d", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min);
        t.tm_year -= 1900;
        t.tm_mon -= 1;
#endif
        tm2unix(t, tmdate);
        break;
    case DateStringType_en::DateStringType_SECOND:
#ifdef Q_OS_LINUX
        strptime(strdate.c_str(), "%Y-%m-%d-%H-%M-%S", &t);
#endif

#ifdef Q_OS_WIN32
        sscanf(strdate.c_str(), "%d-%d-%d-%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec);
        t.tm_year -= 1900;
        t.tm_mon -= 1;
#endif
        tm2unix(t, tmdate);
        break;
    }
}

void csv_handler::hex_to_string(uint8_t *data_ptr, uint16_t data_len, std::string &str)
{
    // char *buffer_ptr = new char[data_len * 3];
    for (int i = 0; i < data_len; i++)
    {
        char temp[4] = {0};
        sprintf(temp, "%02X ", data_ptr[i]);
        str += temp;
    }

    // delete []buffer_ptr;
}

void csv_handler::convert_from_csv(const std::string &str_content, Vehicle_DataInfos_st &datainfo)
{
    std::string str, value;

    memset(&datainfo, 0x00, sizeof(datainfo));

    csv_.load_from_StdString(str_content);


    if (csv_.getString(0, 1, str) && csv_.getString(0, 2, value))
    {
        if (str.find("数据来源") != std::string::npos)
        {
            if (value.find("公安") != std::string::npos)
            {
                datainfo.ga_data.demand = "公安";
            }
        }
    }

    if (csv_.getString(0, 4, str) && csv_.getString(0, 5, value))
    {
        if (str.find("数据来源") != std::string::npos)
        {
            if (value.find("淞泓") != std::string::npos)
            {
                datainfo.sh_data.demand = "淞泓";
            }
        }
    }

    // column: 2
    if (csv_.getString(1, 1, str) && csv_.getString(1, 2, value))
    {
        if (str.find("vehicle_id") != std::string::npos)
        {
            datainfo.ga_data.vehicle_id = value;
        }
    }

    if (csv_.getString(1, 4, str) && csv_.getString(1, 5, value))
    {
        if (str.find("vehicle_id") != std::string::npos)
        {
            datainfo.sh_data.vehicle_id = value;
        }
    }

    // column: 3
    if (csv_.getString(2, 1, str) && csv_.getString(2, 2, value))
    {
        if (str.find("date") != std::string::npos)
        {
            string_to_date(value, datainfo.ga_data.vehicle_date, DateStringType_en::DateStringType_DAY);
        }
    }

    if (csv_.getString(2, 4, str) && csv_.getString(2, 5, value))
    {
        if (str.find("date") != std::string::npos)
        {
            string_to_date(value, datainfo.sh_data.vehicle_date, DateStringType_en::DateStringType_DAY);
        }
    }

    // column: 4
    if (csv_.getString(3, 1, str) && csv_.getString(3, 2, value))
    {
        if (str.find("starttime") != std::string::npos)
        {
            string_to_date(value, datainfo.ga_data.vehicle_starttime, DateStringType_en::DateStringType_SECOND);
        }
    }

    if (csv_.getString(3, 4, str) && csv_.getString(3, 5, value))
    {
        if (str.find("starttime") != std::string::npos)
        {
            // datainfo.ga_data.date_ = 0;
            string_to_date(value, datainfo.sh_data.vehicle_starttime, DateStringType_en::DateStringType_SECOND);

        }
    }

    // column: 5
    if (csv_.getString(4, 1, str) && csv_.getString(4, 2, value))
    {
        if (str.find("endtime") != std::string::npos)
        {
            string_to_date(value, datainfo.ga_data.vehicle_endtime, DateStringType_en::DateStringType_SECOND);
        }
    }

    if (csv_.getString(4, 4, str) && csv_.getString(4, 5, value))
    {
        if (str.find("endtime") != std::string::npos)
        {
            string_to_date(value, datainfo.sh_data.vehicle_endtime, DateStringType_en::DateStringType_SECOND);
        }
    }

    // column: 6
    double value_min = 0.0;
    if (csv_.getString(5, 1, str) && csv_.getFloat(5, 2, value_min))
    {
        if (str.find("test_min") != std::string::npos)
        {
            datainfo.ga_data.vehicle_test_min = value_min;
        }
    }

    value_min = 0.0;
    if (csv_.getString(5, 4, str) && csv_.getFloat(5, 5, value_min))
    {
        if (str.find("test_min") != std::string::npos)
        {
            datainfo.sh_data.vehicle_test_min = value_min;
        }
    }

    //column: 7
    value_min = 0.0;
    if (csv_.getString(6, 1, str) && csv_.getFloat(6, 2, value_min))
    {
        if (str.find("auto_min") != std::string::npos)
        {
            datainfo.ga_data.vehicle_auto_min = value_min;
        }
    }

    value_min = 0.0;
    if (csv_.getString(6, 4, str) && csv_.getFloat(6, 5, value_min))
    {
        if (str.find("auto_min") != std::string::npos)
        {
            datainfo.sh_data.vehicle_auto_min = value_min;
        }
    }

    // column: 8
    double value_kmeter = 0.0;
    if (csv_.getString(7, 1, str) && csv_.getFloat(7, 2, value_kmeter))
    {
        if (str.find("total_mileage(km)") != std::string::npos)
        {
            datainfo.ga_data.vehicle_total_mileage = value_kmeter;
        }
    }

    value_kmeter = 0.0;
    if (csv_.getString(7, 4, str) && csv_.getFloat(7, 5, value_kmeter))
    {
        if (str.find("total_mileage(km)") != std::string::npos)
        {
            datainfo.sh_data.vehicle_total_mileage = value_kmeter;
        }
    }

    // column: 9
    value_kmeter = 0.0;
    if (csv_.getString(8, 1, str) && csv_.getFloat(8, 2, value_kmeter))
    {
        if (str.find("auto_mileage(km)") != std::string::npos)
        {
            datainfo.ga_data.vehicle_auto_mileage = value_kmeter;
        }
    }

    value_kmeter = 0.0;
    if (csv_.getString(8, 4, str) && csv_.getFloat(8, 5, value_kmeter))
    {
        if (str.find("auto_mileage(km)") != std::string::npos)
        {
            datainfo.sh_data.vehicle_auto_mileage = value_kmeter;
        }
    }

    // column: 10
    int songhong_sliphand_count = 0;
    int gongan_sliphand_count = 0;
    if (csv_.getString(9, 1, str) && csv_.getInt(9, 2, songhong_sliphand_count))
    {
        if (str.find("sliphand_num") != std::string::npos)
        {
            for (auto index = 0; index < songhong_sliphand_count; index++)
            {
                std::string auto_begin_key, auto_begin_value;
                std::string auto_end_key, auto_end_value;
                std::string auto_distance_key;
                double auto_distance_value;
                if (csv_.getString(static_cast<uint32_t>(10 + index * 3), 1, auto_begin_key) && \
                    csv_.getString(static_cast<uint32_t>(10 + index * 3), 2, auto_begin_value) && \
                    csv_.getString(static_cast<uint32_t>(11 + index * 3), 1, auto_end_key) && \
                    csv_.getString(static_cast<uint32_t>(11 + index * 3), 2, auto_end_value) && \
                    csv_.getString(static_cast<uint32_t>(12 + index * 3), 1, auto_distance_key) &&  \
                    csv_.getFloat(static_cast<uint32_t>(12 + index * 3), 2, auto_distance_value))
                {
                    Sliphand_Data_st status ={{0, 0, 0 ,0 ,0 ,0 ,0 }, {0, 0, 0 ,0 ,0 ,0 ,0 }, 0};
                    string_to_date(auto_begin_value, status.sliphand_begin_time, DateStringType_en::DateStringType_SECOND);
                    string_to_date(auto_end_value, status.sliphand_end_time, DateStringType_en::DateStringType_SECOND);
                    status.sliphand_distance = auto_distance_value;
                    datainfo.ga_data.vehicle_sliphands.push_back(status);
                }
            }
        }
    }

    if (csv_.getString(9, 4, str) && csv_.getInt(9, 5, gongan_sliphand_count))
    {
        if (str.find("sliphand_num") != std::string::npos)
        {
            for (auto index = 0; index < gongan_sliphand_count; index++)
            {
                std::string auto_begin_key, auto_begin_value;
                std::string auto_end_key, auto_end_value;
                std::string auto_distance_key;
                double auto_distance_value;
                if (csv_.getString(static_cast<uint32_t>(10 + index * 3), 4, auto_begin_key) && \
                    csv_.getString(static_cast<uint32_t>(10 + index * 3), 5, auto_begin_value) && \
                    csv_.getString(static_cast<uint32_t>(11 + index * 3), 4, auto_end_key) && \
                    csv_.getString(static_cast<uint32_t>(11 + index * 3), 5, auto_end_value) && \
                    csv_.getString(static_cast<uint32_t>(12 + index * 3), 4, auto_distance_key) && \
                    csv_.getFloat(static_cast<uint32_t>(12 + index * 3), 5, auto_distance_value))
                {
                    Sliphand_Data_st status ={{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, 0};

                    string_to_date(auto_begin_value, status.sliphand_begin_time, DateStringType_en::DateStringType_SECOND);
                    string_to_date(auto_end_value, status.sliphand_end_time, DateStringType_en::DateStringType_SECOND);
                    status.sliphand_distance = auto_distance_value;
                    datainfo.sh_data.vehicle_sliphands.push_back(status);
                }
            }
        }
    }
}
