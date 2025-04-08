//
//  JavaIterator.hpp
//  jclass
//
//  Created by DE4ME on 27.03.2025.
//


#ifndef JavaIterator_hpp
#define JavaIterator_hpp


#include <stdio.h>
#include <cstdint>


namespace java {


template <typename T>
class IteratorInterface {
    
public:
    
    virtual uint16_t index() const = 0;
    virtual uint16_t count() const = 0;
    virtual size_t sizeOfStruct() const = 0;
    virtual const T* cbegin() const = 0;
    virtual const T* cend() = 0;
    virtual const T* next() = 0;
    virtual const T* current() const = 0;
};


template <typename T>
class ConstantIterator: IteratorInterface<T> {
    
private:
    
    uint16_t _count;
    uint16_t _index;
    
    const T* _current;
    const T* _cbegin;
    const T* _cend;
    
public:
    
    ConstantIterator(uint16_t count = 0, const T* begin = nullptr) :
    _cbegin(begin),
    _current(begin),
    _cend(&begin[count]),
    _count(count),
    _index(0)
    {
        
    }
    
    ConstantIterator& operator=(const ConstantIterator& rvalue) {
        
        if (this == &rvalue) {
            return *this;
        }
        _count = rvalue._count;
        _index = rvalue._index;
        _cbegin = rvalue._cbegin;
        _cend = rvalue._cend;
        _current = rvalue._current;
        return *this;
    }
    
    virtual uint16_t count() const override { return _count; }
    virtual size_t sizeOfStruct() const override;
    virtual uint16_t index() const override { return _index; }
    virtual const T* cbegin() const override { return _cbegin; }
    virtual const T* cend() override { return _cend; }
    virtual const T* current() const override { return _index < _count ? _current : nullptr; }
    
    virtual const T* next() override;
};


template <typename T>
class VarribleIterator: public IteratorInterface<T> {
    
protected:
    
    uint16_t _count;
    uint16_t _index;
    
    const T* _current;
    const T* _cbegin;
    const T* _cend;
    
public:
    
    VarribleIterator(uint16_t count = 0, const T* begin = nullptr) :
    _cbegin(begin),
    _current(begin),
    _cend(nullptr),
    _count(count),
    _index(0)
    {
        
    }
    
    VarribleIterator& operator=(const VarribleIterator& rvalue) {
        
        if (this == &rvalue) {
            return *this;
        }
        _count = rvalue._count;
        _index = rvalue._index;
        _cbegin = rvalue._cbegin;
        _cend = rvalue._cend;
        _current = rvalue._current;
        return *this;
    }
    
    virtual uint16_t count() const override { return _count; }
    virtual uint16_t index() const override { return _index; }
    virtual size_t sizeOfStruct() const override;
    virtual const T* current() const override { return _index < _count ? _current : nullptr; }
    virtual const T* cbegin() const override { return _cbegin; }
    virtual const T* cend() override;
    virtual const T* next() override;
};


struct JavaClassPool;


class PoolIterator: public VarribleIterator<JavaClassPool> {
    
public:
    
    PoolIterator(uint16_t count = 0, const JavaClassPool* begin = nullptr) :
    VarribleIterator<JavaClassPool>(count, begin)
    {
        
    }
    
    virtual const JavaClassPool* cend() override;
    virtual const JavaClassPool* next() override;
};


struct JavaClassInterface;
struct JavaClassInnerClassInfo;
struct JavaClassField;
struct JavaClassAttribute;


typedef class ConstantIterator<JavaClassInterface> InterfaceIterator;
typedef class ConstantIterator<JavaClassInnerClassInfo> InnerClassInfoIterator;
typedef class VarribleIterator<JavaClassField> FieldIterator;
typedef class VarribleIterator<JavaClassField> MethodIterator;
typedef class VarribleIterator<JavaClassAttribute> AttributeIterator;


} //namespace java


#endif /* JavaIterator_hpp */
