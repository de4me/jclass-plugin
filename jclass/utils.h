#pragma once


#include <windows.h>
#include <stdint.h>
#include <string>


unsigned long nozerosul(unsigned long value, int* count);
unsigned long roundul(double value, int count);


std::string format_file_size(unsigned long long size);
std::string format_datetime(FILETIME* time);