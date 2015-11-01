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
#include "Parser.hpp"
#include "Token.hpp"
#include "Error.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    
    std::string input = "[a, b, c = d, [e, f]]";
    
    cout << "Input text: " << input << endl << "Begin tokenizing..." << endl;
    ListLexer myLexer(input);
    myLexer.consume();
    auto tokens = myLexer.tokenize();
    for (auto token: tokens)
        cout << "\t" << token.text() << " " << myLexer.getTokenName(token.type()) << endl;
    myLexer.reset();
    
    cout << "Begin parsing..." << endl;
    ListParser myParser(&myLexer, 2);
    myParser.parse();
    
    cout << "Parsing successful." << endl;
    
    return 0;
}
