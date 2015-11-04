//
//  Token.hpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

//  Created: OCT 30, 2015
//  Last Modified: NOV 1, 2015

//  Modification Logs:
//  NOV 1, 2015:
//      optimize constructor, enhance stability.

#ifndef Token_hpp
#define Token_hpp

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define EOF_TYPE       -1   // EOF
#define NOTHING         0   // really nothing
#define NAME_TYPE       2   // identifier name
#define COMMA_TYPE      3   // ','
#define EQUAL_TYPE      4   // '='
#define LBRACK_TYPE     5   // '['
#define RBRACK_TYPE     6   // ']'

#define INT             20
#define FLOAT           21

#define PLUS            41
#define SUB             42
#define MULTI           43
#define DEVIDE          44

class Token {
    
public:
    
    Token(int type = NOTHING, std::string text = "nil")
        : _type(type), _text(text) { }
    
    const std::string & text() const { return _text; }
    const int & type() const { return _type; }
    
    static const std::string & getTokenName(int tokenType) {
        return tokenNames[tokenType];
    }
    
private:
    
    int _type;
    std::string _text;
    
    static const std::vector<std::string> tokenNames;
};

#endif /* Token_hpp */
