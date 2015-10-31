//
//  Token.hpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#ifndef Token_hpp
#define Token_hpp

#include <iostream>
#include <sstream>
#include <string>

class Token {
    
public:
    
    Token(int type, std::string text): _type(type), _text(text) {}
    
    std::string text() { return _text; }
    int type() { return _type; }
    
private:
    
    int _type;
    std::string _text;
};

#endif /* Token_hpp */
