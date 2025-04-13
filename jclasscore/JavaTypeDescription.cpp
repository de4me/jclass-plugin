//
//  JavaTypeDescription.cpp
//  jclass
//
//  Created by DE4ME on 08.04.2025.
//

#include "JavaTypeDescription.hpp"


#include <vector>
#include <sstream>
#include <format>


#include "JavaStruct.hpp"
#include "JavaHelper.hpp"


namespace java {


JavaTypeDescription::JavaTypeDescription(uint16_t length, const char* buffer) :
_description(buffer, length),
_index(0),
_count(length)
{
    
}


JavaTypeDescription::JavaTypeDescription(const std::string& description) :
_description(description),
_index(0),
_count(static_cast<uint16_t>(description.size()))
{
    
}


bool JavaTypeDescription::isMethod() {
    
    return _description.empty() ? false : _description[0] == DESCRIPTIONTAG_PARAMETER_BEGIN;
}


void push_back(vector<string>& array, const string& name, int dimension) {
    
    if (dimension > 0) {
        auto array_name = JavaHelperTypeNameArray(name, dimension);
        array.push_back(array_name);
    } else {
        array.push_back(name);
    }
}


const string JavaTypeDescription::parse(const std::string& separator) {
    
    int dimension = 0;
    vector<string> array;
    while(char ch = next()) {
        switch (ch) {
                
            case DESCRIPTIONTAG_PARAMETER_BEGIN:
            {
                auto start_index = _index;
                while (true) {
                    auto ch = next();
                    if (ch == 0) {
                        return "";
                    }
                    if (ch == DESCRIPTIONTAG_PARAMETER_END) {
                        break;
                    }
                }
                auto description = JavaTypeDescription(_index - start_index - 1, &_description[start_index]);
                auto name = description.str(", ");
                auto result = format("({})",name);
                array.push_back(result);
                array.push_back("->");
                break;
            }
                
            case DESCRIPTIONTAG_REFERENCE_BEGIN:
            {
                auto start_index = _index;
                while (true) {
                    auto ch = next();
                    if (ch == 0) {
                        return "";
                    }
                    if (ch == DESCRIPTIONTAG_REFERENCE_END) {
                        break;
                    }
                }
                auto name = _description.substr(start_index, _index - start_index - 1);
                push_back(array, name, dimension);
                dimension = 0;
                break;
            }
                
            case TYPETAG_ARRAY:
                dimension++;
                break;
                
            default:
            {
                TYPETAG tag = static_cast<TYPETAG>(ch);
                if (!JavaHelperTypeTagValid(tag) ) {
                    return "";
                }
                auto name = JavaHelperTypeTagName(tag);
                push_back(array, name, dimension);
                dimension = 0;
                break;
            }
        }
    }
    switch (array.size()) {
            
        case 0:
            return "";
            
        case 1:
            return array.front();
            
        default:
            return JavaHelperJoinWithSeparator(array, separator);
    }
}


const string JavaTypeDescription::str(const std::string& separator) {
    
    if (_description.empty()) {
        return _description;
    }
    auto result = parse(separator);
    return result.empty() ? _description : result;
}


} //namespace java
