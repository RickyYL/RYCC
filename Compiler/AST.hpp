//
//  AST.hpp
//  Compiler
//
//  Created by Yuanqi on 11/2/15.
//  Copyright © 2015 Yuanqi. All rights reserved.
//

#ifndef AST_hpp
#define AST_hpp

#include <iostream>
#include <vector>

#include "Token.hpp"

class AST {
    
public:
    
    AST() = default;
    AST(Token token): token(token), children(nullptr) { }
   	AST(int tokenType): token(Token(tokenType)), children(nullptr) { }
    ~AST() { if (children != nullptr) delete children; }
    
    int getNodeType() {
        return token.type();
    }
    
    void addChild(AST t) {
        if (children == nullptr)
            children = new std::vector<AST>();
       	children->push_back(t);
    }
    
    bool isNil() {
        return token.type() == NOTHING;
    }
    
    std::string toString() {
        return token.type() != NOTHING ? token.text() : "nil";
    }
    
    std::string toStringTree() {
        
        if (children == nullptr || children->size() == 0)
            return toString();
        
        std::string buffer;
        if (!isNil()) {
            buffer.append("(");
            buffer.append(toString());
            buffer.append(" ");
        }
        for (int i = 0; i < children->size(); i++) {
            AST t((*children)[i]);
            if (i > 0)
                buffer.append(" ");
            buffer.append(t.toStringTree());
        }
        if (!isNil())
            buffer.append(")");
        
        return buffer;
    }
    
    Token token;
    std::vector<AST> * children;
    
};

class ExprNode: public AST {
    
public:
    
    ExprNode(Token payload): AST(payload) { }

    int evalType() const { return _evalType; }
    void evalType(int newValue) { _evalType = newValue; }
    
    std::string toString() {
        if (evalType() != tINVALID)
            return AST::toString() + "<type=" + (evalType() == tINTEGER ? "tINTEGER" : "tVECTOR" ) + ">";
        return AST::toString();
    }
    
    static const int tINVALID = 0;
    static const int tINTEGER = 1;
    static const int tVECTOR  = 2;
    
private:
    
    int _evalType;
};

class AddNode: public ExprNode {
    
public:
    
    AddNode(ExprNode left, Token addToken, ExprNode right)
        : ExprNode(addToken)
    {
        addChild(left);
        addChild(right);
    }
    
    int getEvalType() {
        
        ExprNode left((*children)[0].token);
        ExprNode right((*children)[1].token);
        
        if ( left.evalType()==tINTEGER && right.evalType()==tINTEGER ) {
            return tINTEGER;
        }
        if ( left.evalType()==tVECTOR && right.evalType()==tVECTOR ) {
            return tVECTOR;
        }
        
        return tINVALID;
    }
};

class IntNode: public ExprNode {
    
public:
    
    IntNode(Token t): ExprNode(t)
    {
        evalType(tINTEGER);
    }
};

class VectorNode: public ExprNode {
    
public:
    
    VectorNode(Token t, std::vector<ExprNode> elements)
        : ExprNode(t)
    {
        evalType(tVECTOR);
        for (auto node: elements) {
            addChild(node);
        }
    }
};

#endif /* AST_hpp */























