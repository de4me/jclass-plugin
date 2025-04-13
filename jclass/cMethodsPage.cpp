#include "cMethodsPage.h"
#include "JavaHelper.hpp"
#include "JavaTypeDescription.hpp"


#include <format>


using namespace java;


void cMethodsPage::onSizeChange(SIZE& size) {

	RECT page_rect = rect();
	SIZE page_size = { page_rect.right - page_rect.left , page_rect.bottom - page_rect.top };

	RECT combobox_rect = _methodComboBox.rect();
	INT space_left = combobox_rect.left - page_rect.left;
	SIZE combobox_size = { page_size.cx - space_left * 2, combobox_rect.bottom - combobox_rect.top };
	_methodComboBox.setSize(combobox_size);

	RECT accflags_rect = _accFlagsTextField.rect();
	SIZE accflags_size = { page_size.cx - (accflags_rect.left - page_rect.left) - space_left, accflags_rect.bottom - accflags_rect.top };
	_accFlagsTextField.setSize(accflags_size);
	_nameIndexTextField.setSize(accflags_size);
	_descIndexTextField.setSize(accflags_size);
	_descriptionTextField.setSize(accflags_size);
	_attributesCountTextField.setSize(accflags_size);

	RECT attrcount_rect = _attributesCountTextField.rect();
	RECT attrpage_rect = { 0, attrcount_rect.bottom - page_rect.top + space_left, size.cx, size.cy};
	_attributesPage.setRect(attrpage_rect);
}


void cMethodsPage::onSelectionChange(IControl& control, ISelectionIndex& index) {

	switch (control.id())
	{
	case IDC_METHODS_COMBO:
		methodChange(index.selectedIndex());
		break;
	default:
		break;
	}
}


void cMethodsPage::updateMethod(JavaClass& java, const JavaClassMethod* method) {

	auto name = java.nameAtIndex(method->name_index());
	auto description = java.nameAtIndex(method->descriptor_index());
	auto access_flags = JavaHelperMethodAccessFlags(method->access_flag());
	auto typedescription = JavaTypeDescription(description);
	auto typedescription_name = typedescription.str();
	auto name_and_flags = JavaHelperAccessFlagsAndName(access_flags, name);

	auto accflag_string = format("0x{:0>4x} // {}", static_cast<uint16_t>(method->access_flag()), access_flags);
	auto name_string = format("{:<6} // {}", method->name_index(), name);
	auto description_string = format("{:<6} // {}", method->descriptor_index(), description);
	auto typedescription_string = format("{:<6} {}", name_and_flags, typedescription_name);
	auto attributescount_string = to_string(method -> attributes_count());

	_accFlagsTextField.setText(accflag_string, CP_UTF8);
	_nameIndexTextField.setText(name_string, CP_UTF8);
	_descIndexTextField.setText(description_string, CP_UTF8);
	_descriptionTextField.setText(typedescription_string, CP_UTF8);
	_attributesCountTextField.setText(attributescount_string);

	_attributesPage.parse(method->attributeIterator());
}


void cMethodsPage::clear() {

	_methodComboBox.clear();
	_accFlagsTextField.clear();
	_nameIndexTextField.clear();
	_descIndexTextField.clear();
	_descriptionTextField.clear();
	_attributesCountTextField.clear();
	_attributesPage.clear();
}


void cMethodsPage::parse() {

	if (_dataSource == nullptr) {
		return;
	}

	auto java = _dataSource->java();

	clear();

	auto iterator = java.methodIterator();
	for (auto method = iterator.current(); method != nullptr; method = iterator.next()) {

		auto method_name = java.nameAtIndex(method->name_index());
		auto text = java.nameAtIndex(method->descriptor_index());
		auto description = JavaTypeDescription(text);
		auto value_type = description.str();
		auto name = format("{}: {} {}", iterator.index(), method_name, value_type);
		auto wname = multibytetowstring(name, CP_UTF8);
		_methodComboBox.add(wname);
	}
}


void cMethodsPage::methodChange(INT index) {

	if (_dataSource == nullptr) {
		return;
	}

	auto java = _dataSource->java();
	auto iter = java.methodIterator();
	for (auto method = iter.current(); method != nullptr; method = iter.next()) {
		if (iter.index() == index) {
			updateMethod(java, method);
			return;
		}
	}
}

