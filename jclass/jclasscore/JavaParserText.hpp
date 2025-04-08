//
//  JavaParserText.hpp
//  jclass
//
//  Created by DE4ME on 27.03.2025.
//

#ifndef JavaParserText_hpp
#define JavaParserText_hpp


#include <cstdint>
#include <stdio.h>
#include <sstream>
#include <format>


#include "JavaClassParser.hpp"
#include "JavaHelper.hpp"


using namespace std;


namespace java {


class JavaClassParserAttributes {
    
public:
    
    string vlaueName;
    string constantValue;
    string accessFlags;
    
    JavaClassParserAttributes()
    {
        
    }

    const string str() {
        
        auto name = JavaHelperAccessFlagsAndName(accessFlags, vlaueName);
        if (constantValue.empty()) {
            return name;
        } else {
            return format("{} = {}", name, constantValue);
        }
    }
    
    void clear() {
        
        vlaueName.clear();
        constantValue.clear();
        accessFlags.clear();
    }
    
    void parseField(const JavaClass& java, const JavaClassField* field) {
        
        auto field_name = java.nameAtIndex(field->name_index());
        auto value_desc = java.nameAtIndex(field->descriptor_index());
        auto value_type = JavaHelperTypeName(value_desc);
        vlaueName = format("{}: {}", field_name, value_type);
        accessFlags = JavaHelperAccessFlags(field->access_flag());
    }
    
    void parseMethod(const JavaClass& java, const JavaClassMethod* method) {
        
        auto field_name = java.nameAtIndex(method->name_index());
        auto value_desc = java.nameAtIndex(method->descriptor_index());
        auto value_type = JavaHelperMethodTypeName(value_desc);
        vlaueName = format("{} {}", field_name, value_type);
        accessFlags = JavaHelperAccessFlags(method->access_flag());
    }
    
    void parse(const JavaClass& java, const JavaClassAttribute* attr) {
        
        auto attr_tag = java.attrTagAtIndex(attr->name_index());
        switch (attr_tag) {
            case ATTRTAG_CONSTANTVALUE:
                constantValue = JavaHelperConstantValue(java, attr->constant_info.constantvalue_index());
                break;
            default:
                break;
        }
    }
};


class JavaParserText final: public JavaClassParser {
    
private:
    
    ostringstream _buffer;
    JavaClassParserAttributes _parseAttributes;
    vector<string> _innerClasses;
    
    void parseHeader(JavaClass &java) override;
    
    void parsePoolBegin(JavaClass &java) override;
    void parsePool(JavaClass &java, const JavaClassPool *pool, uint16_t index) override;
    
    void parseInterfaceBegin(JavaClass &java) override;
    void parseInterface(JavaClass &java, const JavaClassInterface *inte_face, uint16_t index) override;
    
    void parseFieldBegin(JavaClass &java) override;
    void parseField(JavaClass &java, const JavaClassField *field, uint16_t index) override;
    
    void parseFieldAttr(JavaClass &java, const JavaClassField *field, const JavaClassAttribute *attr, uint16_t index) override;
    void parseFieldAttrEnd(JavaClass &java, const JavaClassField *field, uint16_t index) override;
    
    void parseMethodBegin(JavaClass &java) override;
    void parseMethod(JavaClass &java, const JavaClassMethod *method, uint16_t index) override;
    
    void parseMethodAttr(JavaClass &java, const JavaClassMethod *method, const JavaClassAttribute *attr, uint16_t index) override;
    void parseMethodAttrEnd(JavaClass &java, const JavaClassMethod *method, uint16_t index) override;
    
    void parseAttrBegin(JavaClass &java) override;
    void parseAttr(JavaClass &java, const JavaClassAttribute *attr, uint16_t index) override;
    
    void parseEnd(JavaClass &java) override;
    
public:
    
    string str() { return _buffer.str(); }
    
};


} //namespace java


#endif /* JavaParserText_hpp */
