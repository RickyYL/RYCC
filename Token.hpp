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
#define NAME_TYPE       2   // identifier name
#define COMMA_TYPE      3   // ','
#define EQUAL_TYPE      4   // '='
#define LBRACK_TYPE     5   // '['
#define RBRACK_TYPE     6   // ']'

class Token {
    
public:
    
    Token(int type = EOF_TYPE, std::string text = "")
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
