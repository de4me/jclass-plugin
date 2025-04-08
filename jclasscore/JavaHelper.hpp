//
//  JavaHelper.hpp
//  jclass
//
//  Created by DE4ME on 27.03.2025.
//

#ifndef JavaHelper_hpp
#define JavaHelper_hpp


#include <stdio.h>
#include <string>


#include "JavaStruct.hpp"


using namespace std;


namespace java {


class JavaClass;


const string JavaHelperPoolTagName(POOLTAG tag);
const string JavaHelperTypeTagName(TYPETAG tag);
const string JavaHelperAccessFlags(ACCESSFLAG flags, string separator = " ");
const string JavaHelperClassAccessFlags(CLASSACCESSFLAG flags, string separator = " ");
const string JavaHelperVersion(JavaClassVersion& version, bool java_se = false);

const string JavaHelperTypeName(const string& desc);
const string JavaHelperMethodTypeName(const string& desc);
const string JavaHelperFieldNameAndType(const JavaClass& java, const JavaClassPool* pool);
const string JavaHelperMethodNameAndType(const JavaClass& java, const JavaClassPool* pool);
const string JavaHelperInterfaceNameAndType(const JavaClass& java, const JavaClassPool* pool);
const string JavaHelperMethodKindName(uint8_t hind);
const string JavaHelperFieldName(const JavaClass& java, const JavaClassPool* pool);
const string JavaHelperMethodName(const JavaClass& java, const JavaClassPool* pool);
const string JavaHelperInterfaceName(const JavaClass& java, const JavaClassPool* pool);

const string JavaHelperClassName(const JavaClass& java, const JavaClassPool* pool);
const string JavaHelperClassName(const JavaClass& java, uint16_t index);
const string JavaHelperConstantValue(const JavaClass& java, const JavaClassPool* pool);
const string JavaHelperConstantValue(const JavaClass& java, uint16_t index);
const string JavaHelperGetInnerClassName(const JavaClass& java, const JavaClassInnerClassInfo* info);
const string JavaHelperAccessFlagsAndName(const string flags, const string name);

}

#endif /* JavaHelper_hpp */
