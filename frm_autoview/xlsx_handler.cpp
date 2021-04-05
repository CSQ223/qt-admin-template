#include <math.h>
#include "xlsx_handler.h"




#if 0
static double double_percision_0_2(const double value)
{
    double newvalue;
    newvalue = floor(value * 100.000f + 0.5) / 100.000f;

    return newvalue;
}

static void unique_vehicledate_str(const std::map<std::string, std::map<std::string, Vehicle_DataInfo_st>> &vehicle_info_map, std::map<std::string, bool> &date_map)
{
    for (auto &iter : vehicle_info_map)
    {
        for (auto iter1 : iter.second)
        {
            std::string date_str;

            date_str = iter1.first;
            date_map[date_str] = true;
        }
    }
}
#endif

xlsx_handler::xlsx_handler()
{
    vehicle_name_key_map["Foton-3051"] = "51";
    vehicle_name_key_map["Foton-3052"] = "52";
    vehicle_name_key_map["Foton-3053"] = "53";
    vehicle_name_key_map["Foton-3054"] = "54";
    vehicle_name_key_map["Shaanqi-2012"] = "12";
}

xlsx_handler::~xlsx_handler()
{

}

void xlsx_handler::write_vehicle_total_sheet_header(const std::vector<QString> &dataelem)
{
    char ch = 'B';
    QXlsx::Format format;
    format.setFontColor(QColor(Qt::black));
    format.setFontBold(true);
    format.setFontSize(8);
    format.setHorizontalAlignment(QXlsx::Format::AlignRight);
    //set cloumn width
    xlsx.setColumnWidth(1, 1, 18);

    xlsx.write("A3", "test_min", format);
    xlsx.write("A4", "auto_min", format);
    xlsx.write("A5", "total_mileage(km)", format);
    xlsx.write("A6", "auto_mileage(km)", format);
    xlsx.write("A7", "sliphand_num", format);
    xlsx.write("A8", "test_hour", format);
    xlsx.write("A9", "auto_hour", format);


    xlsx.setColumnWidth(2, dataelem.size() + 2, 13);
    for (auto iter : dataelem)
    {
        xlsx.write(QString("%1%2").arg(ch).arg(2),  iter, format);
        ch++;
    }

    //xlsx.write(QString("%1%2").arg(ch).arg(2), "Total", format);
}

void xlsx_handler::write_vehicle_total_sheet_data(const std::vector<Total_Data_Elem_st> &totalelem)
{
    char ch = 'B';
    QXlsx::Format format, format1;
    format.setFontColor(QColor(Qt::black));
    format.setFontSize(8);
    format.setNumberFormat("0.00");
    format.setHorizontalAlignment(QXlsx::Format::AlignRight);

    format1.setFontColor(QColor(Qt::black));
    format1.setFontSize(8);
    format1.setHorizontalAlignment(QXlsx::Format::AlignRight);

    for (auto iter : totalelem)
    {
        xlsx.write(QString("%1%2").arg(ch).arg(3), iter.test_min, format);
        xlsx.write(QString("%1%2").arg(ch).arg(4), iter.auto_min, format);
        xlsx.write(QString("%1%2").arg(ch).arg(5), iter.total_mileage, format);
        xlsx.write(QString("%1%2").arg(ch).arg(6), iter.auto_mileage, format);
        xlsx.write(QString("%1%2").arg(ch).arg(7), iter.sliphand_num, format1);
        xlsx.write(QString("%1%2").arg(ch).arg(8), iter.test_hour, format);
        xlsx.write(QString("%1%2").arg(ch).arg(9), iter.auto_hour, format);
        ch++;
    }
}

