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
    AST(Token token): token(new Token(token)) { }
    ~AST() {
//        if (token) delete token;
//        if (children) delete children;
    }
    
    int getNodeType() const { return token->type(); }
    bool isNil() const { return token == nullptr; }
    
    void addChild(AST *t) {
        if (children == nullptr)
            children = new std::vector<AST*>();
       	children->push_back(new AST(*t));
    }
    
    virtual std::string toString() const {
        return token != nullptr ? token->text() : "nil";
    }
    
    std::string toStringTree() const {
        
        if (children == nullptr || children->size() == 0)
            return toString();
        
        std::string buffer;
        
        for (auto elem: *children)
            buffer += " " + elem->toStringTree();
        if (!isNil())
            buffer = "(" + toString() + buffer + ")";
        
        return buffer;
    }
    
    Token * token = nullptr;
    std::vector<AST*> * children = nullptr;
    
};

class ExprNode: public AST {
    
public:
    
    static const int tINVALID = 0;
    static const int tINTEGER = 1;
    static const int tVECTOR  = 2;
    
    ExprNode(Token payload): AST(payload) { }

    int  evalType() const       { return _evalType; }
    void evalType(int newValue) { _evalType = newValue; }
    
    std::string toString() const {
        if (evalType() != tINVALID)
            return AST::toString() + "<type=" + (evalType() == tINTEGER ? "tINTEGER" : "tVECTOR" ) + ">";
        return AST::toString();
    }
    
private:
    
    int _evalType;
};

class AddNode: public ExprNode {
    
public:
    
    AddNode(ExprNode left, Token addToken, ExprNode right)
        : ExprNode(addToken)
    {
        addChild(&left);
        addChild(&right);
    }
    
    int getEvalType() const {
        
        ExprNode left(*children->at(0)->token);
        ExprNode right(*children->at(1)->token);
        
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
    
    IntNode(Token t): ExprNode(t) {
        evalType(tINTEGER);
    }
};

class VectorNode: public ExprNode {
    
public:
    
    VectorNode(Token t, std::vector<ExprNode> elements) : ExprNode(t) {
        evalType(tVECTOR);
        for (auto node: elements) {
            addChild(&node);
        }
    }
};

#endif /* AST_hpp */
