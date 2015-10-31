//
//  Error.hpp
//  Compiler
//
//  Created by Yuanqi on 10/30/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#ifndef Error_hpp
#define Error_hpp

#include <iostream>
#include <exception>
#include <string>

class Error: std::exception {
    
public:
    
    Error(std::string msg=""): msg(msg) { }
    
private:
    
    std::string msg;
};

#endif /* Error_hpp */