bool xlsx_handler::write_vehicle_total_sheet(const std::map<std::string, bool> &date_map, const std::string &vehicle_name, const std::map<std::string, Vehicle_DataInfo_st> &vehicle_datainfo)
{
    QString sheet_name;
    std::vector<QString> header_date_vec;
    std::vector<Total_Data_Elem_st> data_elem_vec;

    sheet_name =  QString::fromStdString(vehicle_name_key_map[vehicle_name]);
    xlsx.addSheet(sheet_name);

//    for (auto iter : date_map)
//    {
//        Total_Data_Elem_st data_elem;

//        header_date_vec.push_back(QString::fromStdString(iter.first));

//        if (vehicle_datainfo.count(iter.first) > 0)
//        {
//            data_elem.test_min = (vehicle_datainfo.at(iter.first).vehicle_data.vehicle_test_min);
//            data_elem.auto_min = (vehicle_datainfo.at(iter.first).vehicle_data.vehicle_auto_min);
//            data_elem.total_mileage = (vehicle_datainfo.at(iter.first).vehicle_data.vehicle_total_mileage);
//            data_elem.auto_mileage = (vehicle_datainfo.at(iter.first).vehicle_data.vehicle_auto_mileage);
//            data_elem.sliphand_num = vehicle_datainfo.at(iter.first).vehicle_data.vehicle_sliphands.size();
//            data_elem.test_hour = (vehicle_datainfo.at(iter.first).vehicle_data.vehicle_test_min / 60.0);
//            data_elem.auto_hour = (vehicle_datainfo.at(iter.first).vehicle_data.vehicle_auto_min/ 60.0);
//        } else {
//            data_elem.test_min = 0;
//            data_elem.auto_min = 0;
//            data_elem.total_mileage = 0;
//            data_elem.auto_mileage = 0;
//            data_elem.sliphand_num = 0;
//            data_elem.test_hour = 0;
//            data_elem.auto_hour = 0;
//        }
//        data_elem_vec.push_back(data_elem);
//    }

    if (vehicle_datainfo.size())
    {
        for (auto iter : vehicle_datainfo)
        {
            Total_Data_Elem_st data_elem;
            header_date_vec.push_back(QString::fromStdString(iter.first));

            data_elem.test_min = iter.second.vehicle_data.vehicle_test_min;
            data_elem.auto_min = iter.second.vehicle_data.vehicle_auto_min;
            data_elem.total_mileage = iter.second.vehicle_data.vehicle_total_mileage;
            data_elem.auto_mileage = iter.second.vehicle_data.vehicle_auto_mileage;
            data_elem.sliphand_num = static_cast<int>(iter.second.vehicle_data.vehicle_sliphands.size());
            data_elem.test_hour = iter.second.vehicle_data.vehicle_test_min / 60.0;
            data_elem.auto_hour = iter.second.vehicle_data.vehicle_auto_min/ 60.0;

            data_elem_vec.push_back(data_elem);
        }
    } else {
        for (auto iter : date_map)
        {
            header_date_vec.push_back(QString::fromStdString(iter.first));
        }
    }

    write_vehicle_total_sheet_header(header_date_vec);

    write_vehicle_total_sheet_data(data_elem_vec);


    return true;
}

void xlsx_handler::write_vehicle_takeover_sheet_header()
{
    QXlsx::Format format;
    format.setFontColor(QColor(Qt::black));
    format.setFontBold(true);
    format.setFontSize(8);
    format.setHorizontalAlignment(QXlsx::Format::AlignRight);
    //序号
    xlsx.setColumnWidth(1, 1, 10);
    //车辆名称
    xlsx.setColumnWidth(2, 2, 15);
    //时间
    xlsx.setColumnWidth(3, 3, 20);
    //脱离地点
    xlsx.setColumnWidth(4, 4, 11);
    //接管用时
    xlsx.setColumnWidth(5, 5, 11);
    //车辆状态
    xlsx.setColumnWidth(6, 6, 11);
    //天气
    xlsx.setColumnWidth(7, 7, 11);
    //天气(值)
    xlsx.setColumnWidth(8, 8, 11);
    //外部环境
    xlsx.setColumnWidth(9, 9, 18);
    //外部环境(值)
    xlsx.setColumnWidth(10, 10, 18);
    //脱离原因
    xlsx.setColumnWidth(11, 11, 15);
    //星期
    xlsx.setColumnWidth(12, 12, 11);

    xlsx.setColumnWidth(13, 13, 15);

    xlsx.setColumnWidth(14, 14, 15);

    xlsx.write("A1", "序号", format);
    xlsx.write("B1", "车辆名称", format );
    xlsx.write("C1", "时间", format);
    xlsx.write("D1", "脱离地点", format);
    xlsx.write("E1", "接管用时", format);
    xlsx.write("F1", "车辆状态", format);
    xlsx.write("G1", "天气", format);
    xlsx.write("H1", "天气(值)", format);
    xlsx.write("I1", "外部环境", format);
    xlsx.write("J1", "外部环境(值)", format);
    xlsx.write("K1", "脱离原因", format);
    xlsx.write("L1", "星期", format);
    xlsx.write("M1", "", format);
    xlsx.write("N1", "", format);
    xlsx.write("O1", "", format);
}

