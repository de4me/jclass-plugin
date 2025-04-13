

#include "cAttributeUnknownPage.h"
#include "HexFormatter.hpp"


#include <format>


void cAttributeUnknownPage::onSizeChange(SIZE& size) {

	RECT page_rect = rect();
	RECT attrlength_rect = _attrLengthTextField.rect();
	RECT attribute_rect = _attributeTextField.rect();

	attribute_rect.left -= page_rect.left;
	attrlength_rect.left -= page_rect.left;

	SIZE attrlength_size = { size.cx - attrlength_rect.left - attribute_rect.left, attrlength_rect.bottom - attrlength_rect.top };
	_attrLengthTextField.setSize(attrlength_size);
	_nameIndexTextField.setSize(attrlength_size);

	attribute_rect.top -= page_rect.top;

	SIZE attribute_size = { size.cx - attribute_rect.left * 2, size.cy - attribute_rect.top };
	_attributeTextField.setSize(attribute_size);
}


void cAttributeUnknownPage::parse(const java::JavaClassAttribute* attr) {

	if (_dataSource == nullptr) {
		return;
	}

	auto java = _dataSource->java();

	string name_string = java.nameAtIndex(attr->name_index());
	string nameindex_string = format("{:<6} // {}", attr->name_index(), name_string);

	_nameIndexTextField.setText(nameindex_string);

	string attrlength_string = to_string(attr->length());
	_attrLengthTextField.setText(attrlength_string);

	HexFormatter hex(32);
	hex.format( reinterpret_cast<const uint8_t*>(attr->bytes), attr->length() );
	auto attribute_string = hex.str();
	_attributeTextField.setText(attribute_string);
}