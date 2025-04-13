

#include "cHeaderPage.h"
#include "JavaParserText.hpp"


void cHeaderPage::onSizeChange(SIZE& size) {

	_textField.setSize(size);
}


void cHeaderPage::parse() {

	if (_dataSource == nullptr) {
		return;
	}

	auto java = _dataSource->java();
	java::JavaParserText parser;
	parser.parse(java);
	auto text = parser.str();
	_textField.setText(text, CP_UTF8);
}


