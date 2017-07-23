 
 //  SpaceTree.hpp
 //  tree
 //
 //  Created by kevin ambrosia on 6/30/17.
 //
 //

 #pragma once

#include <type_traits>
#include <limits>
#include <glm/vec3.hpp>
#include "ofMain.h"
#include "ofxColorTreeBoundingBox.h"
#include "ofxColorTreeNode.h"

typedef unsigned char byte;

template <class ObjectClass>
class ofxColorTree {
//private:
//    static_assert(std::is_base_of<ofNode, ObjectClass>::value, "Provided class does not inherit from ofNode");
public:
    struct Closest {
        
        ofxColorTree<ObjectClass> * tree;
        ObjectClass * item;
        float dist = std::numeric_limits<float>::max();
        
    };
 
    ofxColorTree() {
        currentLife = -1;
    };

    ofxColorTree(ofxColorTreeBoundingBox _region) {
        region = _region;
        currentLife = -1;
    };

    ofxColorTree(ofxColorTreeBoundingBox _region, vector<ObjectClass*> _objects) {
        for(ObjectClass* object:_objects) {
            pending.push_back(ofxColorTreeNode<ObjectClass>(object));
        }
        region = _region;
        currentLife = -1;
    };
 
    ofxColorTree(ofxColorTreeBoundingBox _region, vector<ofxColorTreeNode<ObjectClass>> _objects) {
        for(auto object:_objects) {
            pending.push_back(*object);
        }
        region = _region;
        currentLife = -1;
    };
    
    ofxColorTree(ofxColorTreeBoundingBox _region, vector<shared_ptr<ofxColorTreeNode<ObjectClass>>> _objects) {
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
    shared_ptr<ofxColorTreeNode<ObjectClass>> insert(ObjectClass *item);
    void insert(shared_ptr<ofxColorTreeNode<ObjectClass>> item);

 
    ObjectClass * getClosestByPoint(ofVec3f point);
    ObjectClass * getClosestByPoint(ofVec3f *point);
    ObjectClass * getClosestByPoint(glm::vec3 point);
    ObjectClass * getClosestByPoint(float x, float y, float z);

    ObjectClass * getClosestByColor(ofVec3f color);
    ObjectClass * getClosestByColor(ofVec3f *color);
    ObjectClass * getClosestByColor(glm::vec3 color);
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

    ofxColorTreeBoundingBox region;
    ofxColorTree * children[8];
    ofxColorTree * parent;
    vector<shared_ptr<ofxColorTreeNode<ObjectClass>>> objects;
    deque<shared_ptr<ofxColorTreeNode<ObjectClass>>> pending;

protected:
    void getClosest(ofVec3f point, Closest & closestInfo);

};
