#pragma once


#include "cWindow.h"
#include "resource.h"
#include "DataSource.h"
#include "cAttributesPage.h"
#include "JavaClass.hpp"


class cFieldsPage : public cWindow
{
private:

	DataSource* _dataSource;
	cComboBox _fieldComboBox;
	cTextField _accFlagsTextField;
	cTextField _nameIndexTextField;
	cTextField _descIndexTextField;
	cTextField _descriptionTextField;
	cTextField _attributesCountTextField;
	cAttributesPage _attributesPage;

	void onSelectionChange(IControl& control, ISelectionIndex& index);
	void onSizeChange(SIZE& size);

	void fieldChange(INT index);
	void updateField(java::JavaClass& java, const java::JavaClassField* field);

public:

	cFieldsPage(const cModule& module, cWindow* parent, DataSource* data_source) :
		cWindow(module, IDD_FIELDS, parent),
		_dataSource(data_source),
		_fieldComboBox(this, IDC_FIELDS_COMBO),
		_accFlagsTextField(this, IDC_FIELDS_ACCFLAGS_EDIT),
		_nameIndexTextField(this, IDC_FIELDS_NAMEINDEX_EDIT),
		_descIndexTextField(this, IDC_FIELDS_DESCINDEX_EDIT),
		_descriptionTextField(this, IDC_FIELDS_DESCRIPTION_EDIT),
		_attributesCountTextField(this, IDC_FIELDS_ATTRIBUTESCOUNT_EDIT),
		_attributesPage(module, this, data_source)
	{
	}

	void setFont(HFONT font) { _attributesPage.setFont(font); }
	BOOL empty() { return _fieldComboBox.empty(); }
	void parse();
	void clear();
};

