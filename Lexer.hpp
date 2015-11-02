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
//  NOV 1, 2015
//      Move token type name to Token.hpp, make it accessable to others.

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <iostream>
#include <string>
#include <vector>

#include "Token.hpp"
#include "Error.hpp"

// LL(1) Lexer

class Lexer {

public:
    
    Lexer(std::string);
    
    void consume();                 // read the next character
    void match(char);               // if char == curr, consume()
    void reset();                   // reset lexer
    
    virtual Token nextToken() = 0;
    virtual std::vector<Token> tokenize() = 0;
    
private:
    
    std::string input;              // the input text
    std::string::iterator index;    // iterator of current character of input
    
protected:
    
    char lookahead;                 // current character of input
};

class ListLexer: public Lexer {
    
public:
    
    ListLexer(std::string);
    
    Token nextToken();              // proceed to read next token and return it
    std::vector<Token> tokenize();  // tokenize all items in inputs
    
private:
    
    Token NAME();                   // recognize if the token is a NAME_TYPE
    void WHITE();                   // skip all white spaces
};

#endif /* Tokenizer_hpp */
