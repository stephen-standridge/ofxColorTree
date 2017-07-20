 
 //  SpaceTree.hpp
 //  tree
 //
 //  Created by kevin ambrosia on 6/30/17.
 //
 //

 #pragma once

 #include "ofMain.h"
 #include "BoundingBox.h"
#include <limits>

typedef unsigned char byte;

template <class ObjectClass>
class ofxColorTree {
public:
    struct ColorTreeNode {
        ColorTreeNode(ObjectClass *o) :
        object(o),
        color(ofVec3f::zero()){};
        ColorTreeNode(ObjectClass *o, ofVec3f c) :
        object(o),
        color(c){};
        ofVec3f color;
        ObjectClass * object;
        //pointer reference to object stored
    };
    
    struct Closest {
        
        ofxColorTree<ObjectClass> * tree;
        ObjectClass * item;
        float dist = std::numeric_limits<float>::max();
        
    };
 
    ofxColorTree() {
        currentLife = -1;
    };

    ofxColorTree(BoundingBox _region) {
        region = _region;
        currentLife = -1;
    };

    ofxColorTree(BoundingBox _region, vector<ObjectClass*> _objects) {
        for(ObjectClass* object:_objects) {
            pending.push_back(ColorTreeNode(object));
        }
        region = _region;
        currentLife = -1;
    };
 
    ofxColorTree(BoundingBox _region, vector<ColorTreeNode> _objects) {
        for(ColorTreeNode object:_objects) {
            pending.push_back(object);
        }
        region = _region;
        currentLife = -1;
    };
 
    void release();
    void setup();
    void drawRegion();
    void update();
    void insert(ObjectClass *item);
    void insert(ColorTreeNode item);
 
    ObjectClass * getClosestByPoint(ObjectClass point);
    ObjectClass * getClosestByPoint(ObjectClass *point);
    ObjectClass * getClosestByPoint(float x, float y, float z);

    ObjectClass * getClosestByColor(ObjectClass color);
    ObjectClass * getClosestByColor(ObjectClass *color);
    ObjectClass * getClosestByColor(float r, float g, float b);

    void buildTree();
    void findEnclosingCube();


    byte active_nodes = 0;
    const int MIN_SIZE = 2;
    int maxLifespan = 8;
    int currentLife = -1;

    bool treeReady = false;
    bool treeBuilt = false;
    bool hasChildren = false;

    BoundingBox region;
    ofxColorTree * children[8];
    ofxColorTree * parent;
    vector<ColorTreeNode> objects;
    deque<ColorTreeNode> pending;

protected:
    void getClosest(ObjectClass point, Closest & closestInfo);

};
