#pragma once


#include "cWindow.h"
#include "DataSource.h"
#include "JavaClass.hpp"
#include "cPropertiesPage.h"
#include "cHeaderPage.h"
#include "cMethodsPage.h"
#include "cFieldsPage.h"
#include "cAttributesPage.h"
#include "cPoolPage.h"


extern cModule module;


class cWlxWindow: public cWindow, public DataSource
{

private:

	string _filePath;
	HFONT _font;
	cTabControl _tabControl;
	cPropertiesPage _propertiesPage;
	cHeaderPage _headerPage;
	cPoolPage _poolPage;
	cMethodsPage _methodsPage;
	cFieldsPage _fieldsPage;
	cAttributesPage _attributesPage;
	java::JavaClass _java;

	void onDeinit() override;
	void onSizeChange(SIZE& size) override;
	void onSelectionChange(IControl&, ISelectionIndex&) override;

	void onPageChange(INT index);

public:

	cWlxWindow(HWND parent);
	~cWlxWindow();

	java::JavaClass& java() override { return _java; }
	const std::string& filePath() override { return _filePath; }

	void setFilePath(const string& path) { _filePath = path; }
	void setParentFocus();

	void read(istream& file, size_t size) { _java.read(file, size); }

	void parse();
	
	void pageNext();
	void pagePrev();
};

