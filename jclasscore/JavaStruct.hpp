//
//  JavaStruct.hpp
//  jclass
//
//  Created by DE4ME on 26.03.2025.
//

#ifndef JavaStruct_h
#define JavaStruct_h

#include <cstdint>
#include <cmath>


#include "JavaIterator.hpp"


namespace java {


#ifdef _WINDOWS

#define BYTESWAP16(N) _byteswap_ushort(N)
#define BYTESWAP32(N) _byteswap_ulong(N)
#define BYTESWAP64(N) _byteswap_uint64(N)

#else

#if __LITTLE_ENDIAN__

#define BYTESWAP16(N) __builtin_bswap16(N)
#define BYTESWAP32(N) __builtin_bswap32(N)
#define BYTESWAP64(N) __builtin_bswap64(N)

#else

#define BYTESWAP16(N) N
#define BYTESWAP32(N) N
#define BYTESWAP64(N) N

#endif

#endif


struct JavaClassInnerClassInfo;
struct JavaClassAttribute;


enum POOLTAG: uint8_t {
    
    POOLTAG_UNKNOWN = 0,
    POOLTAG_UTF8 = 1,
    POOLTAG_INTEGER = 3,
    POOLTAG_FLOAT = 4,
    POOLTAG_LONG = 5,
    POOLTAG_DOUBLE = 6,
    POOLTAG_CLASS = 7,
    POOLTAG_STRING = 8,
    POOLTAG_FIELDREF = 9,
    POOLTAG_METHODREF = 10,
    POOLTAG_INTERFACEMETHODREF = 11,
    POOLTAG_NAMEANDTYPE = 12,
    POOLTAG_METHODHANDLE = 15,
    POOLTAG_METHODTYPE = 16,
    POOLTAG_DYNAMIC = 17,
    POOLTAG_INVOKEDYNAMIC = 18,
    POOLTAG_MODULE = 19,
    POOLTAG_PACKAGE = 20,
};


enum ATTRTAG {
    
    ATTRTAG_UNKNOWN,
    ATTRTAG_CONSTANTVALUE,
    ATTRTAG_CODE,
    ATTRTAG_STACKMAPTABLE,
    ATTRTAG_BOOTSTRAPMETHODS,
    ATTRTAG_NESTHOST,
    ATTRTAG_NESTMEMBERS,
    ATTRTAG_PERMITTEDSUBCLASSES,
    ATTRTAG_EXCEPTIONS,
    ATTRTAG_INNERCLASSES,
    ATTRTAG_ENCLOSINGMETHOD,
    ATTRTAG_SYNTHETIC,
    ATTRTAG_SIGNATURE,
    ATTRTAG_RECORD,
    ATTRTAG_SOURCEFILE,
    ATTRTAG_LINENUMBERTABLE,
    ATTRTAG_LOCALVARIABLETABLE,
    ATTRTAG_LOCALVARIABLETYPETABLE,
    ATTRTAG_SOURCEDEBUGEXTENSION,
    ATTRTAG_DEPRECATED,
    ATTRTAG_RUNTIMEVISIBLEANNOTATIONS,
    ATTRTAG_RUNTIMEINVISIBLEANNOTATIONS,
    ATTRTAG_RUNTIMEVISIBLEPARAMETERANNOTATIONS,
    ATTRTAG_RUNTIMEINVISIBLEPARAMETERANNOTATIONS,
    ATTRTAG_RUNTIMEVISIBLETYPEANNOTATIONS,
    ATTRTAG_RUNTIMEINVISIBLETYPEANNOTATIONS,
    ATTRTAG_ANNOTATIONDEFAULT,
    ATTRTAG_METHODPARAMETERS,
    ATTRTAG_MODULE,
    ATTRTAG_MODULEPACKAGES,
    ATTRTAG_MODULEMAINCLASS,
};


enum METHODKINDTAG: uint8_t {
    
    METHODKINDTAG_UNKNOWN = 0,
    METHODKINDTAG_GETFIELD = 1,
    METHODKINDTAG_GETSTATIC = 2,
    METHODKINDTAG_PUTFIELD = 3,
    METHODKINDTAG_PUTSTATIC = 4,
    METHODKINDTAG_INVOKEVIRTUAL = 5,
    METHODKINDTAG_INVOKESTATIC = 6,
    METHODKINDTAG_INVOKESPECIAL = 7,
    METHODKINDTAG_NEWINVOKESPECIAL = 8,
    METHODKINDTAG_INVOKEINTERFACE = 9,
};


enum TYPETAG: uint8_t {
    