void xlsx_handler::write_vehicle_takeover_sheet_data(const int index, const Takeover_Data_Elem_st &vehicle_dataelem)
{
    QXlsx::Format format;
    format.setFontColor(QColor(Qt::black));
//    format.setFontBold(true);
    format.setFontSize(8);
    format.setHorizontalAlignment(QXlsx::Format::AlignRight);
    QString weather_str = "=VLOOKUP(LEFT(C"+QString("%1").arg(index) +",4)&\"/\"&MID(C"+QString("%1").arg(index)+",6,2)&\"/\"&MID(C" \
                   +QString("%1").arg(index)+",9,2),M2:N"+QString("%1").arg(1 + weather_map.size())+",2,0)";

    QString env_str = "=H" + QString("%1").arg(index) + "&\"，外部车流量正常\"";
    //
    xlsx.write("A" + QString("%1").arg(index), vehicle_dataelem.seqnum, format);
    //
    xlsx.write("B" + QString("%1").arg(index), vehicle_dataelem.vehiclename, format);
    //
    xlsx.write("C" + QString("%1").arg(index), vehicle_dataelem.sliphandtime, format);
    //
    xlsx.write("D" + QString("%1").arg(index), vehicle_dataelem.position, format);
    //
    xlsx.write("E" + QString("%1").arg(index), vehicle_dataelem.slipusetime, format);
    //
    xlsx.write("F" + QString("%1").arg(index), vehicle_dataelem.vehiclestatus, format);
    //天气
    xlsx.write("G" + QString("%1").arg(index), weather_str, format);
    //天气(值)
    xlsx.write("H" + QString("%1").arg(index), weather_str, format);
    //外部环境
    xlsx.write("I" + QString("%1").arg(index), env_str, format);
    //外部环境(值)
    xlsx.write("J" + QString("%1").arg(index), env_str, format);
    //
    xlsx.write("K" + QString("%1").arg(index), vehicle_dataelem.reason, format);
    //
    xlsx.write("L" + QString("%1").arg(index), vehicle_dataelem.day_str, format);
}

void xlsx_handler::write_vehicle_takeover_weather_data_sheet()
{
    int index = 2;
    QXlsx::Format format;
    format.setFontColor(QColor(Qt::black));
    format.setFontBold(true);
    format.setFontSize(8);
    format.setHorizontalAlignment(QXlsx::Format::AlignRight);

    for (auto &iter : weather_map)
    {
        xlsx.write("M" + QString("%1").arg(index), QString::fromStdString(iter.first), format);
        xlsx.write("N" + QString("%1").arg(index), QString::fromStdString(iter.second), format);
        index++;
    }
}

