#pragma once


#include "cWindow.h"
#include "resource.h"
#include "DataSource.h"


#include <list>


class cAttributesPage : public cWindow
{
private:

	HFONT _font;
	cModule _module;
	cTabControl _tabControl;
	DataSource* _dataSource;
	list<cWindow*> _pages;

	void onSizeChange(SIZE& size) override;

public:

	cAttributesPage(const cModule& module, cWindow* parent, DataSource* data_source) :
		cWindow(module, IDD_ATTRIBUTES, parent),
		_module(module),
		_dataSource(data_source),
		_tabControl(this, IDC_ATTRIBUTES_TAB)
	{

	}

	~cAttributesPage() {
		clear();
	}

	void setFont(HFONT font) { _font = font; }
	void parse(java::AttributeIterator attributes);
	void clear();
	bool empty() { return _tabControl.empty(); }
};

