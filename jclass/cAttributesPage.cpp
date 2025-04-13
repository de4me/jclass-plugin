

#include "cAttributesPage.h"
#include "cAttributeUnknownPage.h"


void cAttributesPage::onSizeChange(SIZE& size) {

	RECT page = rect();
	RECT tab_rect = _tabControl.rect();

	tab_rect.left -= page.left;
	tab_rect.top -= page.top;

	SIZE tab_size = { size.cx - tab_rect.left * 2, size.cy - tab_rect.top * 2 };
	_tabControl.setSize(tab_size);
}


void cAttributesPage::clear() {

	_tabControl.clear();

	for (auto page : _pages) {

		DestroyWindow(page->handle());
		free(page);
	}

	_pages.clear();
}


void cAttributesPage::parse(java::AttributeIterator attributes) {

	if (_dataSource == nullptr) {
		return;
	}

	auto java = _dataSource->java();

	INT code_index = 0;

	for (auto attr = attributes.current(); attr != nullptr; attr = attributes.next()) {

		auto attrtag = java.attrTagAtIndex(attr->name_index());
		auto name = java.nameAtIndex(attr->name_index());
		auto wname = multibytetowstring(name, CP_UTF8);

		cWindow* page = nullptr;

		switch (attrtag)
		{
		case java::ATTRTAG_CODE:
			code_index = attributes.index();
			page = new cAttributeUnknownPage(_module, this, _dataSource, _font, attr);
			break;

		default:
			page = new cAttributeUnknownPage(_module, this, _dataSource, _font,  attr);
			break;
		}

		if (page == nullptr) {
			continue;
		}

		_pages.push_back(page);
		_tabControl.addPage(wname, *page);
	}

	_tabControl.setSelectedIndex(code_index);
}