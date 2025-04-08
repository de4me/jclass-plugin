//
//  JavaHelper.cpp
//  jclass
//
//  Created by DE4ME on 27.03.2025.
//


#include "JavaHelper.hpp"


#include <format>
#include <sstream>


#include "JavaStruct.hpp"
#include "JavaClass.hpp"


namespace java {


using namespace std;


const char* pool_helper[] = {
    "", //0
    "Utf8", //1
    "", //2
    "Integer", //3
    "Float", //4
    "Long", //5
    "Double", //6
    "Class", //7
    "String", //8
    "Fieldref", //9
    "Methodref", //10
    "InterfaceMethodref", //11
    "NameAndType", //12
    "", //13
    "", //14
    "MethodHandle", //15
    "MethodType", //16
    "Dynamic", //17
    "InvokeDynamic", //18
    "Module", //19
    "Package", //20
};


const char* method_kind_helper[] = {
    "",
    "getField",
    "getStatic",
    "putField",
    "putStatic",
    "invokeVirtual",
    "invokeStatic",
    "invokeSpecial",
    "newInvokeSpecial",
    "invokeInterface",
};


struct JavaClassTypeHelper {
    TYPETAG tag;
    const char* desc;
};


const JavaClassTypeHelper type_helper[] =
{
    {TYPETAG_BYTE, "byte"},
    {TYPETAG_CHAR, "char"},
    {TYPETAG_DOUBLE, "double"},
    {TYPETAG_FLOAT, "float"},
    {TYPETAG_INTEGER, "int"},
    {TYPETAG_LONG, "long"},
    {TYPETAG_SHORT, "short"},
    {TYPETAG_BOOL, "boolean"},
    {TYPETAG_VOID, "void"},
    {TYPETAG_STRING, "string"},
    {TYPETAG_ENUM, "enum"},
    {TYPETAG_CLASS, "class"},
    {TYPETAG_ARRAY, "array"},
};


struct JavaClassHelperAccessFlags {
    uint64_t flag;
    const char* name;
};


const JavaClassHelperAccessFlags access_type_helper[] =
{
    {ACCESSFLAG_FINAL, "final"},
    {ACCESSFLAG_STATIC, "static"},
    {ACCESSFLAG_PUBLIC, "public"},
    {ACCESSFLAG_PRIVATE, "private"},
    {ACCESSFLAG_PROTECTED, "protected"},
    {ACCESSFLAG_SYNCHRONIZED, "synchronized"},
    {ACCESSFLAG_BRIDGE, "bridge"},
    {ACCESSFLAG_VARARGS, "varargs"},
    {ACCESSFLAG_NATIVE, "native"},
    {ACCESSFLAG_ABSTRACT, "abstract"},
    {ACCESSFLAG_STRICT, "strict"},
    {ACCESSFLAG_SYNTHETIC, "synthetic"},
};


const JavaClassHelperAccessFlags class_access_type_helper[] =
{
    {CLASSACCESSFLAG_FINAL, "final"},
    {CLASSACCESSFLAG_STATIC, "static"},
    {CLASSACCESSFLAG_PUBLIC, "public"},
    {CLASSACCESSFLAG_PRIVATE, "private"},
    {CLASSACCESSFLAG_PROTECTED, "protected"},
    {CLASSACCESSFLAG_SUPER, "super"},
    {CLASSACCESSFLAG_INTERFACE, "interface"},
    {CLASSACCESSFLAG_ABSTRACT, "abstract"},
    {CLASSACCESSFLAG_SYNTHETIC, "synthetic"},
    {CLASSACCESSFLAG_ANNOTATION, "annotation"},
    {CLASSACCESSFLAG_ENUM, "enum"},
    {CLASSACCESSFLAG_MODULE, "module"},
};


struct JavaClassVersionHelper {
    uint8_t major_version;
    const char* date;
    const char* java_se;
};


const JavaClassVersionHelper version_helper[] =
{
    {45, "May 1996/February 1997", "1.0.2/1.1"},
    {46, "December 1998", "1.2"},
    {47, "May 2000", "1.3"},
    {48, "February 2002", "1.4"},
    {49, "September 2004", "5.0"},
    {50, "December 2006", "6"},
    {51, "July 2011", "7"},
    {52, "March 2014", "8"},
    {53, "September 2017", "9"},
    {54, "March 2018", "10"},
    {55, "September 2018", "11"},
    {56, "March 2019", "12"},
    {57, "September 2019", "13"},
    {58, "March 2020", "14"},
    {59, "September 2020", "15"},
    {60, "March 2021", "16"},
    {61, "September 2021", "17"},
    {62, "March 2022", "18"},
    {63, "September 2022", "19"},
    {64, "March 2023", "20"},
    {65, "September 2023", "21"},
};


const string join_with_separator(const vector<string>& array, string separator) {
    
    ostringstream buffer;
    bool add_separator = false;
    for(auto item: array) {
        if (add_separator) {
            buffer << separator;
        }
        add_separator = true;
        buffer << item;
    }
    return buffer.str();
}


const vector<string> array_with_separator(const string& text, char separator, size_t pos = 0, bool keep_separator = false) {
    
    auto current = pos;
    auto count = text.length();
    vector<string> array;
    for(auto i = pos; i < count; i++) {
        if (text[i] != separator) {
            continue;
        }
        auto count = keep_separator ? i - current + 1 : i - current;
        auto substring = text.substr(current, count);
        array.push_back(substring);
        current = i + 1;
    }
    if (count == current) {
        return array;
    }
    array.push_back(text.substr(current, count - current));
    return array;
}


bool JavaHelperTypeTagValid(TYPETAG tag) {
    
    for(auto item: type_helper) {
        if(item.tag == tag) {
            return true;
        }
    }
    return false;
}


const string JavaHelperVersion(JavaClassVersion& version, bool java_se) {
    
    if (!java_se) {
        return format("{:2}.{:0<2}", version.major_version, version.minor_version);
    }
    for(auto item: version_helper) {
        if (item.major_version == version.major_version) {
            return format("{} ({})", item.java_se, item.date);
        }
    }
    return "";
}


const string JavaHelperClassAccessFlags(CLASSACCESSFLAG flags, string separator) {
    
    vector<string> array;
    for(auto item: class_access_type_helper) {
        if (item.flag & flags) {
            array.push_back(item.name);
        }
    }
    return join_with_separator(array, separator);
}


const string JavaHelperAccessFlags(ACCESSFLAG flags, string separator) {
    
    vector<string> array;
    for(auto item: access_type_helper) {
        if (item.flag & flags) {
            array.push_back(item.name);
        }
    }
    return join_with_separator(array, separator);
}


const string JavaHelperTypeTagName(TYPETAG tag) {
    
    for(auto item: type_helper) {
        if(item.tag == tag) {
            return item.desc;
        }
    }
    return string(tag, 1);
}


const vector<string> JavaHelperArrayTypeNames(const vector<string>& array) {
    
    vector<string> result;
    for(auto item: array) {
        auto name = JavaHelperTypeName(item);
        result.push_back(name);
    }
    return result;
}


const string JavaHelperMethodTypeName(const string& desc) {
    
    if (desc.empty()) {
        return desc;
    }
    if (desc[0] != '(') {
        return desc;
    }
    const vector<string> array = array_with_separator(desc, ')', 1);
    const string return_type_name = "";
    switch (array.size()) {
        case 1:
        {
            auto params = array_with_separator(array[0], ';', 0, true);
            auto params_types = JavaHelperArrayTypeNames(params);
            auto params_string = join_with_separator(params_types, ", ");
            return format("({})", params_string);
        }
        case 2:
        {
            auto return_type = JavaHelperTypeName(array[1]);
            auto params = array_with_separator(array[0], ';', 0, true);
            auto params_types = JavaHelperArrayTypeNames(params);
            auto params_string = join_with_separator(params_types, ", ");
            return format("({}) -> {}", params_string, return_type);
        }
        default:
            return desc;
    }
}


const string JavaHelperTypeNameArray(const string& name, uint8_t dimension) {
    
    if (dimension == 0) {
        return name;
    }
    auto name_prefix = string(dimension, '[');
    auto name_suffix = string(dimension, ']');
    return format("{}{}{}", name_prefix, name, name_suffix);
}


const string JavaHelperTypeName(const string& desc) {
    
    if (desc.empty()) {
        return desc;
    }
    int index = 0;
    vector<string> array;
    int array_dimension = 0;
    bool reference = false;
    int reference_index = 0;
    for(auto ch: desc) {
        if (reference) {
            if (ch == ';') {
                auto name = desc.substr(reference_index, index - reference_index);
                if (array_dimension > 0) {
                    auto name_array = JavaHelperTypeNameArray(name, array_dimension);
                    array.push_back(name_array);
                    array_dimension = 0;
                } else {
                    array.push_back(name);
                }
                reference = false;
            }
        } else {
            switch (ch) {
                case TYPETAG_ARRAY:
                    array_dimension++;
                    break;
                    
                case TYPETAG_REFERENCE:
                    reference_index = index + 1;
                    reference = true;
                    break;
                    
                default:
                {
                    TYPETAG tag = static_cast<TYPETAG>(ch);
                    if (!JavaHelperTypeTagValid(tag) ) {
                        return desc;
                    }
                    auto name = JavaHelperTypeTagName(tag);
                    if (array_dimension > 0) {
                        auto name_array = JavaHelperTypeNameArray(name, array_dimension);
                        array.push_back(name_array);
                        array_dimension = 0;
                    } else {
                        array.push_back(name);
                    }
                    break;
                }
            }
        }
        index++;
    }
    return join_with_separator(array, ", ");
}


const string JavaHelperPoolTagName(POOLTAG tag) {
    
    auto index = tag < (sizeof(pool_helper) / sizeof(pool_helper[0])) ? tag : 0;
    return pool_helper[index];
}


const string JavaHelperFieldNameAndType(const JavaClass& java, const JavaClassPool* pool) {
    
    auto name = java.nameAtIndex(pool->name_and_type_info.name_index());
    auto desc = java.nameAtIndex(pool->name_and_type_info.descriptor_index());
    if (desc[0] == '(') {
        auto type = JavaHelperMethodTypeName(desc);
        return format("{} {}", name, type);
    } else {
        auto type = JavaHelperTypeName(desc);
        return format("{}: {}", name, type);
    }
}


const string JavaHelperMethodNameAndType(const JavaClass& java, const JavaClassPool* pool) {
    
    auto name = java.nameAtIndex(pool->name_and_type_info.name_index());
    auto desc = java.nameAtIndex(pool->name_and_type_info.descriptor_index());
    auto type = JavaHelperMethodTypeName(desc);
    return format("{} {}", name, type);
}


const string JavaHelperInterfaceNameAndType(const JavaClass& java, const JavaClassPool* pool) {
    
    auto name = java.nameAtIndex(pool->name_and_type_info.name_index());
    auto desc = java.nameAtIndex(pool->name_and_type_info.descriptor_index());
    auto type = JavaHelperMethodTypeName(desc);
    return format("{} {}", name, type);
}


const string JavaHelperMethodKindName(uint8_t kind) {
    
    auto index = kind < (sizeof(method_kind_helper) / sizeof(method_kind_helper[0])) ? kind : 0;
    return method_kind_helper[index];
}


const string JavaHelperFieldName(const JavaClass& java, const JavaClassPool* pool) {
    
    auto class_name = java.nameAtIndex(pool->field_info.class_index());
    auto name_and_type = java.poolAtIndex(pool->field_info.name_and_type_index());
    auto name = JavaHelperFieldNameAndType(java, name_and_type);
    return format("{}::{}", class_name, name);
}


const string JavaHelperMethodName(const JavaClass& java, const JavaClassPool* pool) {
    
    auto class_name = java.nameAtIndex(pool->method_info.class_index());
    auto name_and_type = java.poolAtIndex(pool->method_info.name_and_type_index());
    auto name = JavaHelperMethodNameAndType(java, name_and_type);
    return format("{}::{}", class_name, name);
}


const string JavaHelperInterfaceName(const JavaClass& java, const JavaClassPool* pool) {
    
    auto class_name = java.nameAtIndex(pool->interface_method_info.class_index());
    auto name_and_type = java.poolAtIndex(pool->interface_method_info.name_and_type_index());
    auto name = JavaHelperInterfaceNameAndType(java, name_and_type);
    return format("{}::{}", class_name, name);
}


const string JavaHelperConstantValue(const JavaClass& java, uint16_t index) {
    
    auto pool = java.poolAtIndex(index);
    return pool != nullptr ? JavaHelperConstantValue(java, pool) : "";
}


const string JavaHelperConstantValue(const JavaClass& java, const JavaClassPool* pool) {
    
    switch (pool->tag) {
        case POOLTAG_INTEGER:
            return format("{}", pool->integer_info.bytes());
            
        case POOLTAG_FLOAT:
            return format("{}", pool->float_info.bytes());
            
        case POOLTAG_LONG:
            return format("{}", pool->long_info.bytes());
            
        case POOLTAG_DOUBLE:
            return format("{}", pool->double_info.bytes());
            
        case POOLTAG_STRING:
        {
            auto text = java.nameAtIndex(pool->string_info.string_index());
            return format("\"{}\"", text);
        }
        default:
            return "";
    }
}


const string JavaHelperClassName(const JavaClass& java, uint16_t index) {
    
    auto pool = java.poolAtIndex(index);
    return pool != nullptr ? JavaHelperClassName(java, pool) : "";
}


const string JavaHelperClassName(const JavaClass& java, const JavaClassPool* pool) {
    
    auto desc = java.nameAtIndex(pool->class_info.name_index());
    return JavaHelperTypeName(desc);
}


const string JavaHelperGetInnerClassName(const JavaClass& java, const JavaClassInnerClassInfo* info) {
    
    auto inner_name = JavaHelperClassName(java, info->inner_class_info_index());
    auto flags = JavaHelperClassAccessFlags(info->inner_class_access_flags());
    return JavaHelperAccessFlagsAndName(flags, inner_name);
}


const string JavaHelperAccessFlagsAndName(const string flags, const string name) {
    
    if(flags.empty()){
        return name;
    } else {
        return format("{} {}", flags, name);
    }
}


} //namespace java
