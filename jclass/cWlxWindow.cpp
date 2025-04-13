

#include "cWlxWindow.h"
#include "resource.h"
#include "JavaHelper.hpp"
#include "JavaParserText.hpp"
#include "JavaTypeDescription.hpp"
#include "utils.h"


#include <sstream>
#include <format>
#include <codecvt>
#include <CommCtrl.h>


cModule module;


cWlxWindow::cWlxWindow(HWND parent) :
	cWindow(module, IDD_MAIN, parent),
	_propertiesPage(module, this, this),
	_headerPage(module, this, this),
	_methodsPage(module, this, this),
	_fieldsPage(module, this, this),
	_attributesPage(module, this, this),
	_tabControl(this, IDC_MAIN_TAB)
{
	_tabControl.addPage(_T("File Properties"), _propertiesPage);
	_tabControl.addPage(_T("File Header"), _headerPage);
	_tabControl.addPage(_T("Fields"), _fieldsPage);
	_tabControl.addPage(_T("Methods"), _methodsPage);
	_tabControl.addPage(_T("Attributes"), _attributesPage);
	_tabControl.setSelectedIndex(0);

	_font = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("Courier New"));
	_propertiesPage.setFont(_font);
	_headerPage.setFont(_font);
	_methodsPage.setFont(_font);
	_fieldsPage.setFont(_font);
	_attributesPage.setFont(_font);
}


cWlxWindow::~cWlxWindow() {

	if (_font) {
		DeleteObject(_font);
	}
}


void cWlxWindow::onDeinit() {

}


void cWlxWindow::onSizeChange(SIZE& size) {

	_tabControl.setSize(size);
}


void cWlxWindow::onPageChange(INT page) {

	setParentFocus();

	switch (page) {
	case 0:

		if (!_propertiesPage.empty()) {
			return;
		}
		_propertiesPage.parse();
		break;

	case 1:

		if (!_headerPage.empty()) {
			return;
		}
		_headerPage.parse();
		break;

	case 2:

		if (!_fieldsPage.empty()) {
			return;
		}
		_fieldsPage.parse();
		break;

	case 3:

		if (!_methodsPage.empty()) {
			return;
		}
		_methodsPage.parse();
		break;

	case 4:

		if (!_attributesPage.empty()) {
			return;
		}
		_attributesPage.parse(_java.attrIterator());
		break;

	default:
		break;
	}
}


void cWlxWindow::onSelectionChange(IControl& control, ISelectionIndex& index) {

	switch (control.id())
	{
	case IDC_MAIN_TAB:
		onPageChange(index.selectedIndex());
		break;
	default:
		break;
	}
}


void cWlxWindow::setParentFocus() {

	auto parent = GetParent(handle());
	SetFocus(parent);
}


void cWlxWindow::pageNext() {

	auto index = _tabControl.selectedIndex() + 1;
	_tabControl.setSelectedIndex(index < _tabControl.count() ? index : 0);
	onSelectionChange(_tabControl, _tabControl);
}


void cWlxWindow::pagePrev() {

	auto index = _tabControl.selectedIndex() - 1;
	_tabControl.setSelectedIndex(index >= 0 ? index : static_cast<INT>(_tabControl.count()) - 1);
	onSelectionChange(_tabControl, _tabControl);
}


void cWlxWindow::parse() {

	switch (_tabControl.selectedIndex()) {
	case 1:
		_headerPage.parse();
		_propertiesPage.clear();
		_methodsPage.clear();
		_fieldsPage.clear();
		_attributesPage.clear();
		break;

	case 2:
		_fieldsPage.parse();
		_methodsPage.clear();
		_propertiesPage.clear();
		_headerPage.clear();
		_attributesPage.clear();
		break;

	case 3:
		_methodsPage.parse();
		_propertiesPage.clear();
		_headerPage.clear();
		_fieldsPage.clear();
		_attributesPage.clear();
		break;

	case 4:
		_attributesPage.parse(_java.attrIterator());
		_propertiesPage.clear();
		_headerPage.clear();
		_methodsPage.clear();
		_fieldsPage.clear();
		break;

	default:
		_propertiesPage.parse();
		_headerPage.clear();
		_methodsPage.clear();
		_fieldsPage.clear();
		_attributesPage.clear();
		break;
	}
}

