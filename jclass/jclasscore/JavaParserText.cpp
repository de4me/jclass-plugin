//
//  JavaParserText.cpp
//  jclass
//
//  Created by DE4ME on 27.03.2025.
//

#include "JavaParserText.hpp"


namespace java {


void JavaParserText::parseHeader(JavaClass &java) {
    
    auto version = java.version();
    _buffer << format("{:26} : {:x}\r\n", "Magic", java.magic());
    _buffer << format("{:26} : {}\r\n", "Java Version", JavaHelperVersion(version));
    auto source_file = java.sourceFile();
    if( !source_file.empty() ){
        _buffer << format("{:26} : {}\r\n", "Source File", source_file);
    }
    auto access_flags = JavaHelperClassAccessFlags(java.accessFlags());
    _buffer << format("{:26} : {}\r\n", "Access Flags", access_flags);
    auto this_name = java.nameAtIndex(java.thisClass());
    _buffer << format("{:26} : {}\r\n", "This Class", this_name);
    auto super_name = java.nameAtIndex(java.superClass());
    _buffer << format("{:26} : {}\r\n", "Super Class", super_name);
    _buffer << format("{:26} : {}\r\n", "Pool Count", java.poolCount());
    _buffer << format("{:26} : {}\r\n", "Interfaces Count", java.interfacesCount());
    _buffer << format("{:26} : {}\r\n", "Fields Count", java.fieldsCount());
    _buffer << format("{:26} : {}\r\n", "Methods Count", java.methodsCount());
    _buffer << format("{:26} : {}\r\n", "Attributes Count", java.attributesCount());
}


void JavaParserText::parsePoolBegin(JavaClass &java) {
    
    _buffer << "\r\nPool\r\n\r\n";
}


void JavaParserText::parsePool(JavaClass &java, const JavaClassPool *pool, uint16_t index) {
    
    index++;
    auto tag = pool->tag;
    auto tag_name = JavaHelperPoolTagName(tag);
//    _buffer << tag_name << " ";
    switch (tag) {
            
        case POOLTAG_UTF8:
        {
            auto name = string(pool->utf8_info.bytes, pool->utf8_info.length());
            _buffer << format("{:5}: \"{}\"\r\n", index, name);
            break;
        }
            
        case POOLTAG_INTEGER:
            _buffer << format("{:5}: {}: {}\r\n", index, pool->integer_info.bytes(), tag_name);
            break;
            
        case POOLTAG_FLOAT:
            _buffer << format("{:5}: {}: {}\r\n", index, pool->float_info.bytes(), tag_name);
            break;
            
        case POOLTAG_LONG:
            _buffer << format("{:5}: {}: {}\r\n", index, pool->long_info.bytes(), tag_name);
            break;
            
        case POOLTAG_DOUBLE:
            _buffer << format("{:5}: {}: {}\r\n", index, pool->double_info.bytes(), tag_name);
            break;
            
        case POOLTAG_CLASS:
        {
            auto desc = java.nameAtIndex(pool->class_info.name_index());
            auto name = JavaHelperTypeName(desc);
            _buffer << format("{:5}: {}: {}\r\n", index, name, tag_name);
            break;
        }
            
        case POOLTAG_STRING:
        {
            auto name = java.nameAtIndex(pool->string_info.string_index());
            _buffer << format("{:5}: \"{}\": {}\r\n", index, name, tag_name);
            break;
        }
            
        case POOLTAG_FIELDREF:
        {
            auto name = JavaHelperFieldName(java, pool);
            _buffer << format("{:5}: {}\r\n", index, name);
            break;
        }
            
        case POOLTAG_METHODREF:
        {
            auto name = JavaHelperMethodName(java, pool);
            _buffer << format("{:5}: {}\r\n", index, name);
            break;
        }
            
        case POOLTAG_INTERFACEMETHODREF:
        {
            auto name = JavaHelperInterfaceName(java, pool);
            _buffer << format("{:5}: {}\r\n", index, name);
            break;
        }
            
        case POOLTAG_NAMEANDTYPE:
        {
            auto name = JavaHelperFieldNameAndType(java, pool);
            _buffer << format("{:5}: {}\r\n", index, name);
            break;
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
                    _buffer << format("{:5}: {} {}\r\n", index, name, field_name);
                    break;
                }
                    
                case POOLTAG_METHODREF:
                {
                    auto method_name = JavaHelperMethodName(java, method);
                    _buffer << format("{:5}: {} {}\r\n", index, name, method_name);
                    break;
                }
                    
                case POOLTAG_INTERFACEMETHODREF:
                {
                    auto interface_name = JavaHelperInterfaceName(java, method);
                    _buffer << format("{:5}: {} {}\r\n", index, name, interface_name);
                    break;
                }
                    
                default:
                    _buffer << format("?{:5}: {} {}\r\n", index, name, pool->method_handle.descriptor_index());
                    break;
            }
            break;
        }
            
