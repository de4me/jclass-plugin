
#include "cWlxWindow.h"


#include <sstream>
#include <format>
#include <codecvt>
#include <CommCtrl.h>


#include "resource.h"
#include "JavaHelper.hpp"
#include "JavaParserText.hpp"
#include "utils.h"


cModule module;


cWlxWindow::cWlxWindow(HWND parent) :
	cWindow(module, IDD_MAIN, parent),
	_tabControl(this, IDC_MAIN_TAB),
	_infoPage(module, IDD_INFO, this),
	_headerPage(module, IDD_HEADER, this),
	_infoTextField(&_infoPage, IDC_INFO_EDIT),
	_headerTextField(&_headerPage, IDC_HEADER_EDIT)
{

	_tabControl.addPageWindow(_infoPage, _T("File Properties"));
	_tabControl.addPageWindow(_headerPage, _T("File Header"));
	_tabControl.setSelectedIndex(0);

	_font = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("Courier New"));
	SendMessage(_infoTextField.handle(), WM_SETFONT, reinterpret_cast<WPARAM>(_font), 1);
	SendMessage(_headerTextField.handle(), WM_SETFONT, reinterpret_cast<WPARAM>(_font), 1);
}


cWlxWindow::~cWlxWindow() {

	if (_font) {
		DeleteObject(_font);
	}
}


void cWlxWindow::onDeinit() {

}


void cWlxWindow::onSizeChange(SIZE& size) {

	_tabControl.setSize(size);
	FRAME bounds = _infoPage.bounds();
	_infoTextField.setSize(bounds.size);
	_headerTextField.setSize(bounds.size);
}


void cWlxWindow::onSelectionChange(IControl& control, ISelectionIndex& index) {

	setParentFocus();

	switch (index.selectedIndex()) {
	case 0:

		if (_infoTextField.textLength() != 0) {
			return;
		}
		parseInfoPage();
		break;

	case 1:

		if (_headerTextField.textLength() != 0) {
			return;
		}
		parseHeaderPage();
		break;

	default:
		break;
	}
}


void cWlxWindow::setParentFocus() {

	auto parent = GetParent(handle());
	SetFocus(parent);
}


void cWlxWindow::pageNext() {

	auto index = _tabControl.selectedIndex() + 1;
	_tabControl.setSelectedIndex(index < _tabControl.pagesCount() ? index : 0);
	onSelectionChange(_tabControl, _tabControl);
}


void cWlxWindow::pagePrev() {

	auto index = _tabControl.selectedIndex() - 1;
	_tabControl.setSelectedIndex(index >= 0 ? index : static_cast<INT>(_tabControl.pagesCount()) - 1);
	onSelectionChange(_tabControl, _tabControl);
}


void cWlxWindow::parse() {

	switch (_tabControl.selectedIndex()) {
	case 1:
		parseHeaderPage();
		_infoTextField.setText(_T(""));
		break;

	default:
		parseInfoPage();
		_headerTextField.setText(_T(""));
		break;
	}
}


void cWlxWindow::parseHeaderPage() {

	java::JavaParserText parser;
	parser.parse(_java);
	auto text = parser.str();

	do {
		auto length = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
		if (length == 0) {
			break;
		}
		auto buffer = new WCHAR[length];
		auto result = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, buffer, length);
		if (result == 0) {
			break;
		}
		_headerTextField.setText(buffer);
		delete buffer;
		return;
	} while (false);

	SetWindowTextA(_headerTextField.handle(), text.c_str());
}


void cWlxWindow::parseInfoPage() {

	ostringstream is;

	is << _filePath << "\r\n";

	is << "\r\nFile Information:\r\n\r\n";

	auto file_attributes = GetFileAttributesA(_filePath.c_str());

	HANDLE file = CreateFileA(_filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

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

	is << format("{:26} : {:x}\r\n", "Magic", _java.magic());

	auto version = _java.version();
	auto java_se = JavaHelperVersion(version, true);
	if (java_se.empty()) {
		auto version_name = JavaHelperVersion(version);
		is << format("{:26} : {}\r\n", "Java Version", version_name);
	}
	else {
		is << format("{:26} : {}\r\n", "Java SE", java_se);
	}

	auto java_source = _java.sourceFile();
	if (!java_source.empty()) {
		is << format("{:26} : {}\r\n", "Source File", java_source);
	}
	
	auto access_flags = JavaHelperAccessFlags(_java.accessFlags());
	is << format("{:26} : {}\r\n", "Access Flags", access_flags);
	auto this_name = _java.nameAtIndex(_java.thisClass());
	is << format("{:26} : {}\r\n", "This Class", this_name);
	auto super_name = _java.nameAtIndex(_java.superClass());
	is << format("{:26} : {}\r\n", "Super Class", super_name);
	is << format("{:26} : {}\r\n", "Pool Count", _java.poolCount());
	is << format("{:26} : {}\r\n", "Interface Count", _java.interfacesCount());
	is << format("{:26} : {}\r\n", "Fields Count", _java.fieldsCount());
	is << format("{:26} : {}\r\n", "Methods Count", _java.methodsCount());
	is << format("{:26} : {}\r\n", "Attributes Count", _java.attributesCount());

	auto text = is.str();
	SetWindowTextA(_infoTextField.handle(), text.c_str());
}


