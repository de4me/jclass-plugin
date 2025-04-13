#pragma once


#include <windows.h>
#include <CommCtrl.h>
#include <windowsx.h>
#include <tchar.h>
#include <string>
#include <array>
#include <vector>
#include <list>


using namespace std;


#ifdef _UNICODE
#define tstring wstring
#define to_tstring to_wstring
#else
#define tstring string
#define to_tstring to_string
#endif


class cModule;
class cIcon;
class cControl;
class cWindow;
class cButton;
class cComboBox;
class cLabel;
class cTextField;
class cWindow;


struct FRAME {
	POINT point;
	SIZE size;
};


extern const tstring empty_string;


const wstring multibytetowstring(const string& text, UINT code_page = CP_ACP);


class IControl {

public:

	virtual ~IControl() = default;

	virtual UINT_PTR id() = 0;
	virtual cWindow* parent() = 0;
	virtual HWND handle() = 0;

	virtual BOOL isHidden() = 0;
	virtual void setHidden(BOOL) = 0;

	virtual BOOL isEnabled() = 0;
	virtual void setEnabled(BOOL) = 0;
};


enum ControlState {

	ControlStateOff = 0,
	ControlStateOn = 1,
	ControlStateMixed = 2
};


class IState {

public:

	virtual ~IState() = default;
	virtual ControlState state() = 0;
	virtual void setState(ControlState) = 0;
};


class ISelectionIndex {

public:

	virtual ~ISelectionIndex() = default;
	virtual INT selectedIndex() = 0;
	virtual void setSelectedIndex(INT index) = 0;

};


class IValue {

public:

	virtual ~IValue() = default;
	virtual INT value() = 0;
	virtual void setValue(INT value) = 0;
	virtual INT minValue() = 0;
	virtual void setMinValue(INT value) = 0;
	virtual INT maxValue() = 0;
	virtual void setMaxValue(INT value) = 0;
};


class cModule {

private:

	HMODULE _handle;

public:

	cModule() {

		this->_handle = GetModuleHandle(nullptr);
	}

	cModule(HMODULE handle) :
		_handle(handle)
	{

	}

	HMODULE handle() const { return this->_handle; }
	void setHandle(HMODULE handle) { _handle = handle; }

	cIcon icon(INT iResourceID) const;
};


class cIcon {

private:

	HICON _handle;

public:

	cIcon(HICON handle) :
		_handle(handle)
	{

	}

	HICON handle() const { return this->_handle; }
};


class cControl: public IControl {

private:

	HWND _handle;
	cWindow* _parent;
	UINT_PTR _id;

public:

	cControl() :
		_handle(nullptr),
		_parent(nullptr),
		_id(0)
	{

	}

	cControl(cWindow* parent, INT iResourceID);
	cControl(cWindow* parent, HWND handle) :
		_handle(handle),
		_parent(parent)
	{

	}

	virtual ~cControl() = default;

	cWindow* parent() override { return this->_parent; }

	UINT_PTR id() override { return this->_id; }
	HWND handle() override { return this->_handle; }

	HWND setFocus() { return SetFocus(handle()); }
	HWND getFocus() { return GetFocus(); }
	
	BOOL isHidden() override { return !IsWindowVisible(handle()); }
	void setHidden(BOOL hidden) override { ShowWindow(handle(), hidden ? SW_HIDE : SW_SHOWNOACTIVATE); }
	
	BOOL isEnabled() override { return IsWindowEnabled(handle()); }
	void setEnabled(BOOL enabled) override { EnableWindow(handle(), enabled); }

	void setFont(HFONT font) { SendMessage(handle(), WM_SETFONT, reinterpret_cast<WPARAM>(font), 1); }

	LONG setExStyle(LONG style) { return SetWindowLong(handle(), GWL_EXSTYLE, style); }
	LONG exStyle() { return GetWindowLong(handle(), GWL_EXSTYLE); }

	virtual FRAME bounds();

	virtual RECT rect();
	virtual void setRect(RECT& rect) { SetWindowPos(handle(), nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE | SWP_NOZORDER); };

	virtual SIZE size();
	virtual void setSize(SIZE& size);

	virtual void clear() { ; }
};


class cButton : public cControl {

public:

	cButton(cWindow* parent, INT iResourceID) :
		cControl(parent, iResourceID)
	{

	}
};


class cCheckBox : public cButton, public IState {

public:

	cCheckBox(cWindow* parent, INT iResourceID) :
		cButton(parent, iResourceID)
	{

	}

	void setState(ControlState state) override;
	ControlState state() override;
};


class cRadioButton : public cButton, public IState {

public:

	cRadioButton(cWindow* parent, INT iResourceID) :
		cButton(parent, iResourceID)
	{

	}

	void setState(ControlState state) override;
	ControlState state() override;
};


class cComboBox : public cControl, public ISelectionIndex {

public:

	cComboBox(cWindow* parent, INT iResourceID) :
		cControl(parent, iResourceID)
	{

	}

	INT selectedIndex() override { return ComboBox_GetCurSel(handle()); }
	void setSelectedIndex(INT index) override { ComboBox_SetCurSel(handle(), index); }

	INT indexOf(const tstring& str) { return ComboBox_FindString(handle(), 0, str.c_str()); }

	INT add(const tstring& str) { return ComboBox_AddString(handle(), str.c_str()); }
	INT insert(const tstring& str, INT index) { return ComboBox_InsertString(handle(), index, str.c_str()); }
	size_t remove(const tstring& str) { return remove(indexOf(str)); }
	size_t remove(INT index) { return ComboBox_DeleteString(handle(), index); }

	const tstring stringAtIndex(INT index);

	void setStrings(const vector<tstring>& strings);

