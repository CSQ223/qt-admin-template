#ifndef xlsx_handler_H
#define xlsx_handler_H

#include <QMap>
#include "common.h"
#include "xlsxdocument.h"
#include "xlsxabstractsheet.h"

QXLSX_USE_NAMESPACE





class xlsx_handler
{
public:
    struct Takeover_Data_Elem_st
    {
        QString seqnum;
        QString vehiclename;
        QString sliphandtime;
        QString position;
        QString slipusetime;
        QString vehiclestatus;
        QString weather_str;
        QString weather_value;
        QString env_str;
        QString env_value;
        QString reason;
        QString day_str;
    };

    struct Total_Data_Elem_st
    {
        double test_min;
        double auto_min;
        double total_mileage;
        double auto_mileage;
        int sliphand_num;
        double test_hour;
        double auto_hour;
    };

    struct TakeoverTotal_Data_Elem_st
    {
        double total_mileage;
        double auto_mileage;
        double test_hour;
        double auto_hour;
    };

    struct VehicleTotal_Data_Elem_st
    {
        double total_mileage;
        double auto_mileage;
        double test_hour;
        double auto_hour;
        int sliphand_num;
        int sliphand_num_env;
        int sliphand_num_road;
    };

    xlsx_handler();
    ~xlsx_handler();
    void write_vehicle_total_sheet_header(const std::vector<QString> &dataelem);
    void write_vehicle_total_sheet_data(const std::vector<Total_Data_Elem_st> &totalelem);
    bool write_vehicle_total_sheet(const std::map<std::string, bool> &date_map, const std::string &vehicle_name, const std::map<std::string, Vehicle_DataInfo_st> &vehicle_datainfo);

    void write_vehicle_takeover_sheet_header();
    void write_vehicle_takeover_weather_data_sheet();
    void write_vehicle_takeover_sheet_data(const int index, const Takeover_Data_Elem_st &vehicle_dataelem);
    bool write_vehicle_takeover_sheet(const std::string &vehicle_name, const std::map<std::string, Vehicle_DataInfo_st> &vehicle_data_map);
    bool write_vehicle_takeover_total_sheet(const std::map<std::string, bool> &date_map, const std::map<std::string, std::map<std::string, Vehicle_DataInfo_st>> &vehicle_map_info);

    void write_vehicle_takeover_total_vehicle_data(const int row, const int col, const TakeoverTotal_Data_Elem_st &data_elem);
    void write_vehicle_takeover_total_vehicle_data_sheet(const int row, const std::string vehicle_name, const std::map<std::string, Vehicle_DataInfo_st> vehicle_data_map, const std::map<std::string, bool> vehicle_date_map);

    void write_total_header(const std::vector<std::string> &vehicle_vec);
    void write_total_data(const std::map<std::string, VehicleTotal_Data_Elem_st> &vehicle_total_map);
    bool write_total_sheet(const std::map<std::string, std::map<std::string, Vehicle_DataInfo_st>> &vehicle_info_map);

    bool export_to_excel(const std::map<std::string, std::string> &weather_map, const std::map<std::string, std::map<std::string, Vehicle_DataInfo_st>> &vehicle_map_info);

    bool save(const std::string &filepathname);
private:
    Document xlsx;
    std::map<std::string, std::string> vehicle_name_key_map;
    std::map<std::string, std::string> weather_map;
};

#endif // xlsx_handler_H
