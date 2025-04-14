#pragma once


#include "cWindow.h"
#include "resource.h"
#include "JavaStruct.hpp"
#include "DataSource.h"


class cPoolUnknownPage : public cWindow
{

private:

	DataSource* _dataSource;
	cTextField _lengthTextField;
	cTextField _textField;

	void onSizeChange(SIZE& size);

public:

	cPoolUnknownPage(const cModule& module, cWindow* parent, DataSource* data_source, HFONT font, const java::JavaClassPool* pool) :
		cWindow(module, IDD_POOL_UNKNOWN, parent),
		_dataSource(data_source),
		_lengthTextField(this, IDC_POOL_UNKNOWN_LENGTH_EDIT),
		_textField(this, IDC_POOL_UNKNOWN_EDIT)
	{
		_textField.setFont(font);
		parse(pool);
	}

	void parse(const java::JavaClassPool* pool);
};
