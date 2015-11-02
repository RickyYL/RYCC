//
//  Token.cpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#include "Token.hpp"

const std::vector<std::string> Token::tokenNames {
    "n/a", "<EOF>", "NAME", "COMMA", "EQUAL", "LBRACK", "RBRACK",
};