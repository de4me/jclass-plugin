

#include "cPoolPage.h"
#include "JavaHelper.hpp"
#include "JavaTypeDescription.hpp"
#include "cPoolUnknownPage.h"


#include <format>


using namespace java;


void cPoolPage::onSizeChange(SIZE& size) {

	RECT page_rect = rect();

	RECT combo_rect = _poolComboBox.rect();
	combo_rect.left -= page_rect.left;
	SIZE combo_size = { size.cx - combo_rect.left * 2, combo_rect.bottom - combo_rect.top };

	_poolComboBox.setSize(combo_size);

	RECT tag_rect = _poolTagTextField.rect();
	tag_rect.left -= page_rect.left;
	SIZE tag_size = { size.cx - tag_rect.left - combo_rect.left, tag_rect.bottom - tag_rect.top };

	_poolTagTextField.setSize(tag_size);

    auto tag_page = _tagPage;

    if (tag_page == nullptr) {
        return;
    }

    RECT tag_page_rect = { combo_rect.left, tag_rect.bottom - page_rect.top + combo_rect.left, page_rect.right - page_rect.left - combo_rect.left, page_rect.bottom - page_rect.top - combo_rect.left };
    tag_page->setRect(tag_page_rect);
}


void cPoolPage::onSelectionChange(IControl& control, ISelectionIndex& index) {

    switch (control.id())
    {
    case IDC_POOL_COMBO:
        poolChange(index.selectedIndex());
        break;
    default:
        break;
    }
}


RECT cPoolPage::tagPageRect() {

    RECT page_rect = rect();
    RECT combo_rect = _poolComboBox.rect();
    RECT tag_rect = _poolTagTextField.rect();

    auto space = combo_rect.left - page_rect.left;

    return { space, tag_rect.bottom - page_rect.top + space, page_rect.right - page_rect.left - space, page_rect.bottom - page_rect.top - space };
}


const string cPoolPage::poolName(JavaClass& java, uint16_t index, const JavaClassPool* pool) {

    auto tag = pool->tag;
    auto tag_name = JavaHelperPoolTagName(tag);

    switch (tag) {

    case POOLTAG_UTF8:
    {
        auto name = string(pool->utf8_info.bytes, pool->utf8_info.length());
        return format("{}: \"{}\"", index, name);
    }

    case POOLTAG_INTEGER:
        return format("{}: {}: {}", index, pool->integer_info.bytes(), tag_name);

    case POOLTAG_FLOAT:
        return format("{}: {}: {}", index, pool->float_info.bytes(), tag_name);

    case POOLTAG_LONG:
        return format("{}: {}: {}", index, pool->long_info.bytes(), tag_name);

    case POOLTAG_DOUBLE:
        return format("{}: {}: {}", index, pool->double_info.bytes(), tag_name);

    case POOLTAG_CLASS:
    {
        auto text = java.nameAtIndex(pool->class_info.name_index());
        auto description = JavaTypeDescription(text);
        auto name = description.str();
        return format("{}: {}: {}", index, name, tag_name);
    }

    case POOLTAG_STRING:
    {
        auto name = java.nameAtIndex(pool->string_info.string_index());
        return format("{}: \"{}\": {}", index, name, tag_name);
    }

    case POOLTAG_FIELDREF:
    {
        auto name = JavaHelperFieldName(java, pool);
        return format("{}: {}", index, name);
    }

    case POOLTAG_METHODREF:
    {
        auto name = JavaHelperMethodName(java, pool);
        return format("{}: {}", index, name);
    }

    case POOLTAG_INTERFACEMETHODREF:
    {
        auto name = JavaHelperInterfaceName(java, pool);
        return format("{}: {}", index, name);
    }

    case POOLTAG_NAMEANDTYPE:
    {
        auto name = JavaHelperNameAndType(java, pool);
        return format("{}: {}", index, name);
    }

    case POOLTAG_METHODHANDLE:
    {
        auto kind = pool->method_handle.reference_kind;
        auto name = JavaHelperMethodKindName(kind);
        auto method = java.poolAtIndex(pool->method_handle.descriptor_index());
        switch (method->tag) {

        case POOLTAG_FIELDREF:
        {
            auto field_name = JavaHelperFieldName(java, method);
            return format("{}: {} {}", index, name, field_name);
        }

        case POOLTAG_METHODREF:
        {
            auto method_name = JavaHelperMethodName(java, method);
            return format("{}: {} {}", index, name, method_name);
        }

        case POOLTAG_INTERFACEMETHODREF:
        {
            auto interface_name = JavaHelperInterfaceName(java, method);
            return format("{}: {} {}", index, name, interface_name);
        }

        default:
            return format("?{}: {} {}", index, name, pool->method_handle.descriptor_index());
        }
    }

    case POOLTAG_METHODTYPE:
    {
        auto text = java.nameAtIndex(pool->method_type_info.descriptor_index());
        auto description = JavaTypeDescription(text);
        auto type_name = description.str();

        return format("{}: {}", index, type_name);
    }

    case POOLTAG_DYNAMIC:
    case POOLTAG_INVOKEDYNAMIC:
    {
        auto name_and_type = java.poolAtIndex(pool->dynamic_info.name_and_type_index());
        auto name = JavaHelperNameAndType(java, name_and_type);
        return format("{}: {} {}", index, tag_name, name);
    }

    case POOLTAG_MODULE:
    {
        auto name = java.nameAtIndex(pool->module_info.name_index());
        return format("{}: {}: {}", index, name, tag_name);
    }

    case POOLTAG_PACKAGE:
    {
        auto name = java.nameAtIndex(pool->package_info.name_index());
        return format("?{:5} {}: {}", index, name, tag_name);
    }

    default:

        return tag_name;
    }
}


void cPoolPage::parse() {

	if (_dataSource == nullptr) {
		return;
	}

	auto java = _dataSource->java();

    clear();

	auto iter = java.poolIterator();
	for (auto pool = iter.current(); pool != nullptr; pool = iter.next()) {

        auto name = poolName(java, iter.index() + 1, pool);
        auto wname = multibytetowstring(name, CP_UTF8);
        _poolComboBox.add(wname);
	}
}


void cPoolPage::clear() {

	_poolComboBox.clear();
	_poolTagTextField.clear();
    tagPageRemove();
}


void cPoolPage::tagPageRemove() {

    auto tag_page = _tagPage;
    _tagPage = nullptr;

    if (tag_page == nullptr) {
        return;
    }

    DestroyWindow(tag_page->handle());
    delete tag_page;
}


void cPoolPage::updatePool(JavaClass& java, const JavaClassPool* pool) {

    auto tag_name = JavaHelperPoolTagName(pool->tag);
    auto tag_name_string = format("{:<6} // {}", static_cast<uint8_t>(pool->tag), tag_name);
    _poolTagTextField.setText(tag_name_string);

   tagPageRemove();

    cWindow* page = nullptr;

    switch (pool->tag)
    {
    default:
        page = new cPoolUnknownPage(_module, this, _dataSource, _font, pool);
        break;
    }

    if (page == nullptr) {
        return;
    }

    _tagPage = page;
    RECT rect = tagPageRect();
    page->setRect(rect);
    page->setHidden(false);
}


void cPoolPage::poolChange(INT index) {

    if (_dataSource == nullptr) {
        return;
    }

    auto java = _dataSource->java();

    INT current = 0;
    auto iter = java.poolIterator();
    for (auto pool = iter.current(); pool != nullptr; pool = iter.next()) {
        if (current == index) {
            updatePool(java, pool);
            return;
        }
        current++;
    }
}