//
//  JavaTypeDescription.hpp
//  jclass
//
//  Created by DE4ME on 08.04.2025.
//

#ifndef JavaTypeDescription_hpp
#define JavaTypeDescription_hpp


#include <cstdint>
#include <string>


using namespace std;


namespace java {


class JavaTypeDescription {
    
private:
    
    const string _description;
    uint16_t _index;
    uint16_t _count;
    
    char next() { return _index < _count ? _description[_index++] : 0; }
    const string parse(const string& separator);
    
public:
    
    JavaTypeDescription(uint16_t length, const char* buffer);
    JavaTypeDescription(const string& description);
    
    bool isMethod();
    const string str(const string& separator = " ");
};


} //namespace java


#endif /* JavaTypeDescription_hpp */
