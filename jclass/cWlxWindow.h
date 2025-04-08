#pragma once


#include "cWindow.h"
#include "JavaClass.hpp"


extern cModule module;


class cWlxWindow: public cWindow
{

private:

	string _filePath;
	HFONT _font;
	cTabControl _tabControl;
	cWindow _infoPage;
	cWindow _headerPage;
	cTextField _infoTextField;
	cTextField _headerTextField;
	java::JavaClass _java;

public:

	cWlxWindow(HWND parent);
	~cWlxWindow();

	void setFilePath(const string path) { _filePath = path; }
	void setParentFocus();

	void onDeinit() override;
	void onSizeChange(SIZE& size) override;
	void onSelectionChange(IControl&, ISelectionIndex&) override;

	void read(istream& file, size_t size) { _java.read(file, size); }

	void parse();
	void parseHeaderPage();
	void parseInfoPage();

	void pageNext();
	void pagePrev();
};