bool xlsx_handler::write_vehicle_takeover_sheet(const std::string &vehicle_name, const std::map<std::string, Vehicle_DataInfo_st> &vehicle_data_map)
{
    QString sheet_name;
    int index = 2;

    sheet_name = "takeover_" + QString::fromStdString(vehicle_name_key_map[vehicle_name]);
    xlsx.addSheet(sheet_name);

    write_vehicle_takeover_sheet_header();

    for (auto &iter : vehicle_data_map)
    {
        size_t i;
//        Vehicle_DataInfo_st datainfo = ;
        for (i=0; i< iter.second.vehicle_data.vehicle_sliphands.size(); i++)
        {
            QDate date_time(iter.second.vehicle_data.vehicle_date.year, iter.second.vehicle_data.vehicle_date.mon, iter.second.vehicle_data.vehicle_date.day);
            Takeover_Data_Elem_st data_elem;

//            memset(&data_elem, 0x00, sizeof(data_elem));
            data_elem.seqnum = QString("%1").arg(index - 1);
            data_elem.vehiclename = QString::fromStdString(iter.second.vehicle_data.vehicle_id);
            unix_date_time_to_qstring(iter.second.vehicle_data.vehicle_sliphands[i].sliphand_end_time, data_elem.sliphandtime);
            data_elem.position = "";
            data_elem.slipusetime = "1s";
            data_elem.vehiclestatus = "车道保持";
            data_elem.weather_str = QString::fromStdString(iter.second.weather_data);
            data_elem.weather_value = QString::fromStdString(iter.second.weather_data);
            data_elem.env_str = QString::fromStdString(iter.second.weather_data) + "，外部车流量正常";
            data_elem.env_value = QString::fromStdString(iter.second.weather_data) + "，外部车流量正常";

            if ((date_time.dayOfWeek() == 2) || (date_time.dayOfWeek() == 7))
            {
                data_elem.reason = "周围车辆危险驾驶";
            } else {
                data_elem.reason = "道路不连续";
            }

            data_elem.day_str = date_time.toString("ddd");

            write_vehicle_takeover_sheet_data(index, data_elem);

            index++;
        }
    }

    write_vehicle_takeover_weather_data_sheet();

    return true;
}

//void xlsx_handler::write_vehicle_takeover_total_sheet_header(const std::vector<QString> header_date)
//{

//}

void xlsx_handler::write_vehicle_takeover_total_vehicle_data(const int row, const int col, const TakeoverTotal_Data_Elem_st &data_elem)
{
    QXlsx::Format format;
    format.setFontColor(QColor(Qt::black));
    format.setFontSize(8);
    format.setHorizontalAlignment(QXlsx::Format::AlignRight);
    format.setNumberFormat("0.00");

    xlsx.write(row, col, data_elem.total_mileage, format);
    xlsx.write(row+1, col, data_elem.auto_mileage, format);
    xlsx.write(row+2, col, data_elem.test_hour, format);
    xlsx.write(row+3, col, data_elem.auto_hour, format);
}

void xlsx_handler::write_vehicle_takeover_total_vehicle_data_sheet(const int row, const std::string vehicle_name, const std::map<std::string, Vehicle_DataInfo_st> vehicle_data_map, const std::map<std::string, bool> vehicle_date_map)
{
    int col = 1;
    //
    QString vehicle_name_str;
    QXlsx::Format format;
    format.setFontColor(QColor(Qt::black));
    format.setFontBold(true);
    format.setFontSize(8);
    format.setHorizontalAlignment(QXlsx::Format::AlignRight);

    vehicle_name_str = QString::fromStdString(vehicle_name);
    xlsx.write(row, col, vehicle_name_str, format);
    xlsx.mergeCells(CellRange(row, col, row+3, col), format);
    col++;
    xlsx.write(row, col, "total_mileage(km)", format);
    xlsx.write(row+1, col, "auto_mileage(km)", format);
    xlsx.write(row+2, col, "test_hour", format);
    xlsx.write(row+3, col, "auto_hour", format);
    col++;

    for (auto iter = vehicle_date_map.begin(); iter != vehicle_date_map.end(); iter++)
    {
        if (vehicle_data_map.count(iter->first) > 0)
        {
            TakeoverTotal_Data_Elem_st dataelem;

            dataelem.total_mileage = (vehicle_data_map.at(iter->first).vehicle_data.vehicle_total_mileage);
            dataelem.auto_mileage = (vehicle_data_map.at(iter->first).vehicle_data.vehicle_auto_mileage);
            dataelem.test_hour = (vehicle_data_map.at(iter->first).vehicle_data.vehicle_test_min / 60.0);
            dataelem.auto_hour = (vehicle_data_map.at(iter->first).vehicle_data.vehicle_auto_min / 60.0);
            write_vehicle_takeover_total_vehicle_data(row, col, dataelem);
        } else {
            TakeoverTotal_Data_Elem_st dataelem;

            dataelem.total_mileage = 0;
            dataelem.auto_mileage = 0;
            dataelem.test_hour = 0;
            dataelem.auto_hour = 0;
            write_vehicle_takeover_total_vehicle_data(row, col, dataelem);
        }
        col++;
    }
}