    TYPETAG_UNKNOWN = 0,
    TYPETAG_BYTE = 'B',
    TYPETAG_CHAR = 'C',
    TYPETAG_FLOAT = 'F',
    TYPETAG_DOUBLE = 'D',
    TYPETAG_INTEGER = 'I',
    TYPETAG_LONG = 'J',
    TYPETAG_SHORT = 'S',
    TYPETAG_BOOL = 'Z',
    TYPETAG_VOID = 'V',
    TYPETAG_STRING = 's',
    TYPETAG_ENUM = 'e',
    TYPETAG_CLASS = 'c',
    TYPETAG_INTERFACE = '@',
    TYPETAG_ARRAY = '[',
    TYPETAG_REFERENCE = 'L',
};


enum DESCRIPTIONTAG: uint8_t {
    
    DESCRIPTIONTAG_REFERENCE_BEGIN = TYPETAG_REFERENCE,
    DESCRIPTIONTAG_REFERENCE_END = ';',
    DESCRIPTIONTAG_PARAMETER_BEGIN = '(',
    DESCRIPTIONTAG_PARAMETER_END = ')',
    DESCRIPTIONTAG_ENUM_BEGIN = '<',
    DESCRIPTIONTAG_ENUM_END = '>',
};


enum ACCFLAG: uint16_t {
    
    ACCFLAG_PUBLIC  = 0x0001,
    ACCFLAG_PRIVATE  = 0x0002,
    ACCFLAG_PROTECTED = 0x0004,
    ACCFLAG_STATIC = 0x0008,
    ACCFLAG_FINAL = 0x0010,
    ACCFLAG_SUPER = 0x0020,
    ACCFLAG_SYNCHRONIZED = 0x0020,
    ACCFLAG_VOLATILE = 0x0040,
    ACCFLAG_BRIDGE = 0x0040,
    ACCFLAG_TRANSIENT = 0x0080,
    ACCFLAG_VARARGS = 0x0080,
    ACCFLAG_NATIVE = 0x0100,
    ACCFLAG_INTERFACE = 0x0200,
    ACCFLAG_ABSTRACT = 0x0400,
    ACCFLAG_STRICT = 0x0800,
    ACCFLAG_SYNTHETIC = 0x1000,
    ACCFLAG_ANNOTATION = 0x2000,
    ACCFLAG_ENUM = 0x4000,
    ACCFLAG_MODULE = 0x8000,
    ACCFLAG_MANDATED = 0x8000,
};


//MARK: PRAGMA PACK 1 PUSH


#pragma pack (push, 1)


//MARK: JAVACLASS


struct JavaClassVersion {
    
    uint16_t minor_version;
    uint16_t major_version;
};

struct JavaClassHeader {
    
    uint32_t _magic;
    JavaClassVersion _version;
    
    uint32_t magic() const { return BYTESWAP32(_magic); }
    JavaClassVersion version() const { return JavaClassVersion( BYTESWAP16(_version.minor_version), BYTESWAP16(_version.major_version) ); }
};


//MARK: POOL


struct JavaClassPoolClass {
    
    uint16_t _name_index;
    
    uint16_t name_index() const { return BYTESWAP16(_name_index); }
};


struct JavaClassPoolUtf8 {
    
    uint16_t _length;
    char bytes[];
    
    uint16_t length() const { return BYTESWAP16(_length); }
};


struct JavaClassPoolFieldRef {
    
    uint16_t _class_index;
    uint16_t _name_and_type_index;
    
    uint16_t class_index() const { return BYTESWAP16(_class_index); }
    uint16_t name_and_type_index() const { return BYTESWAP16(_name_and_type_index); }
};


typedef JavaClassPoolFieldRef JavaClassPoolMethodRef;
typedef JavaClassPoolFieldRef JavaClassPoolInterfaceMethodRef;


struct JavaClassPoolNameAndType {
    
    uint16_t _name_index;
    uint16_t _descriptor_index;
    
    uint16_t name_index() const { return BYTESWAP16(_name_index); }
    uint16_t descriptor_index() const { return BYTESWAP16(_descriptor_index); }
};


struct JavaClassPoolString {
    
    uint16_t _string_index;
    
    uint16_t string_index() const { return BYTESWAP16(_string_index); }
};


struct JavaClassPoolInteger {
    
    int32_t _bytes;
    
    int32_t bytes() const { return BYTESWAP32(_bytes); }
};


struct JavaClassPoolFloat {
    
    uint32_t _bytes;
    
    float bytes() const {
        uint32_t bits = BYTESWAP32(_bytes);
        return reinterpret_cast<float &>(bits);
    }
};


struct JavaClassPoolLong {
    
    union {
        struct {
            uint32_t _hibytes;
            uint32_t _lobytes;
        };
        int64_t _bytes;
    };
    
    long long bytes() const { return BYTESWAP64(_bytes); }
};


struct JavaClassPoolDouble {
    
    union {
        struct {
            uint32_t _hibytes;
            uint32_t _lobytes;
        };
        uint64_t _bytes;
    };
    
