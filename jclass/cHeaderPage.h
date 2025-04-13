#pragma once


#include "cWindow.h"
#include "resource.h"
#include "DataSource.h"
#include "JavaClass.hpp"


class cHeaderPage : public cWindow
{
private:

	DataSource* _dataSource;
	cTextField _textField;

	void onSizeChange(SIZE& size) override;

public:

	cHeaderPage(const cModule& module, cWindow* parent, DataSource* data_source) :
		cWindow(module, IDD_HEADER, parent),
		_dataSource(data_source),
		_textField(this, IDC_HEADER_EDIT)
	{

	}

	void parse();
	void setFont(HFONT font) { _textField.setFont(font); }
	void clear() { _textField.clear(); }
	bool empty() { return _textField.empty(); }
};

