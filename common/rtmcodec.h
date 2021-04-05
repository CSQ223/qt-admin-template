/*
 *Copyright (C) 2020 Tusimple.  All rights reserved.
*/
#pragma once

#include <stdint.h>
#include "common.h"
#include "error.h"

#pragma pack(1)


struct VehicleMonitorInfo
{
    unix_date_time time;
    uint8_t ad_mode;
    double latitude;
    double longitude;
    double altitude;
    double azimuth;
    double velocity;
    double accx;
    double accy;
    double autometer;
    uint8_t takeover;
    uint8_t turnsignel;
    double steerangle;
    double yawvelocity;
    double accelerator_pdeal;
};

#pragma pack()