    double bytes() const {
        uint64_t bits = BYTESWAP64(_bytes);
        return reinterpret_cast<double &>(bits);
    }
};


struct JavaClassPoolModule {
    
    uint16_t _name_index;
    
    uint16_t name_index() const { return BYTESWAP16(_name_index); }
};


typedef JavaClassPoolModule JavaClassPoolPackage;


struct JavaClassPoolDynamic {
    
    uint16_t _bootstrap_method_attr_index;
    uint16_t _name_and_type_index;
    
    uint16_t bootstrap_method_attr_index() const { return BYTESWAP16(_bootstrap_method_attr_index); }
    uint16_t name_and_type_index() const { return BYTESWAP16(_name_and_type_index); }
};


typedef JavaClassPoolDynamic JavaClassPoolInvokeDynamic;


struct JavaClassPoolMethodType {
    
    uint16_t _descriptor_index;
    
    uint16_t descriptor_index() const { return BYTESWAP16(_descriptor_index); }
};


struct JavaClassPoolMethodHandle {
    
    METHODKINDTAG reference_kind;
    uint16_t _reference_index;
    
    uint16_t descriptor_index() const { return BYTESWAP16(_reference_index); }
};


struct JavaClassPool {
    
    POOLTAG tag;
    union {
        JavaClassPoolUtf8 utf8_info;
        JavaClassPoolClass class_info;
        JavaClassPoolString string_info;
        JavaClassPoolFieldRef field_info;
        JavaClassPoolMethodRef method_info;
        JavaClassPoolInterfaceMethodRef interface_method_info;
        JavaClassPoolNameAndType name_and_type_info;
        JavaClassPoolInteger integer_info;
        JavaClassPoolFloat float_info;
        JavaClassPoolLong long_info;
        JavaClassPoolDouble double_info;
        JavaClassPoolModule module_info;
        JavaClassPoolPackage package_info;
        JavaClassPoolDynamic dynamic_info;
        JavaClassPoolInvokeDynamic invoke_dynamic_info;
        JavaClassPoolMethodType method_type_info;
        JavaClassPoolMethodHandle method_handle;
    };
    
    size_t sizeOfStruct() const {
        switch (tag) {
            case POOLTAG_UTF8:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolUtf8) + utf8_info.length();
                
            case POOLTAG_INTEGER:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolInteger);
                
            case POOLTAG_FLOAT:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolFloat);
                
            case POOLTAG_LONG:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolLong);
                
            case POOLTAG_DOUBLE:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolDouble);
                
            case POOLTAG_CLASS:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolClass);
                
            case POOLTAG_STRING:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolString);
                
            case POOLTAG_FIELDREF:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolFieldRef);
                
            case POOLTAG_METHODREF:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolMethodRef);
                
            case POOLTAG_INTERFACEMETHODREF:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolInterfaceMethodRef);
                
            case POOLTAG_NAMEANDTYPE:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolNameAndType);
                
            case POOLTAG_METHODHANDLE:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolMethodHandle);
                
            case POOLTAG_METHODTYPE:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolMethodType);
                
            case POOLTAG_DYNAMIC:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolDynamic);
                
            case POOLTAG_INVOKEDYNAMIC:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolInvokeDynamic);
                
            case POOLTAG_MODULE:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolModule);
                
            case POOLTAG_PACKAGE:
                return sizeof(POOLTAG) + sizeof(JavaClassPoolPackage);
                
            default:
                return 0;
        }
    }
};


//MARK: INTERFACE


struct JavaClassInterface {
    
    uint16_t _index;
    
    uint16_t index() const { return BYTESWAP16(_index); }
    size_t sizeOfStruct() const { return sizeof(JavaClassInterface); }
};


//MARK: ATTRIBUTE


struct JavaClassCodeException {
    
    uint16_t _start_pc;
    uint16_t _end_pc;
    uint16_t _handler_pc;
    uint16_t _catch_type;
};


struct JavaClassAttributeConstantValue {
    
    uint16_t _constantvalue_index;
    
    uint16_t constantvalue_index() const { return BYTESWAP16(_constantvalue_index); }
};


struct JavaClassAttributeSignature {
    
    uint16_t _signature_index;
    
    uint16_t signature_index() const { return BYTESWAP16(_signature_index); }
};


struct JavaClassAttributeSourceFile {
    
    uint16_t _sourcefile_index;
    
    uint16_t sourcefile_index() const { return BYTESWAP16(_sourcefile_index); }
};


struct JavaClassInnerClassInfo {
    
    uint16_t _inner_class_info_index;
    uint16_t _outer_class_info_index;
    uint16_t _inner_name_index;
    uint16_t _inner_class_access_flags;
    
