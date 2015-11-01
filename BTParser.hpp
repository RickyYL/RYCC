//
//  BTParser.hpp
//  Compiler
//
//  Created by Yuanqi on 11/1/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

//  Created: NOV 1, 2015
//  Last modified: NOV 1, 2015

//  Modification Logs:
//  NOV 1, 2015:
//      Created file.

#ifndef BTParser_hpp
#define BTParser_hpp

#include <iostream>
#include <vector>
#include <string>
#include <stack>

#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"

// Back-tracking Parser

// Parsing Rule
//    stat      : list EOF
//              | assign EOF
//              ;
//    assign	: list '=' list;
//    list      : '[' elements ']';
//    elements  : element (',' element)*;
//    element	: NAME '=' NAME
//              | NAME
//              | list
//              ;

class BTParser {
    
public:
    
    BTParser(Lexer &);

    void stat();
    void list();
    void elements();
    void element();
    void assign();
    
private:
    
    Lexer                       &input;
    std::vector<std::size_t>    markers;
    std::vector<Token>          lookahead;
    std::size_t                 p = 0;
    
    void    match(int);
    void    consume();
    
    void    mark();
    void    release();
    bool    isSpeculating();
    
    void    sync(int);
    Token   lookaheadToken(int);
    int     lookaheadType(int);
    
    bool    stat_list();
    bool    stat_assign();
};

#endif /* BTParser_hpp */
