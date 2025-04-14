

#include "cPoolUnknownPage.h"
#include "HexFormatter.hpp"


#include <format>


void cPoolUnknownPage::onSizeChange(SIZE& size) {

	RECT page_rect = rect();
	
	RECT length_rect = _lengthTextField.rect();
	RECT text_rect = _textField.rect();

	text_rect.left -= page_rect.left;
	length_rect.left -= page_rect.left;

	SIZE length_size = { size.cx - length_rect.left - text_rect.left, length_rect.bottom - length_rect.top };

	_lengthTextField.setSize(length_size);

	text_rect.top -= page_rect.top;

	SIZE text_size = { size.cx - text_rect.left * 2, size.cy - text_rect.top };
	_textField.setSize(text_size);
}


void cPoolUnknownPage::parse(const java::JavaClassPool* attr) {

	if (_dataSource == nullptr) {
		return;
	}

	auto java = _dataSource->java();

	auto length = attr->sizeOfStruct() - sizeof(java::JavaClassPool::tag);
	string length_string = to_string(length);
	_lengthTextField.setText(length_string);

	HexFormatter hex(32);
	hex.format(reinterpret_cast<const uint8_t*>(attr) + sizeof(java::JavaClassPool::tag), length);
	auto attribute_string = hex.str();
	_textField.setText(attribute_string);
}