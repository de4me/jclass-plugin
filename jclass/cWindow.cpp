

#include "cWindow.h"


#include <vector>
#include <format>
#include <CommCtrl.h>
#include <windowsx.h>


template<class T> void cList<T>::append(T* item) {

	if (auto current = this->_last) {
		current->_next = item;
		this->_last = item;
	}
	else {
		this->_next = item;
		this->_last = item;
	}
}


cIcon cModule::icon(INT iResourceID) {

	return cIcon(iResourceID, *this);
}


cControl::cControl(cWindow* parent, INT iResourceID) {

	this->_id = iResourceID;
	this->_parent = parent;
	this->_handle = GetDlgItem(parent->handle(), iResourceID);
	parent->registerControl(this);
}


FRAME cControl::bounds() {

	RECT rect = { 0 };
	GetClientRect(handle(), &rect);
	return {rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top};
}


BOOL cControl::isHidden() {

	return !IsWindowVisible(handle());
}


void cControl::setHidden(BOOL hidden) {

	ShowWindow(handle(), hidden ? SW_HIDE : SW_SHOWNOACTIVATE);
}


BOOL cControl::isEnabled() {

	return IsWindowEnabled(handle());
}


void cControl::setEnabled(BOOL enabled) {

	EnableWindow(handle(), enabled);
}


void cControl::setText(const tstring text) {

	SetWindowText(handle(), text.c_str());
}


tstring cControl::text() {

	size_t count = GetWindowTextLength(handle());
	vector<TCHAR> buffer(count + 1);
	Edit_GetText(handle(), buffer.data(), static_cast<INT>(buffer.size()));
	return tstring(buffer.data(), count);
}


