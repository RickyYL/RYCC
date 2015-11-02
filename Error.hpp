//
//  Error.hpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#ifndef Error_hpp
#define Error_hpp

#include <iostream>
#include <sstream>
#include <exception>
#include <string>

class Error: public std::exception {
    
public:
    
    Error(std::string msg=""): _msg(msg) { }
    std::string msg() { return _msg; }
    
private:
    
    std::string _msg;
};

class RecognitionException {
    
public:
    
    RecognitionException(std::string msg=""): _msg(msg) { }
    std::string msg() { return _msg; }
    
private:
    
    std::string _msg;
};

class MismatchedTokenException {
    
public:
    
    MismatchedTokenException(int tokenType, int found)
        : expecting(tokenType), found(found) { }
    
    std::string msg() {
        std::ostringstream oss;
        oss << "Expecting: " << expecting << ", found: " << found;
        return oss.str();
    }
    
private:
    
    int expecting;
    int found;
};

class NoViableAltException {
    
public:
    
    NoViableAltException(int found): found(found) { }
    
    std::string msg() {
        std::ostringstream oss;
        oss << "Expecting: stat, found: " << found;
        return oss.str();
    }
    
private:
    
    int found;
};

class PreviousParseFailedException {
    
    
};






#endif /* Error_hpp */
