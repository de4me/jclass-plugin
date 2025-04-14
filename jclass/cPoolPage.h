#pragma once


#include "cWindow.h"
#include "resource.h"
#include "DataSource.h"
#include "JavaClass.hpp"


class cPoolPage : public cWindow
{

private:

	cModule _module;
	DataSource* _dataSource;
	HFONT _font;
	cComboBox _poolComboBox;
	cTextField _poolTagTextField;
	cWindow* _tagPage;

	void onSelectionChange(IControl& control, ISelectionIndex& index);
	void onSizeChange(SIZE& size) override;

	void tagPageRemove();
	RECT tagPageRect();
	void poolChange(INT index);
	void updatePool(java::JavaClass& java, const java::JavaClassPool* pool);
	const string poolName(java::JavaClass& java, uint16_t index, const java::JavaClassPool* pool);

public:

	cPoolPage(const cModule& module, cWindow* parent, DataSource* data_source) :
		cWindow(module, IDD_POOL, parent),
		_module(module),
		_dataSource(data_source),
		_tagPage(nullptr),
		_poolComboBox(this, IDC_POOL_COMBO),
		_poolTagTextField(this, IDC_POOL_TAG_EDIT)
	{

	}

	~cPoolPage() {

		tagPageRemove();
	}

	void setFont(HFONT font) { _font = font; }
	BOOL empty() { return _poolComboBox.empty(); }
	void parse();
	void clear();
};

