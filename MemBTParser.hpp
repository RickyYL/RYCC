//
//  MemBTParser.hpp
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

#ifndef MemBTParser_hpp
#define MemBTParser_hpp

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <map>

#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"

// stat : list EOF
//      | list '=' list
//      ;

class Parser {
    
public:
    
// class control
    
    Parser(Lexer &source): input(source)
    {
        lookahead.push_back(input.nextToken());
    }
    
// memorization
    
    bool alreadyParsedRule(std::map<std::size_t,int> memoization) {
        
        auto memo = memoization.count(index());
        
        if (memo == 0)
            return false;
        
        std::cout << "parsed list before at index: " << index()
        << "; skip ahead to token index: " << memo
        << ": " << lookahead[memo].text();
        
        if (memo == FAILED)
            throw PreviousParseFailedException();
        
        index(memo);
        
        return true;
    }
    
    void memoize(std::map<std::size_t,int> memoization,
                 std::size_t startTokenIndex, bool failed)
    {
        std::size_t stopTokenIndex = failed ? FAILED : index();
        memoization.insert({startTokenIndex, stopTokenIndex});
    }
    
// Parser control
    
    void match(int type_name) {
        if (lookaheadType(1) == type_name)
            consume();
        else
            throw MismatchedTokenException(type_name, lookaheadType(1));
    }
    
    void consume() {
        // proceed index
        index(index() + 1);
        
        if (index() == lookahead.size() && !isSpeculating()) {
            index(0);
            lookahead.clear();
        }
        sync(1);
    }
    
    void sync(int i) {
        // if there're not enough tokens ahead
        if (index() + i > lookahead.size()) {
            // calculate how many tokens need
            auto n = index() + i - lookahead.size();
            // read sufficient tokens
            for (int j = 0; j < n; j++)
                lookahead.push_back(input.nextToken());
        }
    }
    
    Token lookaheadToken(int i) {
        // make sure enough tokens ahead
        sync(i);
        // return the i-th ahead token
        return lookahead[index() + i - 1];
    }
    
    int lookaheadType(int i) {
        return lookaheadToken(i).type();
    }
    
// stack control
    
    bool isSpeculating() {
        return markers.size() > 0;
    }
    
    void mark() {
        markers.push_back(index());
    }
    
    void release() {
        index(markers[markers.size()-1]);
        markers.pop_back();
    }
    
// Getter and Setter of _index
    
    const std::size_t & index() const {
        return _index;
    }
    
    void index(const std::size_t &index) {
        _index = index;
    }
    
// constant(s)
    
    static const int FAILED = -1;
    
// Parsing Rules
    
    // list parser that enables memorize
    
    void list() {
        
        bool failed = false;
        auto startTokenIndex = index();
        
        if (isSpeculating() && alreadyParsedRule(list_memo))
            return;
        
        try { _list(); }
        catch (RecognitionException re) {
            failed = true;
            if (isSpeculating())
                memoize(list_memo, startTokenIndex, failed);
            throw RecognitionException();
        }
        
        if (isSpeculating())
            memoize(list_memo, startTokenIndex, failed);
    }
    
    void _list() {
        std::cout << "\tParse list rule at token index: " << index() << std::endl;
        match(LBRACK_TYPE);
        elements();
        match(RBRACK_TYPE);
    }
    
    // [RULE] stat: list EOF | assign EOF;
    void stat() {
        // check if stat is a list
        if (stat_list()) {
            list();
            match(EOF_TYPE);
        }
        // check if stat is an assignment
        else if (stat_assign()) {
            assign();
            match(EOF_TYPE);
        } else
            throw NoViableAltException(lookaheadType(1));
    }
    
    // [RULE] stat: list EOF
    bool stat_list() {
        bool success = true;
        mark();         // record where this parsing begun
        try { list(); match(EOF_TYPE); }
        catch (MismatchedTokenException mte) { success = false; }
        release();      // restore to the condition the parsing begun
        return success;
    }
    
    // [RULE] stat: assign EOF
    bool stat_assign() {
        bool success = true;
        mark();
        try { assign(); match(EOF_TYPE); }
        catch (MismatchedTokenException mte) { success = false; }
        release();
        return success;
    }
    
    // [RULE] elements: element (',' element)* ;
    void elements() {
        element();
        while (lookaheadType(1) == COMMA_TYPE) {
            match(COMMA_TYPE);
            element();
        }
    }
    
    // [RULE] element: NAME '=' NAME | NAME | list ;
    void element() {
        if (lookaheadType(1) == NAME_TYPE && lookaheadType(2) == EQUAL_TYPE) {
            match(NAME_TYPE);
            match(EQUAL_TYPE);
            match(NAME_TYPE);
        } else if (lookaheadType(1) == NAME_TYPE)
            match(NAME_TYPE);
        else if (lookaheadType(1) == LBRACK_TYPE)
            list();
        else
            throw Error("Expecting a NAME, LIST or an assignment.");
    }
    
    // [RULE] assign: list '=' list;
    void assign() {
        list();
        match(EQUAL_TYPE);
        list();
    }
    
private:
    
    Lexer &input;                       // input lexer, ref enables dynamic binding
    std::vector<Token> lookahead;       // lookahead tokens
    std::size_t _index = 0;             // index of the lookahead token
    std::vector<std::size_t> markers;   // stack enables LL(k) grammar
    std::map<std::size_t, int> list_memo;   // memo container for list_rule
};

#endif /* MemBTParser_hpp */