    uint16_t inner_class_info_index() const {return BYTESWAP16(_inner_class_info_index); }
    uint16_t outer_class_info_index() const {return BYTESWAP16(_outer_class_info_index); }
    uint16_t inner_name_index() const {return BYTESWAP16(_inner_name_index); }
    ACCFLAG inner_class_access_flags() const { return static_cast<ACCFLAG>(BYTESWAP16(_inner_class_access_flags)); }
};
    

struct JavaClassAttributeInnerClasses {
    
    uint16_t _number_of_classes;
#ifndef _WINDOWS
    const JavaClassInnerClassInfo info[];
#endif
    
    uint16_t number_of_classes() const { return BYTESWAP16(_number_of_classes); }
    InnerClassInfoIterator infoIterator() const;
    
#ifndef _WINDOWS
    const JavaClassInnerClassInfo* infoBegin() const {return info; }
#else
    const JavaClassInnerClassInfo* infoBegin() const { return reinterpret_cast<const JavaClassInnerClassInfo*>(reinterpret_cast<const char*>(this) + sizeof(uint16_t)); }
#endif
};


struct JavaClassAttributeCode {
    
    uint16_t _max_stack;
    uint16_t _max_locals;
    uint32_t _code_length;
    uint8_t code[];
    
    uint16_t max_stack() const { return BYTESWAP16(_max_stack); }
    uint16_t max_locals() const { return BYTESWAP16(_max_locals); }
    uint32_t code_length() const { return BYTESWAP32(_code_length); }
    
    uint16_t exception_count() const {
        
        auto ptr = reinterpret_cast<const uint16_t*>(code + code_length());
        return BYTESWAP16(*ptr);
    }
    
    uint16_t attrubutes_count() const {
        
        auto exception_begin = reinterpret_cast<const uint8_t*>(exceptionBegin());
        auto ptr = reinterpret_cast<const uint16_t*>(exception_begin + sizeof(JavaClassCodeException) * exception_count());
        return BYTESWAP16(*ptr);
    }
    
    const JavaClassCodeException* exceptionBegin() const {
        
        auto ptr = reinterpret_cast<const JavaClassCodeException*>(code + code_length() + sizeof(uint16_t));
        return ptr;
    }
    
    const JavaClassAttribute* attributeBegin() const {
        
        auto exception_begin = reinterpret_cast<const uint8_t*>(exceptionBegin());
        auto ptr = reinterpret_cast<const JavaClassAttribute*>(exception_begin + sizeof(JavaClassCodeException) * exception_count() + sizeof(uint16_t));
        return ptr;
    }
    
    AttributeIterator attributeIterator() const;
};


struct JavaClassAttribute {
    
    uint16_t _name_index;
    uint32_t _length;
    union {
        uint8_t bytes[];
        JavaClassAttributeCode code_info;
        JavaClassAttributeConstantValue constant_info;
        JavaClassAttributeSignature signature_info;
        JavaClassAttributeSourceFile sourcefile_info;
        JavaClassAttributeInnerClasses innerclasses_info;
    };
    
    uint16_t name_index() const { return BYTESWAP16(_name_index); }
    uint32_t length() const { return BYTESWAP32(_length); }
    size_t sizeOfStruct() const { return offsetof(JavaClassAttribute, bytes) + length(); }
};


//MARK: FIELD / METHOD


struct JavaClassField {
    
    uint16_t _access_flag;
    uint16_t _name_index;
    uint16_t _descriptor_index;
    uint16_t _attributes_count;
    JavaClassAttribute attribute[];
    
    ACCFLAG access_flag() const { return static_cast<ACCFLAG>( BYTESWAP16(_access_flag) ); }
    uint16_t name_index() const { return BYTESWAP16(_name_index); }
    uint16_t descriptor_index() const { return BYTESWAP16(_descriptor_index); }
    uint16_t attributes_count() const { return BYTESWAP16(_attributes_count); }
    
    size_t sizeOfStruct() const;
    
    AttributeIterator attributeIterator() const;
};


struct JavaClassMethod {
    
    uint16_t _access_flag;
    uint16_t _name_index;
    uint16_t _descriptor_index;
    uint16_t _attributes_count;
    JavaClassAttribute attribute[];
    
    ACCFLAG access_flag() const { return static_cast<ACCFLAG>( BYTESWAP16(_access_flag) ); }
    uint16_t name_index() const { return BYTESWAP16(_name_index); }
    uint16_t descriptor_index() const { return BYTESWAP16(_descriptor_index); }
    uint16_t attributes_count() const { return BYTESWAP16(_attributes_count); }
    
    size_t sizeOfStruct() const;
    
    AttributeIterator attributeIterator() const;
};


#pragma pack (pop)


//MARK: PRAGMA PACK 1 POP


} //namespace java


#endif /* JavaStruct_h */
