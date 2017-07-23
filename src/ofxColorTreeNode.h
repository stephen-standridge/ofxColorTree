//
//  ofxColorTreeNode.h
//  tree
//
//  Created by kevin ambrosia on 7/22/17.
//
//

#pragma once
#include <type_traits>
#include "ofMain.h"

template <class ContainedClass>
struct ofxColorTreeNode {
//private:
//    static_assert(std::is_base_of<ofNode, ContainedClass>::value, "Provided class does not inherit from ofNode");
public:
    ofxColorTreeNode() : object(NULL), color(ofVec3f::zero()){};
    ofxColorTreeNode(ContainedClass *o) : object(o), color(ofVec3f::zero()){};
    ofxColorTreeNode(ContainedClass *o, ofVec3f c) : object(o), color(c){};
    ofVec3f color;
    bool shouldMove = false;
    bool shouldDelete = false;
    ContainedClass * object;
};

