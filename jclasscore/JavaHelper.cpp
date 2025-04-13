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
#include "JavaTypeDescription.hpp"


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


struct JavaClassHelperType {
    
    TYPETAG tag;
    const char* desc;
};


const JavaClassHelperType type_helper[] =
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


struct JavaClassHelperAccessFlag {
    
    ACCFLAG flag;
    const char* name;
};


const JavaClassHelperAccessFlag access_flag_helper[] =
{
    {ACCFLAG_FINAL, "final"},
    {ACCFLAG_STATIC, "static"},
    {ACCFLAG_PUBLIC, "public"},
    {ACCFLAG_PRIVATE, "private"},
    {ACCFLAG_PROTECTED, "protected"},
    {ACCFLAG_SUPER, "super"},
    {ACCFLAG_INTERFACE, "interface"},
    {ACCFLAG_ABSTRACT, "abstract"},
    {ACCFLAG_SYNTHETIC, "synthetic"},
    {ACCFLAG_ANNOTATION, "annotation"},
    {ACCFLAG_ENUM, "enum"},
    {ACCFLAG_MODULE, "module"},
};


const JavaClassHelperAccessFlag access_flag_field_helper[] =
{
    {ACCFLAG_FINAL, "final"},
    {ACCFLAG_STATIC, "static"},
    {ACCFLAG_PUBLIC, "public"},
    {ACCFLAG_PRIVATE, "private"},
    {ACCFLAG_PROTECTED, "protected"},
    {ACCFLAG_VOLATILE, "volatile"},
    {ACCFLAG_TRANSIENT, "transient"},
    {ACCFLAG_SYNTHETIC, "synthetic"},
    {ACCFLAG_ENUM, "enum"},
};


const JavaClassHelperAccessFlag access_flag_method_helper[] =
{
    {ACCFLAG_FINAL, "final"},
    {ACCFLAG_STATIC, "static"},
    {ACCFLAG_PUBLIC, "public"},
    {ACCFLAG_PRIVATE, "private"},
    {ACCFLAG_PROTECTED, "protected"},
    {ACCFLAG_SYNCHRONIZED, "synchronized"},
    {ACCFLAG_BRIDGE, "bridge"},
    {ACCFLAG_VARARGS, "varargs"},
    {ACCFLAG_NATIVE, "native"},
    {ACCFLAG_ABSTRACT, "abstract"},
    {ACCFLAG_STRICT, "strict"},
    {ACCFLAG_SYNTHETIC, "synthetic"},
};


const JavaClassHelperAccessFlag access_flag_innerclass_helper[] =
{
    {ACCFLAG_FINAL, "final"},
    {ACCFLAG_STATIC, "static"},
    {ACCFLAG_PUBLIC, "public"},
    {ACCFLAG_PRIVATE, "private"},
    {ACCFLAG_PROTECTED, "protected"},
    {ACCFLAG_INTERFACE, "interface"},
    {ACCFLAG_ABSTRACT, "abstract"},
    {ACCFLAG_SYNTHETIC, "synthetic"},
    {ACCFLAG_ANNOTATION, "annotation"},
    {ACCFLAG_ENUM, "enum"},
};


const JavaClassHelperAccessFlag access_flag_methodparameter_helper[] =
{
    {ACCFLAG_FINAL, "final"},
    {ACCFLAG_SYNTHETIC, "synthetic"},
    {ACCFLAG_MANDATED, "mandated"},
};


struct JavaClassHelperVersion {
    
    uint8_t major_version;
    const char* date;
    const char* java_se;
};


const JavaClassHelperVersion version_helper[] =
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


vector<string> JavaHelperGetAccessFlags(const JavaClassHelperAccessFlag allflags[], size_t count, uint16_t flags) {
    
    vector<string> array;
    uint16_t unknown_flags = flags;
    for(size_t i = 0; i < count; i++) {
        if (allflags[i].flag & flags) {
            unknown_flags &= ~allflags[i].flag;
            array.push_back(allflags[i].name);
        }
    }
    if(unknown_flags != 0) {
        auto unknown_string = format("ACC_{:0>4x}", unknown_flags);
        array.push_back(unknown_string);
    }
    return array;
}


const string JavaHelperJoinWithSeparator(const vector<string>& array, const string& separator) {
    
    ostringstream buffer;
    bool add_separator = false;
    for(auto& item: array) {
        if (add_separator) {
            buffer << separator;
        }
        add_separator = true;
        buffer << item;
    }
    return buffer.str();
}


bool JavaHelperTypeTagValid(TYPETAG tag) {
    
    for(auto& item: type_helper) {
        if(item.tag == tag) {
            return true;
        }
    }
    return false;
}


const string JavaHelperVersion(const JavaClassVersion& version, bool java_se) {
    
    if (!java_se) {
        return format("{:2}.{:0>2}", version.major_version, version.minor_version);
    }
    for(auto& item: version_helper) {
        if (item.major_version == version.major_version) {
            return format("{} ({})", item.java_se, item.date);
        }
    }
    return "";
}


