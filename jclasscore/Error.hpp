//
//  Error.hpp
//  jclass
//
//  Created by DE4ME on 27.03.2025.
//

#ifndef Error_h
#define Error_h


#include <cstdint>
#include <string>


using namespace std;


class Error {
    
private:
    
    int _code;
    string _message;
  
public:
    
    Error(const string message, int code = -1) :
    _code(code),
    _message(message)
    {
        
    }
    
    Error(int code, const string message = "") :
    _code(code),
    _message(message)
    {
        
    }
    
    int code() const { return _code; }
    string message() const { return  _message; }
};


#endif /* Error_h */
