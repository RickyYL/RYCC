//
//  main.cpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>

#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"
#include "MemBTParser.hpp"
#include "AST.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    
    std::string input = "[e, f] = [g, h]";
    
    cout << "Input text: " << input << endl << "Begin tokenizing..." << endl;
    ListLexer myLexer(input);
    myLexer.consume();
    auto tokens = myLexer.tokenize();
    for (auto token: tokens)
        cout << "\t" << token.text() << " " << Token::getTokenName(token.type()) << endl;
    myLexer.reset();
    cout << "Tokenizing succeed." << endl << endl;
    
    cout << "Begin parsing..." << endl;
    Parser myParser(myLexer);
    myParser.stat();
    cout << "Parsing succeed." << endl << endl;
    
    cout << "Begin building AST..." << endl;
    Token plus = Token(42, "+");
    Token one  = Token(42, "1");
    Token two  = Token(42, "2");
    AST root(plus);
    root.addChild(new AST(one));
    root.addChild(new AST(two));
    cout << "\t" << root.toStringTree() << endl;
    
    AddNode root2(IntNode(two), plus ,IntNode(two));
    cout << "\t" << root2.toStringTree() << endl;
    cout << "Building AST succeed." << endl;
    
    return 0;
}