        case POOLTAG_METHODTYPE:
        {
            auto desc = java.nameAtIndex(pool->method_type_info.descriptor_index());
            auto type_name = JavaHelperMethodTypeName(desc);
            _buffer << format("{:5}: {}\r\n", index, type_name);
            break;
        }
            
        case POOLTAG_DYNAMIC:
        case POOLTAG_INVOKEDYNAMIC:
        {
            auto name_and_type = java.poolAtIndex(pool->dynamic_info.name_and_type_index());
            auto name = JavaHelperFieldNameAndType(java, name_and_type);
            _buffer << format("{:5}: {} {}\r\n", index, tag_name, name);
            break;
        }
            
        case POOLTAG_MODULE:
        {
            auto name = java.nameAtIndex(pool->module_info.name_index());
            _buffer << format("{:5}: {}: {}\r\n", index, name, tag_name);
            break;
        }
            
        case POOLTAG_PACKAGE:
        {
            auto name = java.nameAtIndex(pool->package_info.name_index());
            _buffer << format("?{:5} {}: {}\r\n", index, name, tag_name);
            break;
        }
            
        default:
            break;
    }
}


void JavaParserText::parseInterfaceBegin(JavaClass &java) {
    
    _buffer << "\r\nInterfaces\r\n\r\n";
}


void JavaParserText::parseInterface(JavaClass &java, const JavaClassInterface *inte_face, uint16_t index) {
    
    auto name = java.nameAtIndex(inte_face->index());
    _buffer << format("{:5}: {}\r\n", index, name);
}


void JavaParserText::parseFieldBegin(JavaClass &java) {

    _buffer << "\r\nFields\r\n\r\n";
}


void JavaParserText::parseField(JavaClass &java, const JavaClassField *field, uint16_t index) {
    
    _parseAttributes.clear();
    _parseAttributes.parseField(java, field);
}


void JavaParserText::parseFieldAttr(JavaClass &java, const JavaClassField *field, const JavaClassAttribute *attr, uint16_t index) {
    
    _parseAttributes.parse(java, attr);
}


void JavaParserText::parseFieldAttrEnd(JavaClass &java, const JavaClassField *field, uint16_t index) {
    
    _buffer << format("{:5}: {}\r\n", index, _parseAttributes.str());
}


void JavaParserText::parseMethodBegin(JavaClass &java) {
    
    _buffer << "\r\nMethods\r\n\r\n";
}


void JavaParserText::parseMethod(JavaClass &java, const JavaClassMethod *method, uint16_t index) {
    
    _parseAttributes.clear();
    _parseAttributes.parseMethod(java, method);
}


void JavaParserText::parseMethodAttr(JavaClass &java, const JavaClassMethod *method, const JavaClassAttribute *attr, uint16_t index) {
    
    _parseAttributes.parse(java, attr);
}


void JavaParserText::parseMethodAttrEnd(JavaClass &java, const JavaClassMethod *method, uint16_t index) {
    
    _buffer << format("{:5}: {}\r\n", index, _parseAttributes.str());
}


void JavaParserText::parseAttrBegin(JavaClass &java) {
    
    _buffer << "\r\nAttributes\r\n\r\n";
}


void JavaParserText::parseAttr(JavaClass &java, const JavaClassAttribute *attr, uint16_t index) {
    
    auto attr_name = java.nameAtIndex(attr->name_index());
    auto attrid = java.attrTagAtIndex(attr->name_index());
    switch (attrid) {
            
        case ATTRTAG_SOURCEFILE:
        {
            auto source_file = java.nameAtIndex(attr->sourcefile_info.sourcefile_index());
            _buffer << format("{:5}: {}: {}\r\n", index, source_file, attr_name);
            break;
        }
            
        case ATTRTAG_INNERCLASSES:
        {
            auto iter = attr->innerclasses_info.infoIterator();
            for(auto info = iter.cbegin(); info != nullptr; info = iter.next()) {
                auto name = JavaHelperGetInnerClassName(java, info);
                _innerClasses.push_back(name);
            }
            _buffer << format("{:5}: {}\r\n", index, attr_name);
            break;
        }
            
        case ATTRTAG_SIGNATURE:
        {
            auto signature = java.nameAtIndex(attr->signature_info.signature_index());
            _buffer << format("{:5}: {}: {}\r\n", index, signature, attr_name);
            break;
        }
            
        default:
            _buffer << format("{:5}: {}\r\n", index, attr_name);
            break;
    }
    
}


void JavaParserText::parseEnd(JavaClass &java) {
    
    if(!_innerClasses.empty()) {
        _buffer << "\r\nInner Classes\r\n\r\n";
        int index = 0;
        for(auto item: _innerClasses) {
            _buffer << format("{:5} {}\r\n", index, item);
            index++;
        }
    }
}


} //namespace java
