 
 //  SpaceTree.hpp
 //  tree
 //
 //  Created by kevin ambrosia on 6/30/17.
 //
 //

 #pragma once

 #include "ofMain.h"
 #include "BoundingBox.h"

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
 };
