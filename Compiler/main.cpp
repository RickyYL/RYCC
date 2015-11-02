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

using namespace std;

int main(int argc, const char * argv[]) {
    
    std::string input = "[e, f] = [g, h]";
    std::string input2;
    
    cout << "Input text: " << input << endl << "Begin tokenizing..." << endl;
    ListLexer myLexer(input);
    myLexer.consume();
    auto tokens = myLexer.tokenize();
    for (auto token: tokens)
        cout << "\t" << token.text() << " " << Token::getTokenName(token.type()) << endl;
    myLexer.reset();
    
    Parser myParser(myLexer);
    myParser.stat();
    
    return 0;
}
