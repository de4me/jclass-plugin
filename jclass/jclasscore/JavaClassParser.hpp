//
//  JavaClassParser.hpp
//  jclass
//
//  Created by DE4ME on 27.03.2025.
//

#ifndef JavaClassParser_hpp
#define JavaClassParser_hpp


#include <stdio.h>


#include "JavaClass.hpp"


namespace java {


class JavaClassParser {
    
public:
    
    void parse(JavaClass& java);
    
    virtual void parseBegin(JavaClass& java) {;}
    virtual void parseHeader(JavaClass& java) {;}
    
    virtual void parsePoolBegin(JavaClass& java) {;}
    virtual void parsePool(JavaClass& java, const JavaClassPool* pool, uint16_t index) {;}
    virtual void parsePoolEnd(JavaClass& java) {;}
    
    virtual void parseInterfaceBegin(JavaClass& java) {;}
    virtual void parseInterface(JavaClass& java, const JavaClassInterface* inte_face, uint16_t index) {;}
    virtual void parseInterfaceEnd(JavaClass& java) {;}
    
    virtual void parseFieldBegin(JavaClass& java) {;}
    virtual void parseField(JavaClass& java, const JavaClassField* field, uint16_t index) {;}
    virtual void parseFieldEnd(JavaClass& java) {;}
    
    virtual void parseFieldAttrBegin(JavaClass& java, const JavaClassField* field, uint16_t index) {;}
    virtual void parseFieldAttr(JavaClass& java, const JavaClassField* field, const JavaClassAttribute* attr, uint16_t index) {;}
    virtual void parseFieldAttrEnd(JavaClass& java, const JavaClassField* field, uint16_t index) {;}
    
    virtual void parseMethodBegin(JavaClass& java) {;}
    virtual void parseMethod(JavaClass& java, const JavaClassMethod* method, uint16_t index) {;}
    virtual void parseMethodEnd(JavaClass& java) {;}
    
    virtual void parseMethodAttrBegin(JavaClass& java, const JavaClassMethod* method, uint16_t index) {;}
    virtual void parseMethodAttr(JavaClass& java, const JavaClassMethod* method, const JavaClassAttribute* attr, uint16_t index) {;}
    virtual void parseMethodAttrEnd(JavaClass& java, const JavaClassMethod* method, uint16_t index) {;}
    
    virtual void parseCodeAttrBegin(JavaClass& java, const JavaClassMethod* method, const JavaClassAttribute* method_attr, uint16_t index) {;}
    virtual void parseCodeAttr(JavaClass& java, const JavaClassMethod* method, const JavaClassAttribute* method_attr,  const JavaClassAttribute* attr, uint16_t index) {;}
    virtual void parseCodeAttrEnd(JavaClass& java, const JavaClassMethod* method, const JavaClassAttribute* method_attr, uint16_t index) {;}
    
    virtual void parseAttrBegin(JavaClass& java) {;}
    virtual void parseAttr(JavaClass& java, const JavaClassAttribute* attr, uint16_t index) {;}
    virtual void parseAttrEnd(JavaClass& java) {;}
    
    virtual void parseEnd(JavaClass& java) {;}
    
};


} //namespace java


#endif /* JavaClassParser_hpp */
