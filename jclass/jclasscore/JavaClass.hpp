//
//  JavaClass.hpp
//  jclass
//
//  Created by DE4ME on 25.03.2025.
//

#ifndef JavaClass_hpp
#define JavaClass_hpp


#include <cstdint>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>


#include "JavaIterator.hpp"
#include "JavaStruct.hpp"


using namespace std;


namespace java {


class JavaClass {

  private:
    
    vector<char> _data;

    PoolIterator _poolIterator;
    InterfaceIterator _interfaceIterator;
    FieldIterator _fieldIterator;
    MethodIterator _methodIterator;
    AttributeIterator _attrIterator;

    bool _isValid;
    
    const JavaClassPool *poolFirst();
    const JavaClassInterface *interfaceFirst();
    const JavaClassField *fieldFirst();
    const JavaClassMethod *methodFirst();
    const JavaClassAttribute *attributeFirst();

  public:
    
    JavaClass() :
    _isValid(false)
    {
        
    }

    const char *data() const { return _data.data(); }
    size_t dataLength() const { return _data.size(); }

    uint32_t magic() const;
    JavaClassVersion version() const;
    uint16_t poolCount() const;
    CLASSACCESSFLAG accessFlags();
    uint16_t thisClass();
    uint16_t superClass();
    uint16_t interfacesCount();
    uint16_t fieldsCount();
    uint16_t methodsCount();
    uint16_t attributesCount();
    const string sourceFile();

    bool isValid() const { return _isValid; }

    const string nameAtIndex(uint16_t index) const;
    const JavaClassPool *poolAtIndex(uint16_t index) const;
    ATTRTAG attrTagAtIndex(uint16_t index) const;
    const JavaClassAttribute* attrubuteForTag(ATTRTAG tag) const;

    const PoolIterator poolIterator() const { return _poolIterator; }
    auto interfaceIterator() const { return _interfaceIterator; }
    auto fieldIterator() const { return _fieldIterator; }
    auto methodIterator() const { return _methodIterator; }
    auto attrIterator() const { return _attrIterator; }

    void read(istream &input, size_t length);
};


} // namespace java


#endif /* JavaClass_hpp */
