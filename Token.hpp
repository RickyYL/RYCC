//
//  Token.hpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

//  Created: OCT 30, 2015
//  Last Modified: OCT 30, 2015

#ifndef Token_hpp
#define Token_hpp

#include <iostream>
#include <sstream>
#include <string>

#define EOF_TYPE       -1   // EOF
#define NAME_TYPE       2   // identifier name
#define COMMA_TYPE      3   // ','
#define LBRACK_TYPE     4   // '['
#define RBRACK_TYPE     5   // ']'
#define EQUAL_TYPE      6   // '='

class Token {
    
public:
    
    Token(): _type(-1), _text("") { }
    
    Token(int type, std::string text)
        : _type(type), _text(text) { }
    
    std::string text() { return _text; }
    int type() { return _type; }
    
private:
    
    int _type;
    std::string _text;
};

#endif /* Token_hpp */