const string JavaHelperAccessFlags(ACCFLAG flags, const string& separator) {
    
    vector<string> array = JavaHelperGetAccessFlags(access_flag_helper, sizeof(access_flag_helper) / sizeof(access_flag_helper[0]), flags);
    return JavaHelperJoinWithSeparator(array, separator);
}


const string JavaHelperFieldAccessFlags(ACCFLAG flags, const string& separator) {
    
    vector<string> array = JavaHelperGetAccessFlags(access_flag_field_helper, sizeof(access_flag_field_helper) / sizeof(access_flag_field_helper[0]), flags);
    return JavaHelperJoinWithSeparator(array, separator);
}


const string JavaHelperMethodAccessFlags(ACCFLAG flags, const string& separator) {
    
    vector<string> array = JavaHelperGetAccessFlags(access_flag_method_helper, sizeof(access_flag_method_helper) / sizeof(access_flag_method_helper[0]), flags);
    return JavaHelperJoinWithSeparator(array, separator);
}


const string JavaHelperInnerClassAccessFlags(ACCFLAG flags, const string& separator) {
    
    vector<string> array = JavaHelperGetAccessFlags(access_flag_innerclass_helper, sizeof(access_flag_innerclass_helper) / sizeof(access_flag_innerclass_helper[0]), flags);
    return JavaHelperJoinWithSeparator(array, separator);
}


const string JavaHelperMethodParameterAccessFlags(ACCFLAG flags, const string& separator) {
    
    vector<string> array = JavaHelperGetAccessFlags(access_flag_methodparameter_helper, sizeof(access_flag_methodparameter_helper) / sizeof(access_flag_methodparameter_helper[0]), flags);
    return JavaHelperJoinWithSeparator(array, separator);
}


const string JavaHelperTypeTagName(TYPETAG tag) {
    
    for(auto& item: type_helper) {
        if(item.tag == tag) {
            return item.desc;
        }
    }
    return string(1, tag);
}


const string JavaHelperTypeNameArray(const string& name, uint8_t dimension) {
    
    if (dimension == 0) {
        return name;
    }
    auto name_prefix = string(dimension, '[');
    auto name_suffix = string(dimension, ']');
    return format("{}{}{}", name_prefix, name, name_suffix);
}


const string JavaHelperPoolTagName(POOLTAG tag) {
    
    auto index = tag < (sizeof(pool_helper) / sizeof(pool_helper[0])) ? tag : 0;
    return pool_helper[index];
}


const string JavaHelperNameAndType(const JavaClass& java, const JavaClassPool* pool) {
    
    auto name = java.nameAtIndex(pool->name_and_type_info.name_index());
    auto text = java.nameAtIndex(pool->name_and_type_info.descriptor_index());
    auto description = JavaTypeDescription(text);
    auto type_name = description.str();
    if(description.isMethod()) {
        return format("{} {}", name, type_name);
    } else {
        return format("{}: {}", name, type_name);
    }
}


const string JavaHelperMethodNameAndType(const JavaClass& java, const JavaClassPool* pool) {
    
    auto name = java.nameAtIndex(pool->name_and_type_info.name_index());
    auto text = java.nameAtIndex(pool->name_and_type_info.descriptor_index());
    auto description = JavaTypeDescription(text);
    auto type_name = description.str();
    return format("{} {}", name, type_name);
}


const string JavaHelperInterfaceNameAndType(const JavaClass& java, const JavaClassPool* pool) {
    
    auto name = java.nameAtIndex(pool->name_and_type_info.name_index());
    auto text = java.nameAtIndex(pool->name_and_type_info.descriptor_index());
    auto description = JavaTypeDescription(text);
    auto type_name = description.str();
    return format("{} {}", name, type_name);
}


const string JavaHelperMethodKindName(uint8_t kind) {
    
    auto index = kind < (sizeof(method_kind_helper) / sizeof(method_kind_helper[0])) ? kind : 0;
    return method_kind_helper[index];
}


const string JavaHelperFieldName(const JavaClass& java, const JavaClassPool* pool) {
    
    auto class_name = java.nameAtIndex(pool->field_info.class_index());
    auto name_and_type = java.poolAtIndex(pool->field_info.name_and_type_index());
    auto name = JavaHelperNameAndType(java, name_and_type);
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
            return to_string(pool->integer_info.bytes());
            
        case POOLTAG_FLOAT:
            return to_string(pool->float_info.bytes());
            
        case POOLTAG_LONG:
            return to_string(pool->long_info.bytes());
            
        case POOLTAG_DOUBLE:
            return to_string(pool->double_info.bytes());
            
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
    
    auto text = java.nameAtIndex(pool->class_info.name_index());
    auto description = JavaTypeDescription(text);
    return description.str();
}


const string JavaHelperGetInnerClassName(const JavaClass& java, const JavaClassInnerClassInfo* info) {
    
    auto inner_name = JavaHelperClassName(java, info->inner_class_info_index());
    auto flags = JavaHelperInnerClassAccessFlags(info->inner_class_access_flags());
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
