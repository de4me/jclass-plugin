//
//  JavaStruct.cpp
//  jclass
//
//  Created by DE4ME on 26.03.2025.
//


#include <stdio.h>


#include "JavaStruct.hpp"
#include "JavaClass.hpp"


namespace java {


AttributeIterator JavaClassField::attributeIterator() const {
    
    return AttributeIterator(attributes_count(), &attribute[0]);
}


size_t JavaClassField::sizeOfStruct() const {
    
    auto iter = attributeIterator();
    auto begin_ptr = reinterpret_cast<const char*>(iter.cbegin());
    auto end_ptr = reinterpret_cast<const char*>(iter.cend());
    return sizeof(JavaClassField) + (end_ptr - begin_ptr);
}


AttributeIterator JavaClassAttributeCode::attributeIterator() const {
    
    return AttributeIterator(attrubutes_count(),  attributeBegin());
}


InnerClassInfoIterator JavaClassAttributeInnerClasses::infoIterator() const {
    
    return InnerClassInfoIterator(number_of_classes(), infoBegin());
}


} //namespace java
