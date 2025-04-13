//
//  JavaHelper.hpp
//  jclass
//
//  Created by DE4ME on 27.03.2025.
//

#ifndef JavaHelper_hpp
#define JavaHelper_hpp


#include "JavaStruct.hpp"


#include <stdio.h>
#include <string>
#include <vector>


using namespace std;


namespace java {


class JavaClass;


const string JavaHelperPoolTagName(POOLTAG tag);
const string JavaHelperTypeTagName(TYPETAG tag);

const string JavaHelperVersion(const JavaClassVersion& version, bool java_se = false);

const string JavaHelperAccessFlags(ACCFLAG flags, const string& separator = " ");
const string JavaHelperFieldAccessFlags(ACCFLAG flags, const string& separator = " ");
const string JavaHelperMethodAccessFlags(ACCFLAG flags, const string& separator = " ");
const string JavaHelperInnerClassAccessFlags(ACCFLAG flags, const string& separator = " ");
const string JavaHelperMethodParameterAccessFlags(ACCFLAG flags, const string& separator = " ");

const string JavaHelperNameAndType(const JavaClass& java, const JavaClassPool* pool);
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

bool JavaHelperTypeTagValid(TYPETAG tag);

const string JavaHelperTypeNameArray(const string& name, uint8_t dimension);
const string JavaHelperJoinWithSeparator(const vector<string>& array, const string& separator);

}

#endif /* JavaHelper_hpp */
