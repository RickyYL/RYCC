//
//  Tokenizer.hpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

//  Created: OCT 30, 2015
//  Last Modified: OCT 30, 2015

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <iostream>
#include <string>
#include <vector>

#include "Token.hpp"
#include "Error.hpp"

// LL(k) Lexer class

class Lexer {

public:
    
    Lexer(std::string);             // default constructor
    
    void consume();                 // proceed the current character
    void match(char);               // math the given character with the current one
    
protected:
    
    std::string input;              // the input text
    std::string::iterator curr;     // iterator of current character of input
    char ch;                        // current character of input
};

class ListLexer: protected Lexer {
    
public:
    
    ListLexer(std::string text): Lexer(text) { }    // default constructor
    
    Token nextToken();              // proceed to read next token and return it
    std::vector<Token> tokenize();  // tokenize all items in inputs
    
    Token NAME();                   // recognize if the token is a NAME_TYPE
    Token NUMBER();                 // recognize if the token is a NUM_TYPE
    void WHITE();                   // skip all white spaces
    
    // Token type names
    
    static const int EOF_TYPE    = -1;  // EOF
    static const int NAME_TYPE   = 2;   // identifier name
    static const int NUM_TYPE    = 3;   // numbers
    static const int COMMA_TYPE  = 4;   // ','
    static const int SEMIC_TYPE  = 5;   // ';'
    static const int LBRACK_TYPE = 6;   // '['
    static const int RBRACK_TYPE = 7;   // ']'
    static const int LPARE_TYPE  = 8;   // '('
    static const int RPARE_TYPE  = 9;   // ')'
    
    static const std::vector<std::string> tokenNames;
    static std::string getTokenName(int n) { return tokenNames[n]; }
};

#endif /* Tokenizer_hpp */
