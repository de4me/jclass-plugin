//
//  HexFormatter.hpp
//  jclass
//
//  Created by DE4ME on 12.04.2025.
//

#ifndef HexFormatter_hpp
#define HexFormatter_hpp


#include <stdio.h>
#include <sstream>


using namespace std;


class HexFormatter: public ostringstream {
    
public:
    
    int bytesPerLine;
    
    string offsetPrefix;
    string offsetSuffix;
    string bytePrefix;
    string byteSeparator;
    string byteSuffix;
    
    HexFormatter(int bytes_per_line) :
    offsetPrefix(""),
    offsetSuffix(": "),
    bytePrefix(""),
    byteSeparator(""),
    byteSuffix(""),
    bytesPerLine(bytes_per_line)
    {
        
    }
    
    void format(const uint8_t* buffer, size_t length);
    
};


#endif /* HexFormatter_hpp */
