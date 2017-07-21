 
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

template <class ContainedClass>
struct NodeData {
    NodeData() : object(NULL), color(ofVec3f::zero()){};
    NodeData(ContainedClass *o) : object(o), color(ofVec3f::zero()){};
    NodeData(ContainedClass *o, ofVec3f c) : object(o), color(c){};
    ofVec3f color;
    bool shouldMove = false;
    bool shouldDelete = false;
    ContainedClass * object;
};

template <class ContainedClass>
struct ofxColorTreeNode {
    ofxColorTreeNode(NodeData<ContainedClass>* node) : data(node) {};
//    ~ofxColorTreeNode() { delete data; };
    NodeData<ContainedClass> *data;
};

template <class ObjectClass>
class ofxColorTree {
public:
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
            pending.push_back(NodeData<ObjectClass>(object));
        }
        region = _region;
        currentLife = -1;
    };
 
    ofxColorTree(BoundingBox _region, vector<NodeData<ObjectClass>> _objects) {
        for(auto object:_objects) {
            pending.push_back(*object);
        }
        region = _region;
        currentLife = -1;
    };
    
    ofxColorTree(BoundingBox _region, vector<NodeData<ObjectClass>*> _objects) {
        for(auto object:_objects) {
            pending.push_back(object);
        }
        region = _region;
        currentLife = -1;
    };
 
    void release();
    void setup();
    void drawRegion();
    void update();
    ofxColorTreeNode<ObjectClass> insert(ObjectClass *item);
    void insert(NodeData<ObjectClass> & item);
    void insert(NodeData<ObjectClass>* item);

 
    ObjectClass * getClosestByPoint(ObjectClass point);
    ObjectClass * getClosestByPoint(ObjectClass *point);
    ObjectClass * getClosestByPoint(float x, float y, float z);

    ObjectClass * getClosestByColor(ObjectClass color);
    ObjectClass * getClosestByColor(ObjectClass *color);
    ObjectClass * getClosestByColor(float r, float g, float b);

    void buildTree();
    void findEnclosingCube();


    byte active_nodes = 0;
    const int MIN_SIZE = 1;
    int maxLifespan = 8;
    int currentLife = -1;

    bool treeReady = false;
    bool treeBuilt = false;
    bool hasChildren = false;

    BoundingBox region;
    ofxColorTree * children[8];
    ofxColorTree * parent;
    vector<NodeData<ObjectClass>*> objects;
    deque<NodeData<ObjectClass>*> pending;

protected:
    void getClosest(ObjectClass point, Closest & closestInfo);

};