	int count() { return ComboBox_GetCount(handle()); }
	BOOL empty() { return count() == 0; }

	void clear() override { ComboBox_ResetContent(handle()); }
};


class cLabel : public cControl {

public:

	cLabel(cWindow* parent, INT iResourceID) :
		cControl(parent, iResourceID)
	{

	}

	void setText(const string& text, UINT codepage = CP_ACP);
	void setText(const wstring& text) { SetWindowTextW(handle(), text.c_str()); }

	tstring text();

	INT length() { return GetWindowTextLength(handle()); }
	BOOL empty() { return length() == 0; }

	void clear() { SetWindowText(handle(), nullptr); }
};


class cTextField : public cLabel {

public:

	cTextField(cWindow* parent, INT iResourceID) :
		cLabel(parent, iResourceID)
	{

	}
	
};


class cTabControl : public cControl, public ISelectionIndex {

	friend class cWindow;

	struct TabItem: public TCITEMHEADER {
		HWND handle;
		UINT_PTR id;
		TCHAR caption[];

		static TabItem* newTabItem(cWindow& window, const tstring& caption, UINT_PTR tabid = 0);
	};

private:

	list<TabItem*> _tabItems;
	BOOL _hasTabs;

	void adjustPageFrame(HWND page);
	void updateSelectedIndex(INT index);

public:

	cTabControl(cWindow* parent, INT iResourceID, BOOL hasTabs = TRUE) :
		cControl(parent, iResourceID),
		_hasTabs(hasTabs)
	{

	}

	~cTabControl() {

		clear();
	}

	INT count() { return static_cast<INT>(_tabItems.size()); }
	BOOL empty() { return count() == 0; }

	void addPage(const tstring& title, cWindow& window, UINT_PTR tabid = 0);

	void setSelectedIndex(INT index) override;
	INT selectedIndex() override;

	void setSize(SIZE size);
	FRAME bounds();

	void clear();
};


class cSlider : public cControl, public IValue {

public:

	cSlider(cWindow* parent, INT iResourceID, INT minValue = 0, INT maxValue = 100) :
		cControl(parent, iResourceID)
	{
		setMinValue(minValue);
		setMaxValue(maxValue);
	}

	INT value() override;
	void setValue(INT value) override;

	INT minValue() override;
	void setMinValue(INT value) override;

	INT maxValue() override;
	void setMaxValue(INT value) override;
};


class cProgress : public cControl, public IValue {

public:

	cProgress(cWindow* parent, INT iResourceID, INT minValue = 0, INT maxValue = 100) :
		cControl(parent, iResourceID)
	{
		setMinValue(minValue);
		setMaxValue(maxValue);
	}

	INT value() override;
	void setValue(INT value) override;

	INT minValue() override;
	void setMinValue(INT value) override;

	INT maxValue() override;
	void setMaxValue(INT value) override;
};


class cStepper : public cControl, public IValue {

public:

	cStepper(cWindow* parent, INT iResourceID, INT minValue = 0, INT maxValue = 100) :
		cControl(parent, iResourceID)
	{
		setMinValue(minValue);
		setMaxValue(maxValue);
		setValue(minValue);
	}

	INT value() override;
	void setValue(INT value) override;

	INT minValue() override;
	void setMinValue(INT value) override;

	INT maxValue() override;
	void setMaxValue(INT value) override;
};


class cWindow: public IControl {

private:

	HWND _handle;
	UINT_PTR _id;
	cWindow* _parent;
	list<cControl*> _registredControls;

	static INT_PTR WINAPI DlgProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

protected:

	virtual void onDeinit();
	virtual void onClick(IControl&);
	virtual void onStateChange(IControl&, IState&);
	virtual void onSelectionChange(IControl&, ISelectionIndex&);
	virtual void onValueChange(IControl&, IValue&);
	virtual void onFocus(IControl&);
	virtual void onKillFocus(IControl&);
	virtual void onDisable(IControl&);
	virtual void onChange(IControl&);
	virtual void onSizeChange(SIZE& size);

public:

	cWindow(const cModule& module, UINT_PTR iResourceID, cWindow* parent = nullptr);
	cWindow(const cModule& module, UINT_PTR iResourceID, HWND parent);

	HWND handle() override { return _handle; }
	cWindow* parent() override { return _parent; }
	UINT_PTR id() override { return _id; }

	BOOL isHidden() override { return !IsWindowVisible(handle()); }
	void setHidden(BOOL hidden) override { ShowWindow(handle(), hidden ? SW_HIDE : SW_SHOWNOACTIVATE); }

	BOOL isEnabled() override { return IsWindowEnabled(handle()); }
	void setEnabled(BOOL enabled) override { EnableWindow(handle(), enabled); }

	void close() { DestroyWindow(handle()); }

	void registerControl(cControl* control) { _registredControls.push_back(control); }
	void unregisterControl(cControl* control) { _registredControls.remove(control); }

	cControl* controlWithId(UINT_PTR iResourceID);
	cControl* controlWithHandle(HWND handle);

	FRAME bounds();

	SIZE size();
	void setSize(SIZE& size) { SetWindowPos(handle(), 0, 0, 0, size.cx, size.cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER); }

	RECT rect();
	void setRect(RECT& rect) { SetWindowPos(handle(), nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE | SWP_NOZORDER); };
	
	void setCaption(tstring caption);
	void setIcon(cIcon icon);

	HWND setFocus() { return SetFocus(handle()); }
	HWND getFocus() { return GetFocus(); }

	LONG setExStyle(LONG style) { return SetWindowLong(handle(), GWL_EXSTYLE, style); }
	LONG exStyle() { return GetWindowLong(handle(), GWL_EXSTYLE); }

};
