//
//  JavaClassParser.cpp
//  jclass
//
//  Created by DE4ME on 27.03.2025.
//


#include "JavaClassParser.hpp"
#include "Error.hpp"


namespace java {


void JavaClassParser::parse(JavaClass& java) {
    
    parseBegin(java);
    
    parseHeader(java);
    
    parsePoolBegin(java);
    
    auto pool_iter = java.poolIterator();
    for(auto obj = pool_iter.current(); obj != nullptr; obj = pool_iter.next()) {
        parsePool(java, obj, pool_iter.index());
    }
    
    parsePoolEnd(java);
    
    parseInterfaceBegin(java);
    
    auto interf_iter = java.interfaceIterator();
    for(auto obj = interf_iter.current(); obj != nullptr; obj = interf_iter.next() ) {
        parseInterface(java, obj, interf_iter.index());
    }
    
    parseInterfaceEnd(java);
    
    parseFieldBegin(java);
    
    auto field_iter = java.fieldIterator();
    for(auto field = field_iter.current(); field != nullptr; field = field_iter.next() ) {
        
        parseField(java, field, field_iter.index());
        
        parseFieldAttrBegin(java, field, field_iter.index());
        
        auto attr_iter = field->attributeIterator();
        for(auto field_attr = attr_iter.current(); field_attr != nullptr; field_attr = attr_iter.next()) {
            
            parseFieldAttr(java, field, field_attr, attr_iter.index());
        }
        
        parseFieldAttrEnd(java, field, field_iter.index());
    }
    
    parseFieldEnd(java);
    
    parseMethodBegin(java);
    
    auto method_iter = java.methodIterator();
    for(auto method = method_iter.current(); method != nullptr; method = method_iter.next() ) {
        
        parseMethod(java, method, method_iter.index());
        
        parseMethodAttrBegin(java, method, method_iter.index());
        
        auto method_attr_iter = method->attributeIterator();
        for(auto method_attr = method_attr_iter.current(); method_attr != nullptr; method_attr = method_attr_iter.next()) {
            
            parseMethodAttr(java, method, method_attr, method_attr_iter.index());
            
            ATTRTAG tag = java.attrTagAtIndex(method_attr->name_index());
            switch (tag) {
                case ATTRTAG_CODE:
                {
                    parseCodeAttrBegin(java, method, method_attr, method_iter.index());
                    
                    auto code_attr_iter = method_attr->code_info.attributeIterator();
                    for(auto code_attr = code_attr_iter.current(); code_attr != nullptr; code_attr = code_attr_iter.next()) {
                        
                        parseCodeAttr(java, method, method_attr, code_attr, code_attr_iter.index());
                    }
                    
                    parseCodeAttrEnd(java, method, method_attr, method_iter.index());
                    break;
                }
                default:
                    break;
            }
        }
        
        parseMethodAttrEnd(java, method, method_iter.index());
    }
    
    parseMethodEnd(java);
    
    parseAttrBegin(java);
    
    auto attr_iter = java.attrIterator();
    for(auto attr = attr_iter.current(); attr != nullptr; attr = attr_iter.next()) {
        parseAttr(java, attr, attr_iter.index());
    }
    
    parseAttrEnd(java);
    
    parseEnd(java);
}


} //namespace java