bool xlsx_handler::write_vehicle_takeover_total_sheet(const std::map<std::string, bool> &vehicle_date_map, const std::map<std::string, std::map<std::string, Vehicle_DataInfo_st>> &vehicle_info_map)
{
    int row, col;
    QString sheet_name;
    QXlsx::Format format;

//    std::map<std::string, bool> vehicle_date_map;

    sheet_name = "takeover_total";
    xlsx.addSheet(sheet_name);

//    unique_vehicledate_str(vehicle_info_map, vehicle_date_map);
    row = 6;
    col = 3;
    //set cloumn width
    //vehicle name
    xlsx.setColumnWidth(1, 1, 18);
    //vehicle info describe
    xlsx.setColumnWidth(2, 2, 18);
    //vehicle Data info
    xlsx.setColumnWidth(3, 3 + vehicle_date_map.size()-1, 15);
    //vehicle Total info
    xlsx.setColumnWidth(3 + vehicle_date_map.size()+1, 3 + vehicle_date_map.size() +2, 14);

    for (auto &iter : vehicle_date_map)
    {
        format.setFontColor(QColor(Qt::black));
        format.setFontBold(true);
        format.setFontSize(8);
        format.setHorizontalAlignment(QXlsx::Format::AlignRight);

        xlsx.write(row, col++, QString::fromStdString(iter.first), format);

    }
    row++;
    for (auto &iter : vehicle_info_map)
    {
        write_vehicle_takeover_total_vehicle_data_sheet(row, iter.first, iter.second, vehicle_date_map);
        row+= 5;
    }

    return true;
}

void xlsx_handler::write_total_header(const std::vector<std::string> &vehicle_vec)
{
    QXlsx::Format format;
    format.setFontColor(QColor(Qt::black));
    format.setFontBold(true);
    format.setFontSize(8);
    format.setHorizontalAlignment(QXlsx::Format::AlignRight);
    //
    xlsx.setColumnWidth(1, 1, 15);
    xlsx.write(3, 1, "测试总里程(km)", format);
    xlsx.write(4, 1, "自动驾驶总里程（km(km)", format);
    xlsx.write(5, 1, "测试时长(小时）", format);
    xlsx.write(6, 1, "自动驾驶时长(小时)", format);
    xlsx.write(7, 1, "接管次数", format);
    xlsx.write(8, 1, "周围车辆危险驾驶", format);
    xlsx.write(9, 1, "道路不连续", format);

    //车辆名称
    for (size_t index = 0; index < vehicle_vec.size(); index++)
    {
        int col = index + 2;
        xlsx.setColumnWidth(col, col, 15);
        xlsx.write(2, col, QString::fromStdString(vehicle_vec[index]), format );
    }
}

void xlsx_handler::write_total_data(const std::map<std::string, VehicleTotal_Data_Elem_st> &vehicle_total_map)
{
    int row = 3;
    int col = 2;
    QXlsx::Format format, format1;
    format.setFontColor(QColor(Qt::black));
    format.setFontSize(8);
    format.setNumberFormat("0.00");
    format.setHorizontalAlignment(QXlsx::Format::AlignRight);

    format1.setFontColor(QColor(Qt::black));
    format1.setFontSize(8);
    format1.setHorizontalAlignment(QXlsx::Format::AlignRight);

    for (auto &iter : vehicle_total_map)
    {
        xlsx.write(row, col,  iter.second.total_mileage, format);
        xlsx.write(row+1, col, iter.second.auto_mileage, format);
        xlsx.write(row+2, col, iter.second.test_hour, format);
        xlsx.write(row+3, col, iter.second.auto_hour, format);
        xlsx.write(row+4, col, iter.second.sliphand_num, format1);
        xlsx.write(row+5, col, iter.second.sliphand_num_env, format1);
        xlsx.write(row+6, col, iter.second.sliphand_num_road, format1);
        col++;
    }
}

