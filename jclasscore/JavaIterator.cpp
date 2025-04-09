//
//  JavaIterator.cpp
//  jclass
//
//  Created by DE4ME on 27.03.2025.
//


#include "JavaIterator.hpp"
#include "JavaStruct.hpp"


namespace java {


template class VarribleIterator<JavaClassPool>;
template class ConstantIterator<JavaClassInterface>;
template class VarribleIterator<JavaClassField>;
template class VarribleIterator<JavaClassMethod>;
template class VarribleIterator<JavaClassAttribute>;
template class ConstantIterator<JavaClassInnerClassInfo>;


//ConstantIterator


template<typename T>
size_t ConstantIterator<T>::sizeOfStruct() const {
    
    return sizeof(T);
}


template<typename T>
const T* ConstantIterator<T>::next() {
    
    if (_index >= _count) {
        return nullptr;
    }
    _index++;
    _current++;
    return _index < _count ? _current : nullptr;
}


//MARK: VarribleIterator


template<typename T>
size_t VarribleIterator<T>::sizeOfStruct() const {
    
    return _current->sizeOfStruct();
}


template<typename T>
const T* VarribleIterator<T>::cend() {
    
    if (_cend != nullptr) {
        return _cend;
    }
    auto current = cbegin();
    for(uint16_t i=0; i < _count; i++) {
        auto size = current->sizeOfStruct();
        auto buffer = reinterpret_cast<const char*>(current) + size;
        current = reinterpret_cast<const T*>(buffer);
    }
    _cend = current;
    return _cend;
}


template<typename T>
const T* VarribleIterator<T>::next() {
    
    if (_index >= _count) {
        return nullptr;
    }
    _index++;
    auto buffer = reinterpret_cast<const char*>(_current);
    auto size = _current->sizeOfStruct();
    _current = reinterpret_cast<const T*>(buffer + size);
    return _index < _count ? _current : nullptr;
}


//MARK: PoolIterator


const JavaClassPool* PoolIterator::cend() {
    
    if (_cend != nullptr) {
        return _cend;
    }
    auto current = cbegin();
    for(uint16_t i = 0; i < _count; ) {
        auto size = current->sizeOfStruct();
        if (size == 0) {
            return nullptr;
        }
        switch (current->tag) {
            case POOLTAG_LONG:
            case POOLTAG_DOUBLE:
                i += 2;
                break;
            default:
                i ++;
                break;
        }
        const char* buffer = reinterpret_cast<const char*>(current) + size;
        current = reinterpret_cast<const JavaClassPool*>(buffer);
    }
    _cend = current;
    return _cend;
}


const JavaClassPool* PoolIterator::next() {
    
    if (_index >= _count) {
        return nullptr;
    }
    switch (_current->tag) {
        case POOLTAG_LONG:
        case POOLTAG_DOUBLE:
            _index += 2;
            break;
        default:
            _index ++;
    }
    auto buffer = reinterpret_cast<const char*>(_current);
    auto size = _current->sizeOfStruct();
    if (size == 0) {
        return nullptr;
    }
    _current = reinterpret_cast<const JavaClassPool*>(buffer + size);
    return _index < _count ? _current : nullptr;
}


} //namespace java
