#pragma once


#include "cWindow.h"
#include "resource.h"
#include "DataSource.h"
#include "JavaClass.hpp"


class cPropertiesPage : public cWindow
{
private:

	cTextField _textField;
	DataSource* _dataSource;

	void onSizeChange(SIZE& size) override;

public:

	cPropertiesPage(const cModule& module, cWindow* parent, DataSource* data_source) :
		cWindow(module, IDD_PROPERTIES, parent),
		_dataSource(data_source),
		_textField(this, IDC_PROPERTIES_EDIT)
	{

	}

	void parse();
	void setFont(HFONT font) { _textField.setFont(font); }
	void clear() { _textField.clear(); }
	bool empty() { return _textField.empty(); }
};

