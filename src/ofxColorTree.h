 
 //  SpaceTree.hpp
 //  tree
 //
 //  Created by kevin ambrosia on 6/30/17.
 //
 //

 #pragma once

 #include "ofMain.h"
 #include "BoundingBox.h"

 typedef bitset<8> bitmask;

 template <class ObjectClass>
 class ofxColorTree {
 private:
     struct ColorTreeNode {
         ColorTreeNode(ObjectClass *o) :
         object(o),
         shouldMove(false),
         shouldDelete(false),
         color(ofVec3f::zero()){}
         ofVec3f color;
         bool shouldMove;
         bool shouldDelete;
         ObjectClass * object;
         //pointer reference to object stored
     };

 public:
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

     void update();
     void insert(ObjectClass *item);
     void initializeTree();
     void buildTree();
     void findEnclosingCube();


     bitmask active_nodes = 0;
     const int MIN_SIZE = 1;
     int maxLifespan = 8;
     int currentLife = -1;

     static bool treeReady;
     static bool treeBuilt;

     BoundingBox region;
     ofxColorTree * children[8];
     ofxColorTree * parent;
     vector<ColorTreeNode> objects;
     deque<ColorTreeNode> pending;
 };