void cControl::setSize(SIZE size) {

	SetWindowPos(handle(), nullptr, 0, 0, size.cx, size.cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}


SIZE cControl::size() {

	RECT rect;
	GetClientRect(handle(), &rect);
	MapDialogRect(handle(), &rect);
	return { rect.right, rect.bottom };
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


void cComboBox::setItems(vector<tstring> items) {

	this->_items = items;
	ComboBox_ResetContent(handle());
	for (auto &text : items) {
		ComboBox_AddString(handle(), text.c_str());
	}
	setSelectedIndex(0);
	FRAME rect = bounds();
	LONG height = rect.size.cy;
	height += ComboBox_GetItemHeight(handle()) * static_cast<LONG>(items.size());
	height += GetSystemMetrics(SM_CXFRAME) * 2;
	SetWindowPos(handle(), NULL, 0, 0, rect.size.cx, height, SWP_NOACTIVATE | SWP_NOZORDER);
}


INT cComboBox::selectedIndex() {

	return ComboBox_GetCurSel(handle());
}


void cComboBox::setSelectedIndex(INT index) {

	ComboBox_SetCurSel(handle(), index);
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
	for (auto item : _items) {
		SetWindowPos(item.handle, HWND_TOP, rect.point.x, rect.point.y, rect.size.cx, rect.size.cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}
}


void cTabControl::addPageWindow(cWindow& child, tstring title) {

	TabItem item;
	item.handle = child.handle();
	this->_items.push_back(item);
	if (_hasTabs) {
		TCITEM citem;
		citem.mask = TCIF_TEXT;
		citem.pszText = (LPTSTR) title.c_str();
		TabCtrl_InsertItem(handle(), _items.size(), &citem);
	}
	SetParent(child.handle(), handle());
}


INT cTabControl::selectedIndex() {

	for (INT i = 0; i < static_cast<INT>(_items.size()); i++) {
		if (IsWindowVisible(_items[i].handle)) {
			return i;
		}
	}
	return -1;
}


void cTabControl::setSelectedIndex(INT index) {

	TabCtrl_SetCurSel(handle(), index);

	updateSelectedIndex(index);
}


void cTabControl::updateSelectedIndex(INT index) {

	for (INT i = 0; i < static_cast<INT>(_items.size()); i++) {
		HWND wnd = _items[i].handle;
		if (i == index) {
			adjustPageFrame(wnd);
			ShowWindow(wnd, SW_SHOWNA);
		}
		else {
			ShowWindow(wnd, SW_HIDE);
		}
	}
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


cWindow::cWindow(cModule& module, INT iResourceID, HWND parent) :
	_parent(nullptr)
{

	_handle = CreateDialogParam(module.handle(), MAKEINTRESOURCE(iResourceID), parent, DlgProc, reinterpret_cast<LPARAM>(this));
}


cWindow::cWindow(cModule& module, INT iResourceID, cWindow* parent)
{
	_parent = parent;
	HWND wnd = parent ? parent->handle() : nullptr;
	_handle = CreateDialogParam(module.handle(), MAKEINTRESOURCE(iResourceID), wnd, DlgProc, reinterpret_cast<LPARAM>(this));
}


SIZE cWindow::size() {

	RECT rect = { 0 };
	GetClientRect(handle(), &rect);
	return {rect.right - rect.left, rect.bottom - rect.top};
}


FRAME cWindow::bounds() {

	RECT rect = { 0 };
	GetClientRect(handle(), &rect);
	return {rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top};
}


BOOL cWindow::isHidden() {

	return !IsWindowVisible(handle());
}


void cWindow::setHidden(BOOL hidden) {

	ShowWindow(handle(), hidden ? SW_HIDE : SW_SHOW);
}


void cWindow::close() {

	DestroyWindow(handle());
}


cControl* cWindow::findControlWithId(UINT_PTR iResourceID) {

	for (cControl* control = this->next(); control != nullptr; control = control->next()) {
		if (control->id() != iResourceID) {
			continue;
		}
		return control;
	}
	return nullptr;
}


cControl* cWindow::findControlWithHandle(HWND handle) {

	for (cControl* control = this->next(); control != nullptr; control = control->next()) {
		if (control->handle() != handle) {
			continue;
		}
		return control;
	}
	return nullptr;
}


void cWindow::setCaption(tstring caption) {

	SetWindowText(handle(), caption.c_str());
}


void cWindow::setIcon(cIcon icon) {

	SendMessage(handle(), WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon.handle()));
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
		cControl* control = window->findControlWithId(notify->idFrom);
		if (control == nullptr) {
			return FALSE;
		}

		cWindow* parent = window->parent();
		if (cTabControl* tabcontrol = dynamic_cast<cTabControl*>(control)) {
			switch (notify->code)
			{
			case TCN_SELCHANGE:
			{
				INT index = TabCtrl_GetCurSel(tabcontrol->handle());
				tabcontrol->updateSelectedIndex(index);
				window->onSelectionChange(*tabcontrol, *tabcontrol);
				if (parent) {
					parent->onSelectionChange(*tabcontrol, *tabcontrol);
				}
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
				if (parent) {
					parent->onValueChange(*slider, *slider);
				}
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
		cControl* control = window->findControlWithId(LOWORD(wparam));
		if (control == nullptr) {
			return FALSE;
		}
		cWindow* parent = window->parent();
		if (cCheckBox* checkbox = dynamic_cast<cCheckBox*>(control)) {
			switch (notify)
			{
			case BN_CLICKED:

				window->onStateChange(*checkbox, *checkbox);
				if (parent) {
					parent->onStateChange(*checkbox, *checkbox);
				}
				return FALSE;

			case BN_DISABLE:

				window->onDisable(*checkbox);
				if (parent) {
					parent->onDisable(*checkbox);
				}
				return FALSE;

			case BN_SETFOCUS:

				window->onFocus(*checkbox);
				if (parent) {
					parent->onFocus(*checkbox);
				}
				return FALSE;

			case BN_KILLFOCUS:

				window->onKillFocus(*checkbox);
				if (parent) {
					parent->onKillFocus(*checkbox);
				}
				return FALSE;

			default:

				return FALSE;
			}
		} else  if (cRadioButton* radio = dynamic_cast<cRadioButton*>(control)) {
			switch (notify)
			{
			case BN_CLICKED:

				window->onStateChange(*radio, *radio);
				if (parent) {
					parent->onStateChange(*radio, *radio);
				}
				return FALSE;

			case BN_DISABLE:

				window->onDisable(*radio);
				if (parent) {
					parent->onDisable(*radio);
				}
				return FALSE;

			case BN_SETFOCUS:

				window->onFocus(*radio);
				if (parent) {
					parent->onFocus(*radio);
				}
				return FALSE;

			case BN_KILLFOCUS:

				window->onKillFocus(*radio);
				if (parent) {
					parent->onKillFocus(*radio);
				}
				return FALSE;

			default:

				return FALSE;
			}
		} else if (cButton* button = dynamic_cast<cButton*>(control)) {
			switch (notify)
			{
			case BN_CLICKED:

				window->onClick(*button);
				if (parent) {
					parent->onClick(*button);
				}
				return FALSE;

			case BN_DISABLE:

				window->onDisable(*button);
				if (parent) {
					parent->onDisable(*button);
				}
				return FALSE;

			case BN_SETFOCUS:

				window->onFocus(*button);
				if (parent) {
					parent->onFocus(*button);
				}
				return FALSE;

			case BN_KILLFOCUS:

				window->onKillFocus(*button);
				if (parent) {
					parent->onKillFocus(*button);
				}
				return FALSE;

			default:
				return FALSE;
			}
		} else if (cTextField* field = dynamic_cast<cTextField*>(control)) {
			switch (notify)
			{
			case EN_SETFOCUS:

				window->onFocus(*field);
				if (parent) {
					parent->onFocus(*field);
				}
				return FALSE;

			case EN_KILLFOCUS:

				window->onKillFocus(*field);
				if (parent) {
					parent->onKillFocus(*field);
				}
				return FALSE;

			case EN_CHANGE:

				window->onChange(*field);
				if (parent) {
					parent->onChange(*field);
				}
				return FALSE;

			default:

				return FALSE;
			}
		} else if (cComboBox* combobox = dynamic_cast<cComboBox*>(control)) {
			switch (notify)
			{
			case CBN_SELCHANGE:

				window->onSelectionChange(*combobox, *combobox);
				if (parent) {
					parent->onSelectionChange(*combobox, *combobox);
				}
				return FALSE;

			case CBN_EDITCHANGE:

				window->onChange(*combobox);
				if (parent) {
					parent->onChange(*combobox);
				}
				return FALSE;

			case CBN_SETFOCUS:

				window->onFocus(*combobox);
				if (parent) {
					parent->onFocus(*combobox);
				}
				return FALSE;

			case CBN_KILLFOCUS:

				window->onKillFocus(*combobox);
				if (parent) {
					parent->onKillFocus(*combobox);
				}
				return FALSE;

			default:

				return FALSE;
			}
		} else if (cStepper* stepper = dynamic_cast<cStepper*>(control)) {
			switch (notify)
			{
			case BN_CLICKED:

				window->onValueChange(*stepper, *stepper);
				if (parent) {
					parent->onValueChange(*stepper, *stepper);
				}
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
		cControl* control = window->findControlWithHandle(reinterpret_cast<HWND>(lparam));
		if (control == nullptr) {
			return FALSE;
		}

		cWindow* parent = window->parent();
		if (cSlider* slider = dynamic_cast<cSlider*>(control)) {
			switch (notify) {

			case SB_LINELEFT:
			case SB_LINERIGHT:

				window->onValueChange(*slider, *slider);
				if (parent) {
					parent->onValueChange(*slider, *slider);
				}
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

