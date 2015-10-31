//
//  main.cpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#include <iostream>

#include "Lexer.hpp"
#include "Token.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    
    ListLexer myLexer("[a , (a); bc]aa-23cddcdc2");
    
    auto tokens = myLexer.tokenize();
    
    for (auto token : tokens) {
        cout << token.text() << " ";
    }
    cout << endl << endl;
    
    for (auto token : tokens) {
        cout << "< \"" << token.text() << "\", "
        << ListLexer::getTokenName(token.type())
        << " >" << endl;
    }

    return 0;
}
