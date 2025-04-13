#pragma once


#include "cWindow.h"
#include "resource.h"
#include "DataSource.h"
#include "cAttributesPage.h"
#include "JavaClass.hpp"


class cMethodsPage : public cWindow
{
private:

	DataSource* _dataSource;
	cComboBox _methodComboBox;
	cTextField _accFlagsTextField;
	cTextField _nameIndexTextField;
	cTextField _descIndexTextField;
	cTextField _descriptionTextField;
	cTextField _attributesCountTextField;
	cAttributesPage _attributesPage;

	void onSelectionChange(IControl& control, ISelectionIndex& index);
	void onSizeChange(SIZE& size);

	void methodChange(INT index);
	void updateMethod(java::JavaClass& java, const java::JavaClassMethod* method);

public:

	cMethodsPage(const cModule& module, cWindow* parent, DataSource* data_source) :
		cWindow(module, IDD_METHODS, parent),
		_dataSource(data_source),
		_methodComboBox(this, IDC_METHOD_COMBO),
		_accFlagsTextField(this, IDC_METHOD_ACCFLAGS_EDIT),
		_nameIndexTextField(this, IDC_METHOD_NAMEINDEX_EDIT),
		_descIndexTextField(this, IDC_METHOD_DESCINDEX_EDIT),
		_descriptionTextField(this, IDC_METHOD_DESCRIPTION_EDIT),
		_attributesCountTextField(this, IDC_METHOD_ATTRIBUTESCOUNT_EDIT),
		_attributesPage(module, this, data_source)
	{
	}

	void setFont(HFONT font) { _attributesPage.setFont(font); }
	BOOL empty() { return _methodComboBox.empty(); }
	void parse();
	void clear();
};

