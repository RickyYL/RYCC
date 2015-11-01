//
//  Tokenizer.hpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

//  Created: OCT 30, 2015
//  Last Modified: OCT 31, 2015

//  Modification Logs:
//  OCT 31, 2015:
//      Resoloved a problem caused by incorrectly defining virtual functions.
//      Refactoring code to correctly mark all members (private/protected).

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <iostream>
#include <string>
#include <vector>

#include "Token.hpp"
#include "Error.hpp"

// LL(1) Lexer class

class Lexer {

public:
    
    Lexer(std::string);
    
    void consume();                 // read the next character
    void match(char);               // if char == curr, consume()
    void reset();                   // reset lexer
    
    virtual Token nextToken() = 0;
    virtual std::vector<Token> tokenize() = 0;
    virtual std::string getTokenName(int) = 0;
    
private:
    
    std::string input;              // the input text
    std::string::iterator curr;     // iterator of current character of input
    
protected:
    
    char ch;                        // current character of input
};

class ListLexer: public Lexer {
    
    friend class ListParser;
    
public:
    
    ListLexer(std::string);
    
    Token nextToken();              // proceed to read next token and return it
    std::vector<Token> tokenize();  // tokenize all items in inputs
    std::string getTokenName(int);  // return the token name
    
private:
    
    static const int EOF_TYPE    = 1;   // EOF
    static const int NAME_TYPE   = 2;   // identifier name
    static const int COMMA_TYPE  = 3;   // ','
    static const int LBRACK_TYPE = 4;   // '['
    static const int RBRACK_TYPE = 5;   // ']'
    static const int EQUAL_TYPE  = 6;   // '='
    
    static const std::vector<std::string> tokenNames;
    
    Token NAME();                   // recognize if the token is a NAME_TYPE
    void WHITE();                   // skip all white spaces
};

#endif /* Tokenizer_hpp */