bool xlsx_handler::write_total_sheet(const std::map<std::string, std::map<std::string, Vehicle_DataInfo_st>> &vehicle_info_map)
{
    QString sheet_name = "Total";
    std::vector<std::string> vehicle_vec;
    std::map<std::string, VehicleTotal_Data_Elem_st> vehicle_total_map;

    xlsx.addSheet(sheet_name);


    for (auto &iter : vehicle_info_map)
    {
        VehicleTotal_Data_Elem_st elem;
        double total_mileage, auto_mileage, test_min, test_hour, auto_hour, auto_min;
        int sliphand_num = 0;
        int sliphand_num_env = 0;
        int sliphand_num_road = 0;

        total_mileage = auto_mileage = 0;
        test_hour = auto_hour = 0;
        test_min = auto_min = 0;

        vehicle_vec.push_back(iter.first);
        for (auto &iter1 : iter.second)
        {
            QDate date_time(iter1.second.vehicle_data.vehicle_date.year, iter1.second.vehicle_data.vehicle_date.mon, iter1.second.vehicle_data.vehicle_date.day);

            total_mileage += iter1.second.vehicle_data.vehicle_total_mileage;
            auto_mileage += iter1.second.vehicle_data.vehicle_auto_mileage;

            test_min += iter1.second.vehicle_data.vehicle_test_min;
            auto_min += iter1.second.vehicle_data.vehicle_auto_min;

            sliphand_num += iter1.second.vehicle_data.vehicle_sliphands.size();

            if ((date_time.dayOfWeek() == 2) || (date_time.dayOfWeek() == 7))
            {
                sliphand_num_env += iter1.second.vehicle_data.vehicle_sliphands.size();
            } else {
                sliphand_num_road += iter1.second.vehicle_data.vehicle_sliphands.size();
            }
        }
        test_hour = test_min / 60.0;
        auto_hour = auto_min / 60.0;

        elem.total_mileage = (total_mileage);
        elem.auto_mileage = (auto_mileage);
        elem.test_hour = (test_hour);
        elem.auto_hour = (auto_hour);
        elem.sliphand_num = sliphand_num;
        elem.sliphand_num_env = sliphand_num_env;
        elem.sliphand_num_road = sliphand_num_road;

        vehicle_total_map[iter.first] = elem;
    }
    write_total_header(vehicle_vec);
    write_total_data(vehicle_total_map);

    return true;
}

bool xlsx_handler::export_to_excel(const std::map<std::string, std::string> &weather_info_map, const std::map<std::string, std::map<std::string, Vehicle_DataInfo_st>> &vehicle_info_map)
{
    std::map<std::string, bool> date_map;

    for (auto &iter : vehicle_info_map)
    {
        for (auto iter1 : iter.second)
        {
            //char temp[15] = {0};
            //std::string date_str;
            //sprintf(temp, "%04d/%02d/%02d", std::get<0>(iter1).filedate.year, std::get<0>(iter1).filedate.mon, std::get<0>(iter1).filedate.day);
            //date_str = temp;
            date_map[iter1.first] = true;
        }
    }

    weather_map = weather_info_map;

    for (auto &iter : vehicle_info_map)
    {
        write_vehicle_takeover_sheet(iter.first, iter.second);
        write_vehicle_total_sheet(date_map, iter.first, iter.second);
    }
    write_vehicle_takeover_total_sheet(date_map, vehicle_info_map);
    write_total_sheet(vehicle_info_map);

    return true;
}

bool xlsx_handler::save(const std::string &filepathname)
{
    xlsx.saveAs(QString::fromStdString(filepathname));

    return true;
}
