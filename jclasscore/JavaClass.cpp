//
//  JavaClass.cpp
//  jclass
//
//  Created by DE4ME on 25.03.2025.
//


#include <cstring>


#include "Error.hpp"
#include "JavaClass.hpp"


namespace java {


struct AttributeName {
    
    ATTRTAG tag;
    const char* name;
};


const AttributeName attribute_names[] = {
    
    {ATTRTAG_CONSTANTVALUE, "ConstantValue"},
    {ATTRTAG_CODE, "Code"},
    {ATTRTAG_STACKMAPTABLE, "StackMapTable"},
    {ATTRTAG_BOOTSTRAPMETHODS, "BootstrapMethods"},
    {ATTRTAG_NESTHOST, "NestHost"},
    {ATTRTAG_NESTMEMBERS, "NestMembers"},
    {ATTRTAG_PERMITTEDSUBCLASSES, "PermittedSubclasses"},
    {ATTRTAG_EXCEPTIONS, "Exceptions"},
    {ATTRTAG_INNERCLASSES, "InnerClasses"},
    {ATTRTAG_ENCLOSINGMETHOD, "EnclosingMethod"},
    {ATTRTAG_SYNTHETIC, "Synthetic"},
    {ATTRTAG_SIGNATURE, "Signature"},
    {ATTRTAG_RECORD, "Record"},
    {ATTRTAG_SOURCEFILE, "SourceFile"},
    {ATTRTAG_LINENUMBERTABLE, "LineNumberTable"},
    {ATTRTAG_LOCALVARIABLETABLE, "LocalVariableTable"},
    {ATTRTAG_LOCALVARIABLETYPETABLE, "LocalVariableTypeTable"},
    {ATTRTAG_SOURCEDEBUGEXTENSION, "SourceDebugExtension"},
    {ATTRTAG_DEPRECATED, "Deprecated"},
    {ATTRTAG_RUNTIMEVISIBLEANNOTATIONS, "RuntimeVisibleAnnotations"},
    {ATTRTAG_RUNTIMEINVISIBLEANNOTATIONS, "RuntimeInvisibleAnnotations"},
    {ATTRTAG_RUNTIMEVISIBLEPARAMETERANNOTATIONS, "RuntimeVisibleParameterAnnotations"},
    {ATTRTAG_RUNTIMEINVISIBLEPARAMETERANNOTATIONS, "RuntimeInvisibleParameterAnnotations"},
    {ATTRTAG_RUNTIMEVISIBLETYPEANNOTATIONS, "RuntimeVisibleTypeAnnotations"},
    {ATTRTAG_RUNTIMEINVISIBLETYPEANNOTATIONS, "RuntimeInvisibleTypeAnnotations"},
    {ATTRTAG_ANNOTATIONDEFAULT, "AnnotationDefault"},
    {ATTRTAG_METHODPARAMETERS, "MethodParameters"},
    {ATTRTAG_MODULE, "Module"},
    {ATTRTAG_MODULEPACKAGES, "ModulePackages"},
    {ATTRTAG_MODULEMAINCLASS, "ModuleMainClass"},
};


ATTRTAG JavaClass::attrTagAtIndex(uint16_t index) const {
    
    auto pool = poolAtIndex(index);
    if (pool == nullptr || pool->tag != POOLTAG_UTF8) {
        return ATTRTAG_UNKNOWN;
    }
    auto length = pool->utf8_info.length();
    auto name = pool->utf8_info.bytes;
    for(auto item: attribute_names) {
        if (strncmp(name, item.name, length) != 0) {
            continue;
        }
        if (item.name[length] != 0) {
            continue;
        }
        return item.tag;
    }
    return ATTRTAG_UNKNOWN;
}


uint32_t JavaClass::magic() const {
    
    const JavaClassHeader* header = reinterpret_cast<const JavaClassHeader*>(data());
    return header->magic();
}


JavaClassVersion JavaClass::version() const {
    
    const JavaClassHeader* header = reinterpret_cast<const JavaClassHeader*>(data());
    return header->version();
}


uint16_t JavaClass::poolCount() const {
    
    const uint16_t* pool = reinterpret_cast<const uint16_t*>(data() + sizeof(JavaClassHeader));
    uint16_t count = BYTESWAP16(*pool);
    return count > 1 ? count - 1 : 0;
}


ACCFLAG JavaClass::accessFlags() {
    
    const uint16_t* buffer = reinterpret_cast<const uint16_t*>(_poolIterator.cend());
    return static_cast<ACCFLAG>(BYTESWAP16(buffer[0]));
}


uint16_t JavaClass::thisClass() {
    
    const uint16_t* buffer = reinterpret_cast<const uint16_t*>(_poolIterator.cend());
    return BYTESWAP16(buffer[1]);
}


uint16_t JavaClass::superClass() {
    
    const uint16_t* buffer = reinterpret_cast<const uint16_t*>(_poolIterator.cend());
    return BYTESWAP16(buffer[2]);
}


uint16_t JavaClass::interfacesCount() {
    
    const uint16_t* buffer = reinterpret_cast<const uint16_t*>(_poolIterator.cend());
    return BYTESWAP16(buffer[3]);
}


const JavaClassInterface* JavaClass::interfaceFirst() {
    
    auto buffer = reinterpret_cast<const uint16_t*>(_poolIterator.cend());
    return reinterpret_cast<const JavaClassInterface*>(buffer + 4);
}


uint16_t JavaClass::fieldsCount() {
    
    auto buffer = reinterpret_cast<const uint16_t*>(_interfaceIterator.cend());
    return BYTESWAP16(*buffer);
}


const JavaClassField* JavaClass::fieldFirst() {
    
    auto buffer = reinterpret_cast<const uint16_t*>(_interfaceIterator.cend());
    return reinterpret_cast<const JavaClassField*>(buffer + 1);
}


uint16_t JavaClass::methodsCount() {
    
    auto buffer = reinterpret_cast<const uint16_t*>(_fieldIterator.cend());
    return BYTESWAP16(*buffer);
}


const JavaClassMethod* JavaClass::methodFirst() {
    
    auto buffer = reinterpret_cast<const uint16_t*>(_fieldIterator.cend());
    return reinterpret_cast<const JavaClassMethod*>(buffer + 1);
}


uint16_t JavaClass::attributesCount() {
    
    auto buffer = reinterpret_cast<const uint16_t*>(_methodIterator.cend());
    return BYTESWAP16(*buffer);
}


const JavaClassAttribute* JavaClass::attributeFirst() {
    
    auto buffer = reinterpret_cast<const uint16_t*>(_methodIterator.cend());
    return reinterpret_cast<const JavaClassAttribute*>(buffer + 1);
}


const JavaClassPool* JavaClass::poolFirst() {
    
    return reinterpret_cast<const JavaClassPool*>(data() + sizeof(JavaClassHeader) + sizeof(uint16_t));
}


const string JavaClass::nameAtIndex(uint16_t index) const {
    
    auto pool = poolAtIndex(index);
    if (pool == nullptr) {
        return "";
    }
    switch (pool->tag) {
            
        case POOLTAG_UTF8:
            return string(pool->utf8_info.bytes, pool->utf8_info.length());
            
        case POOLTAG_STRING:
            return nameAtIndex(pool->string_info.string_index());
            
        case POOLTAG_CLASS:
            return nameAtIndex(pool->class_info.name_index());
            
        case POOLTAG_METHODTYPE:
            return nameAtIndex(pool->method_type_info.descriptor_index());
            
        case POOLTAG_MODULE:
            return nameAtIndex(pool->module_info.name_index());
            
        case POOLTAG_PACKAGE:
            return nameAtIndex(pool->package_info.name_index());
            
        default:
            return "";
    }
}


const JavaClassPool* JavaClass::poolAtIndex(uint16_t index) const {
    
    if (index > 0) {
        auto iter = poolIterator();
        auto pool_index = index - 1;
        for(auto pool = iter.current(); pool != nullptr; pool = iter.next() ) {
            if (pool_index == iter.index()) {
                return pool;
            }
        }
    }
    return nullptr;
}


void JavaClass::read(istream& input, size_t length) {
    
    _isValid = false;
    _data.resize(length);
    input.read(_data.data(), _data.size());
    _poolIterator = PoolIterator(poolCount(), poolFirst());
    if (_poolIterator.cend() == nullptr) {
        throw Error("parse pool");
    }
    _interfaceIterator = InterfaceIterator(interfacesCount(), interfaceFirst());
    if (_interfaceIterator.cend() == nullptr) {
        throw Error("parse interfaces");
    }
    _fieldIterator = FieldIterator(fieldsCount(), fieldFirst());
    if (_fieldIterator.cend() == nullptr) {
        throw Error("parse fields");
    }
    _methodIterator = MethodIterator(methodsCount(), methodFirst());
    if (_methodIterator.cend() == nullptr) {
        throw Error("parse methods");
    }
    _attrIterator = AttributeIterator(attributesCount(), attributeFirst());
    if (_attrIterator.cend() == nullptr) {
        throw Error("parse attributes");
    }
    _isValid = true;
}


const JavaClassAttribute* JavaClass::attrubuteForTag(ATTRTAG tag) const {
    
    auto iter = attrIterator();
    for(auto attr = iter.current(); attr != nullptr; attr = iter.next()) {
        auto current = attrTagAtIndex(attr->name_index());
        if(current == tag) {
            return attr;
        }
    }
    return nullptr;
}


const string JavaClass::sourceFile() {
    
    auto attr = attrubuteForTag(ATTRTAG_SOURCEFILE);
    if(attr == nullptr) {
        return "";
    }
    return nameAtIndex(attr->sourcefile_info.sourcefile_index());
}


} //namespace java
