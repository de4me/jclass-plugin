

#include "cWindow.h"


#include <vector>
#include <format>
#include <CommCtrl.h>
#include <windowsx.h>


const tstring empty_string(_T(""));


const wstring multibytetowstring(const string& text, UINT code_page) {

	auto length = MultiByteToWideChar(code_page, 0, text.c_str(), -1, nullptr, 0);
	if (length == 0) {
		return empty_string;
	}
	wstring result(length, 0);
	if (MultiByteToWideChar(code_page, 0, text.c_str(), -1, result.data(), length) == 0) {
		return empty_string;
	}
	return result;
}


cIcon cModule::icon(INT iResourceID) const {

	auto icon = LoadIcon(handle(), MAKEINTRESOURCE(iResourceID));
	return cIcon(icon);
}


cControl::cControl(cWindow* parent, INT iResourceID) {

	_id = iResourceID;
	_parent = parent;
	_handle = GetDlgItem(parent->handle(), iResourceID);
	parent->registerControl(this);
}


FRAME cControl::bounds() {

	RECT rc = rect();
	return { 0, 0, rc.right - rc.left, rc.bottom - rc.top};
}


RECT cControl::rect() {

	RECT rect;
	GetWindowRect(handle(), &rect);
	return rect;
}


void cControl::setSize(SIZE& size) {

	SetWindowPos(handle(), nullptr, 0, 0, size.cx, size.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}


SIZE cControl::size() {

	RECT rc = rect();
	return { rc.right - rc.left, rc.bottom - rc.top };
}


ControlState cCheckBox::state() {

	switch (Button_GetCheck(handle()))
	{
	case BST_UNCHECKED:
		return ControlStateOff;
	case BST_CHECKED:
		return ControlStateOn;
	default:
		return ControlStateMixed;
	}
}


void cCheckBox::setState(ControlState state) {

	int new_state;
	switch (state)
	{
	case ControlStateOff:

		new_state = BST_UNCHECKED;
		break;

	case ControlStateOn:

		new_state = BST_CHECKED;
		break;

	default:

		new_state = BST_INDETERMINATE;
		break;
	}
	Button_SetCheck(handle(), new_state);
}


ControlState cRadioButton::state() {

	switch (Button_GetCheck(handle()))
	{
	case BST_CHECKED:

		return ControlStateOn;

	default:

		return ControlStateOff;
	}
}


void cRadioButton::setState(ControlState state) {

	int new_state;
	switch (state)
	{
	case ControlStateOn:
		new_state = BST_CHECKED;
		break;

	default:

		new_state = BST_UNCHECKED;
		break;
	}
	Button_SetCheck(handle(), new_state);
}


const tstring cComboBox::stringAtIndex(INT index) {

	auto length = ComboBox_GetLBTextLen(handle(), index);
	if (length == 0) {
		return empty_string;
	}
	tstring result(length + 1, 0);
	if (ComboBox_GetLBText(handle(), index, result.data()) != length) {
		return empty_string;
	}
	return result;
}


void cComboBox::setStrings(const vector<tstring>& strings) {

	ComboBox_ResetContent(handle());
	for (auto const &str : strings) {
		add(str);
	}
}


void cLabel::setText(const string& text, UINT codepage) {
	if (codepage == CP_ACP) {
		SetWindowTextA(handle(), text.c_str());
	}
	else {
		auto wstring = multibytetowstring(text, codepage);
		setText(wstring);
	}
}


tstring cLabel::text() {

	size_t count = GetWindowTextLength(handle());
	vector<TCHAR> buffer(count + 1);
	Edit_GetText(handle(), buffer.data(), static_cast<INT>(buffer.size()));
	return tstring(buffer.data(), count);
}


cTabControl::TabItem* cTabControl::TabItem::newTabItem(cWindow& window, const tstring& caption, UINT_PTR tabid) {

	size_t caption_size = (caption.size() + 1) * sizeof(TCHAR);
	size_t item_size = sizeof(TabItem) + caption_size;

	TabItem* result = reinterpret_cast<TabItem*>(malloc(item_size));
	if (result == nullptr) {
		return nullptr;
	}
	result->mask = TCIF_TEXT | TCIF_PARAM;
	result->pszText = result->caption;
	result->handle = window.handle();
	result->id = tabid;
	memcpy(result->caption, caption.c_str(), caption_size);
	return result;
}


void cTabControl::addPage(const tstring& title, cWindow& child, UINT_PTR tabid) {

	TabItem* item = TabItem::newTabItem(child, title, tabid);
	if (item == nullptr) {
		return;
	}

	_tabItems.push_back(item);

	if (_hasTabs) {
		TabCtrl_InsertItem(handle(), _tabItems.size(), item);
	}

	SetParent(child.handle(), handle());
}


FRAME cTabControl::bounds() {

	RECT rect;
	GetClientRect(handle(), &rect);
	TabCtrl_AdjustRect(handle(), FALSE, &rect);
	return {rect.left, rect.top, rect.right-rect.left, rect.bottom - rect.top};
}


void cTabControl::adjustPageFrame(HWND page) {

	FRAME rect = bounds();
	SetWindowPos(page, HWND_TOP, rect.point.x, rect.point.y, rect.size.cx, rect.size.cy, SWP_NOACTIVATE | SWP_NOZORDER);
}


void cTabControl::setSize(SIZE size) {

	cControl::setSize(size);
	FRAME rect = bounds();
	for (auto item : _tabItems) {
		SetWindowPos(item->handle, nullptr, rect.point.x, rect.point.y, rect.size.cx, rect.size.cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}
}


INT cTabControl::selectedIndex() {

	INT index = 0;
	for (auto item: _tabItems) {
		if (IsWindowVisible(item->handle)) {
			return index;
		}
		index++;
	}
	return -1;
}


void cTabControl::setSelectedIndex(INT index) {

	TabCtrl_SetCurSel(handle(), index);
	updateSelectedIndex(index);
}


void cTabControl::updateSelectedIndex(INT index) {

	INT current = 0;
	for (auto item: _tabItems) {
		HWND handle = item->handle;
		if (current == index) {
			adjustPageFrame(handle);
			ShowWindow(handle, SW_SHOWNA);
		}
		else {
			ShowWindow(handle, SW_HIDE);
		}
		current++;
	}
}


void cTabControl::clear() {

	TabCtrl_DeleteAllItems(handle());

	for (auto item: _tabItems) {

		DestroyWindow(item->handle);
		free(item);
	}

	_tabItems.clear();
}


INT cSlider::value() {

	return static_cast<INT>(SendMessage(handle(), TBM_GETPOS, 0, 0));
}


void cSlider::setValue(INT value) {

	SendMessage(handle(), TBM_SETPOS, TRUE, value);
}


INT cSlider::minValue() {

	return static_cast<INT>(SendMessage(handle(), TBM_GETRANGEMIN, 0, 0));
}


void cSlider::setMinValue(INT value) {

	SendMessage(handle(), TBM_SETRANGEMIN, TRUE, value);
}


INT cSlider::maxValue() {

	return static_cast<INT>(SendMessage(handle(), TBM_GETRANGEMAX, 0, 0));
}


void cSlider::setMaxValue(INT value) {

	SendMessage(handle(), TBM_SETRANGEMAX, TRUE, value);
}


INT cStepper::value() {

	return static_cast<INT>(SendMessage(handle(), UDM_GETPOS32, 0, 0));
}


void cStepper::setValue(INT value) {

	SendMessage(handle(), UDM_SETPOS32, 0, value);
}


INT cStepper::minValue() {

	INT value;
	SendMessage(handle(), UDM_GETRANGE32, (WPARAM) &value, 0);
	return value;
}


void cStepper::setMinValue(INT value) {

	INT max_value = maxValue();
	SendMessage(handle(), UDM_SETRANGE32, value, max_value);
}


INT cStepper::maxValue() {

	INT value;
	SendMessage(handle(), UDM_GETRANGE32, 0, (LPARAM) &value);
	return value;
}


void cStepper::setMaxValue(INT value) {

	INT min_value = minValue();
	SendMessage(handle(), UDM_SETRANGE32, min_value, value);
}


INT cProgress::value() {

	return static_cast<INT>(SendMessage(handle(), PBM_GETPOS, 0, 0));
}


void cProgress::setValue(INT value) {

	SendMessage(handle(), PBM_SETPOS, value, 0);
}


INT cProgress::minValue() {

	return static_cast<INT>(SendMessage(handle(), PBM_GETRANGE, TRUE, 0));
}


void cProgress::setMinValue(INT value) {

	INT max_value = maxValue();
	SendMessage(handle(), PBM_SETRANGE32, value, max_value);
}


INT cProgress::maxValue() {

	return static_cast<INT>(SendMessage(handle(), PBM_GETRANGE, FALSE, 0));
}


void cProgress::setMaxValue(INT value) {

	INT min_value = minValue();
	SendMessage(handle(), PBM_SETRANGE32, min_value, value);
}


cWindow::cWindow(const cModule& module, UINT_PTR iResourceID, HWND parent) :
	_parent(nullptr),
	_id(iResourceID)
{

	_handle = CreateDialogParam(module.handle(), MAKEINTRESOURCE(iResourceID), parent, DlgProc, reinterpret_cast<LPARAM>(this));
}


cWindow::cWindow(const cModule& module, UINT_PTR iResourceID, cWindow* parent) :
	_id(iResourceID)
{
	_parent = parent;
	HWND wnd = parent ? parent->handle() : nullptr;
	_handle = CreateDialogParam(module.handle(), MAKEINTRESOURCE(iResourceID), wnd, DlgProc, reinterpret_cast<LPARAM>(this));
}


SIZE cWindow::size() {

	RECT rc = rect();
	return { rc.right - rc.left, rc.bottom - rc.top};
}


FRAME cWindow::bounds() {

	RECT rc = rect();
	return {0, 0, rc.right - rc.left, rc.bottom - rc.top};
}


RECT cWindow::rect() {

	RECT rect;
	GetWindowRect(handle(), &rect);
	return rect;
}


cControl* cWindow::controlWithId(UINT_PTR iResourceID) {

	for (auto const control : _registredControls) {
		if (control->id() == iResourceID) {
			return control;
		}
	}
	return nullptr;
}


cControl* cWindow::controlWithHandle(HWND handle) {

	for (auto const control : _registredControls) {
		if (control->handle() == handle) {
			return control;
		}
	}
	return nullptr;
}


void cWindow::setCaption(tstring caption) {

	SetWindowText(handle(), caption.c_str());
}


void cWindow::setIcon(cIcon icon) {

	SendMessage(handle(), WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon.handle()));
}


void cWindow::onDeinit() {

}

void cWindow::onSizeChange(SIZE& size) {

}

void cWindow::onClick(IControl& control) {

	if (_parent == nullptr) {
		return;
	}
	_parent->onClick(control);
}

void cWindow::onStateChange(IControl& control, IState& state) {

	if (_parent == nullptr) {
		return;
	}
	_parent->onStateChange(control, state);
}


void cWindow::onSelectionChange(IControl& control, ISelectionIndex& index) {

	if (_parent == nullptr) {
		return;
	}
	_parent->onSelectionChange(control, index);
}


void cWindow::onValueChange(IControl& control, IValue& value) {

	if (_parent == nullptr) {
		return;
	}
	_parent->onValueChange(control, value);
}


void cWindow::onFocus(IControl& control) {

	if (_parent == nullptr) {
		return;
	}
	_parent->onFocus(control);
}


void cWindow::onKillFocus(IControl& control) {

	if (_parent == nullptr) {
		return;
	}
	_parent->onKillFocus(control);
}


void cWindow::onDisable(IControl& control) {

	if (_parent == nullptr) {
		return;
	}
	_parent->onDisable(control);
}


void cWindow::onChange(IControl& control) {

	if (_parent == nullptr) {
		return;
	}
	_parent->onChange(control);
}


INT_PTR cWindow::DlgProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) {

	cWindow* window;

#if _DEBUG
	auto message_text = format("{} {}\n", message, wparam);
	OutputDebugStringA(message_text.c_str());
#endif

	switch (message) {

	case WM_INITDIALOG:

		window = reinterpret_cast<cWindow*>(lparam);
		break;

	default:

		window = reinterpret_cast<cWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		break;
	}

	if (window == nullptr) {
		return FALSE;
	}

	switch (message)
	{

	case WM_INITDIALOG:

		SetWindowLongPtr(handle, GWLP_USERDATA, static_cast<LONG_PTR>(lparam));
		return FALSE;

	case WM_DESTROY:

		window->onDeinit();
		return FALSE;

	case WM_CLOSE:

		window->close();
		return FALSE;

	case WM_SIZE:
	{
		SIZE size = { LOWORD(lparam), HIWORD(lparam) };
		window->onSizeChange(size);
		return FALSE;
	}

	case WM_NOTIFY:
	{
		LPNMHDR notify = reinterpret_cast<LPNMHDR>(lparam);
#if _DEBUG
		auto message_text = format("NOTIFY: {}\n", notify->code);
		OutputDebugStringA(message_text.c_str());
#endif
		cControl* control = window->controlWithId(notify->idFrom);
		if (control == nullptr) {
			return FALSE;
		}

		if (cTabControl* tabcontrol = dynamic_cast<cTabControl*>(control)) {
			switch (notify->code)
			{
			case TCN_SELCHANGE:
			{
				INT index = TabCtrl_GetCurSel(tabcontrol->handle());
				tabcontrol->updateSelectedIndex(index);
				window->onSelectionChange(*tabcontrol, *tabcontrol);
				return TRUE;
			}

			default:
				return FALSE;
			}
		} else if (cSlider* slider = dynamic_cast<cSlider*>(control)) {
			switch (notify->code)
			{
			case TRBN_THUMBPOSCHANGING:
			case NM_RELEASEDCAPTURE:

				window->onValueChange(*slider, *slider);
				return FALSE;

			default:

				return FALSE;
			}
		} else if (cStepper* stepper = dynamic_cast<cStepper*>(control)) {
			switch (notify->code)
			{
			case UDN_DELTAPOS:

				PostMessage(window->handle(), WM_COMMAND, MAKELONG(notify->idFrom, BN_CLICKED), 0);
				return FALSE;

			default:

				return FALSE;
			}
		}

		return FALSE;
	}

	case WM_COMMAND:
	{
		INT notify = HIWORD(wparam);
#if _DEBUG
		tstring text = to_tstring(notify);
		OutputDebugString(text.c_str());
		OutputDebugString(L"=COMMAND\n");
#endif
		cControl* control = window->controlWithId(LOWORD(wparam));
		if (control == nullptr) {
			return FALSE;
		}

		if (cCheckBox* checkbox = dynamic_cast<cCheckBox*>(control)) {
			switch (notify)
			{
			case BN_CLICKED:

				window->onStateChange(*checkbox, *checkbox);
				return FALSE;

			case BN_DISABLE:

				window->onDisable(*checkbox);
				return FALSE;

			case BN_SETFOCUS:

				window->onFocus(*checkbox);
				return FALSE;

			case BN_KILLFOCUS:

				window->onKillFocus(*checkbox);
				return FALSE;

			default:

				return FALSE;
			}
		} else  if (cRadioButton* radio = dynamic_cast<cRadioButton*>(control)) {
			switch (notify)
			{
			case BN_CLICKED:

				window->onStateChange(*radio, *radio);
				return FALSE;

			case BN_DISABLE:

				window->onDisable(*radio);
				return FALSE;

			case BN_SETFOCUS:

				window->onFocus(*radio);
				return FALSE;

			case BN_KILLFOCUS:

				window->onKillFocus(*radio);
				return FALSE;

			default:

				return FALSE;
			}
		} else if (cButton* button = dynamic_cast<cButton*>(control)) {
			switch (notify)
			{
			case BN_CLICKED:

				window->onClick(*button);
				return FALSE;

			case BN_DISABLE:

				window->onDisable(*button);
				return FALSE;

			case BN_SETFOCUS:

				window->onFocus(*button);
				return FALSE;

			case BN_KILLFOCUS:

				window->onKillFocus(*button);
				return FALSE;

			default:
				return FALSE;
			}
		} else if (cTextField* field = dynamic_cast<cTextField*>(control)) {
			switch (notify)
			{
			case EN_SETFOCUS:

				window->onFocus(*field);
				return FALSE;

			case EN_KILLFOCUS:

				window->onKillFocus(*field);
				return FALSE;

			case EN_CHANGE:

				window->onChange(*field);
				return FALSE;

			default:

				return FALSE;
			}
		} else if (cComboBox* combobox = dynamic_cast<cComboBox*>(control)) {
			switch (notify)
			{
			case CBN_SELCHANGE:

				window->onSelectionChange(*combobox, *combobox);
				return FALSE;

			case CBN_EDITCHANGE:

				window->onChange(*combobox);
				return FALSE;

			case CBN_SETFOCUS:

				window->onFocus(*combobox);
				return FALSE;

			case CBN_KILLFOCUS:

				window->onKillFocus(*combobox);
				return FALSE;

			default:

				return FALSE;
			}
		} else if (cStepper* stepper = dynamic_cast<cStepper*>(control)) {
			switch (notify)
			{
			case BN_CLICKED:

				window->onValueChange(*stepper, *stepper);
				return FALSE;

			default:
				return FALSE;
			}
		}

		return FALSE;
	}

	case WM_VSCROLL:
	case WM_HSCROLL:
	{
		INT notify = LOWORD(wparam);
		cControl* control = window->controlWithHandle(reinterpret_cast<HWND>(lparam));
		if (control == nullptr) {
			return FALSE;
		}

		if (cSlider* slider = dynamic_cast<cSlider*>(control)) {
			switch (notify) {

			case SB_LINELEFT:
			case SB_LINERIGHT:

				window->onValueChange(*slider, *slider);
				return FALSE;

			default:

				return FALSE;
			}
		}

		return FALSE;
	}
	default:
		return FALSE;
	}
}

