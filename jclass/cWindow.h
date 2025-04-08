#pragma once


#include <windows.h>
#include <tchar.h>
#include <string>
#include <array>
#include <vector>


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


struct FRAME {
	POINT point;
	SIZE size;
};


template<class T> class cList;


template<class T>
class cListItem {

	friend cList<T>;

private:

	T* _next;

public:

	T* next() { return this->_next; }
};


template<class T>
class cList : public cListItem<T>
{

private:

	T* _last;

public:

	void append(T* item);
};


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
	virtual FRAME bounds() = 0;
	virtual void setText(tstring) = 0;
	virtual tstring text() = 0;
	virtual size_t textLength() = 0;
	virtual HWND setFocus() = 0;
	virtual HWND getFocus() = 0;
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

	HMODULE handle() { return this->_handle; }
	void setHandle(HMODULE handle) { _handle = handle; }

	cIcon icon(INT iResourceID);
};


class cIcon {

private:

	HICON _handle;

public:

	cIcon(INT iResourceID, cModule module) {

		this->_handle = LoadIcon(module.handle(), MAKEINTRESOURCE(iResourceID));
	}

	cIcon(HICON handle) :
		_handle(handle)
	{

	}

	HICON handle() { return this->_handle; }
};


class cControl: public cListItem<cControl>, public IControl {

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

	HWND setFocus() override { return SetFocus(handle()); }
	HWND getFocus() override { return GetFocus(); }
	
	BOOL isHidden() override;
	void setHidden(BOOL hidden) override;
	
	BOOL isEnabled() override;
	void setEnabled(BOOL enabled) override;

	tstring text() override;
	void setText(const tstring text) override;
	size_t textLength() override { return SendMessage(handle(), WM_GETTEXTLENGTH, 0, 0); }

	FRAME bounds();
	SIZE size();
	void setSize(SIZE size);

	LONG setExStyle(LONG style) { return SetWindowLong(handle(), GWL_EXSTYLE, style); }
	LONG exStyle() { return GetWindowLong(handle(), GWL_EXSTYLE); }
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

private:

	vector<tstring> _items;

public:

	cComboBox(cWindow* parent, INT iResourceID) :
		cControl(parent, iResourceID)
	{

	}

	vector<tstring> items() { return this->_items; }
	void setItems(vector<tstring> items);

	INT selectedIndex() override;
	void setSelectedIndex(INT index) override;
};


class cLabel : public cControl {

public:

	cLabel(cWindow* parent, INT iResourceID) :
		cControl(parent, iResourceID)
	{

	}
};


class cTextField : public cLabel {

public:

	cTextField(cWindow* parent, INT iResourceID) :
		cLabel(parent, iResourceID)
	{

	}
};


class cTabControl : public cControl, public ISelectionIndex {

	struct TabItem {
		HWND handle;
	};

	friend class cWindow;

private:

	vector<TabItem> _items;
	BOOL _hasTabs;

	void adjustPageFrame(HWND page);
	void updateSelectedIndex(INT index);

public:

	cTabControl(cWindow* parent, INT iResourceID, BOOL hasTabs = TRUE) :
		cControl(parent, iResourceID),
		_hasTabs(hasTabs)
	{

	}

	INT pagesCount() { return static_cast<INT>(_items.size()); }
	void addPageWindow(cWindow& window, tstring title);

	void setSelectedIndex(INT index) override;
	INT selectedIndex() override;

	void setSize(SIZE size);
	FRAME bounds();
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


class cWindow: cList<cControl> {

private:

	HWND _handle;
	INT _id;
	cWindow* _parent;

	static INT_PTR WINAPI DlgProc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

public:

	cWindow(cModule& module, INT iResourceID, cWindow* parent = nullptr);
	cWindow(cModule& module, INT iResourceID, HWND parent);

	virtual void onDeinit() {};
	virtual void onClick(IControl&) {};
	virtual void onStateChange(IControl&, IState&) {};
	virtual void onSelectionChange(IControl&, ISelectionIndex&) {};
	virtual void onValueChange(IControl&, IValue&) {};
	virtual void onFocus(IControl&) {};
	virtual void onKillFocus(IControl&) {};
	virtual void onDisable(IControl&) {};
	virtual void onChange(IControl&) {};
	virtual void onSizeChange(SIZE& size) {};
	virtual void onNotification(NMHDR& notify) {};

	cControl* findControlWithId(UINT_PTR iResourceID);
	cControl* findControlWithHandle(HWND handle);

	HWND handle() { return this->_handle; }
	cWindow* parent() { return this->_parent; }

	FRAME bounds();
	SIZE size();
	void setSize(SIZE& size) { SetWindowPos(handle(), 0, 0, 0, size.cx, size.cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER); }

	BOOL isHidden();
	void setHidden(BOOL hidden);
	
	void close();
	void registerControl(cControl* control) { this->append(control); }
	
	void setCaption(tstring caption);
	void setIcon(cIcon icon);

	HWND setFocus() { return SetFocus(handle()); }
	HWND getFocus() { return GetFocus(); }

	LONG setExStyle(LONG style) { return SetWindowLong(handle(), GWL_EXSTYLE, style); }
	LONG exStyle() { return GetWindowLong(handle(), GWL_EXSTYLE); }
};
