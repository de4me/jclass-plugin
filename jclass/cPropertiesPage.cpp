

#include "cPropertiesPage.h"
#include "utils.h"
#include "JavaHelper.hpp"


#include <sstream>
#include <format>


void cPropertiesPage::onSizeChange(SIZE& size) {

	_textField.setSize(size);
}


void cPropertiesPage::parse() {

	if (_dataSource == nullptr) {
		return;
	}

	auto path = _dataSource->filePath();
	auto java = _dataSource->java();

	ostringstream is;

	is << path << "\r\n";

	is << "\r\nFile Information:\r\n\r\n";

	auto file_attributes = GetFileAttributesA(path.c_str());

	HANDLE file = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (file != INVALID_HANDLE_VALUE) {

		LARGE_INTEGER file_size;

		if (GetFileSizeEx(file, &file_size)) {
			is << format("{:26} : {}\r\n", "File Size", format_file_size(file_size.QuadPart));
		}

		FILETIME create_time;
		FILETIME open_time;
		FILETIME write_time;

		if (GetFileTime(file, &create_time, &open_time, &write_time)) {
			is << format("{:26} : {}\r\n", "Creation Date", format_datetime(&create_time));
			is << format("{:26} : {}\r\n", "Modification Date", format_datetime(&write_time));
			is << format("{:26} : {}\r\n", "Open Date", format_datetime(&open_time));
		}

		CloseHandle(file);
	}

	is << "\r\nHeader Information:\r\n\r\n";

	is << format("{:26} : {:x}\r\n", "Magic", java.magic());

	auto version = java.version();
	auto java_se = JavaHelperVersion(version, true);
	if (java_se.empty()) {
		auto version_name = JavaHelperVersion(version);
		is << format("{:26} : {}\r\n", "Java Version", version_name);
	}
	else {
		is << format("{:26} : {}\r\n", "Java SE", java_se);
	}

	auto java_source = java.sourceFile();
	if (!java_source.empty()) {
		is << format("{:26} : {}\r\n", "Source File", java_source);
	}

	auto access_flags = JavaHelperAccessFlags(java.accessFlags());
	is << format("{:26} : {}\r\n", "Access Flags", access_flags);
	auto this_name = java.nameAtIndex(java.thisClass());
	is << format("{:26} : {}\r\n", "This Class", this_name);
	auto super_name = java.nameAtIndex(java.superClass());
	is << format("{:26} : {}\r\n", "Super Class", super_name);
	is << format("{:26} : {}\r\n", "Pool Count", java.poolCount());
	is << format("{:26} : {}\r\n", "Interface Count", java.interfacesCount());
	is << format("{:26} : {}\r\n", "Fields Count", java.fieldsCount());
	is << format("{:26} : {}\r\n", "Methods Count", java.methodsCount());
	is << format("{:26} : {}\r\n", "Attributes Count", java.attributesCount());

	auto text = is.str();
	_textField.setText(text);
}