#pragma once


#include "cWindow.h"
#include "resource.h"
#include "DataSource.h"
#include "JavaStruct.hpp"


class cAttributeUnknownPage : public cWindow
{
private:

	DataSource* _dataSource;
	cTextField _nameIndexTextField;
	cTextField _attrLengthTextField;
	cTextField _attributeTextField;

	void onSizeChange(SIZE& size) override;

	void parse(const java::JavaClassAttribute* attr);

public:

	cAttributeUnknownPage(const cModule& module, cWindow* parent, DataSource* data_source, HFONT font, const java::JavaClassAttribute* attr) :
		cWindow(module, IDD_ATTRIBUTE_UNKNOWN, parent),
		_dataSource(data_source),
		_nameIndexTextField(this, IDC_ATTRIBUTENAMEINDEX_EDIT),
		_attrLengthTextField(this, IDC_ATTRIBUTELENGTH_EDIT),
		_attributeTextField(this, IDC_ATTRIBUTE_EDIT)
	{
		_attributeTextField.setFont(font);
		parse(attr);
	}
};

