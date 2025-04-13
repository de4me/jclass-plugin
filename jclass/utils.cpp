

#include "utils.h"


#include <cmath>
#include <format>
#include <tchar.h>


using namespace std;


unsigned long nozerosul(unsigned long value, int* count) {

    int index = 0;
    unsigned long result = value;
    if (result > 0) {
        while ((result % 10) == 0) {
            result /= 10;
            index++;
        }
    }
    if (count != nullptr) {
        *count = index;
    }
    return result;
}


unsigned long roundul(double value, int count) {

    double point = 10.0 * static_cast<double>(count);
    double result = value / point;
    return static_cast<unsigned long>(round(result) * point);
}


string format_file_size(unsigned long long size) {

	static const char value_suffixes_array[] = { 0, 'k', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y', 'B' };

	unsigned long long value = size;
	int suffix_index = 0;

	while (value >= 1024 * 1024) {
		if (suffix_index < (sizeof(value_suffixes_array) - 1)) {
			suffix_index++;
			value /= 1024;
		}
		else {
			break;
		}
	}

	unsigned long value_prefix;
	unsigned long value_suffix;

	if (value < 1024) {
		value_prefix = static_cast<unsigned long>(value);
		value_suffix = 0;
	}
	else {
		value = roundul(static_cast<double>(value) * (1000.0 / 1024.0), 2);
		value_prefix = static_cast<unsigned long>(value) / 1000;
		value_suffix = static_cast<unsigned long>(value) % 1000;
		suffix_index++;
	}

	if (value_suffix > 0) {
		int count;
		value_suffix = nozerosul(value_suffix, &count);
		switch (count) {
		case 1:
			return format("{}.{:02}{}", value_prefix, value_suffix, value_suffixes_array[suffix_index]);

		case 2:
			return format("{}.{}{}", value_prefix, value_suffix, value_suffixes_array[suffix_index]);

		default:
			return format("{}.{:03}{}", value_prefix, value_suffix, value_suffixes_array[suffix_index]);
		}
	}
	else {
		if (suffix_index > 0) {
			return format("{}{}", value_prefix, value_suffixes_array[suffix_index]);
		}
		else {
			return to_string(value_prefix);
		}
	}
}


std::string format_datetime(FILETIME* time) {

	CHAR datebuffer[64];
	CHAR timebuffer[64];

	SYSTEMTIME system_time;
	SYSTEMTIME time_zone;

	if (!FileTimeToSystemTime(time, &system_time)) {
		return "";
	}

	bool zone_time = SystemTimeToTzSpecificLocalTime(NULL, &system_time, &time_zone);

	if (GetDateFormatA(LOCALE_USER_DEFAULT, 0, zone_time ? &time_zone : &system_time, "dd MMMM yyyy", datebuffer, sizeof(datebuffer) / sizeof(datebuffer[0])) == 0) {
		return "";
	}

	if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, zone_time ? &time_zone : &system_time, "HH:mm:ss tt", timebuffer, sizeof(timebuffer) / sizeof(timebuffer[0])) == 0) {
		return "";
	}

	return zone_time ? format("{} {}", datebuffer, timebuffer) : format("{} {} UTC", datebuffer, timebuffer);
}


