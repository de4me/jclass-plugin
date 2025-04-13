//
//  HexFormatter.cpp
//  jclass
//
//  Created by DE4ME on 12.04.2025.
//


#include "HexFormatter.hpp"


#include <format>


void HexFormatter::format(const uint8_t* buffer, size_t length) {
    
    uint8_t count = 0;
    
    if(length > 0) {
        for(auto i = length; i != 0; i/=0x10) {
            count++;
        }
        count = ((count + 1) / 2) * 2;
    } else {
        count = 2;
    }
    
    string offset_format = "{}{:0>" + to_string(count) + "x}{}";
    
    for(size_t i = 0; i < length; i++) {
        
        if( (i % lineLength) == 0) {
            string offset_string = std::vformat(offset_format, make_format_args(offsetPrefix, i, offsetSuffix));
            *this << offset_string;
        }
        
        auto inext = i + 1;
        if ( (inext % lineLength) == 0 || (inext >= length) ) {
            auto byte_string = std::format("{}{:0>2x}{}\r\n", bytePrefix, buffer[i], byteSuffix);
            *this << byte_string;
        } else {
            auto byte_string = std::format("{}{:0>2x}{}{}", bytePrefix, buffer[i], byteSuffix, byteSeparator);
            *this << byte_string;
        }
    }
}
