 //
 //  SpaceTree.cpp
 //  tree
 //
 //  Created by kevin ambrosia on 6/30/17.
 //
 //

 #include "ofxColorTree.h"

 template <class ObjectClass>
 bool ofxColorTree<ObjectClass>::treeReady = false;
 template <class ObjectClass>
 bool ofxColorTree<ObjectClass>::treeBuilt = false;

 template <class ObjectClass>
 void ofxColorTree<ObjectClass>::initializeTree() {
    if (!treeBuilt) {
        while (pending.size() != 0) {
            objects.push_back(pending.move(0));
            pending.pop_front();
        }
        buildTree();
    } else {
        while (pending.size() != 0) {
            insert(pending.move(0));
            pending.pop_front();
        }
    }

    treeReady = true;
 };

 template <class ObjectClass>
 void ofxColorTree<ObjectClass>::buildTree() {
    if (objects.size() <= 1) { return; } //we're at a leaf

    ofVec3f dimensions = region.getMax() - region.getMin();

    if (dimensions == ofVec3f::zero()) {
        findEnclosingCube();
        dimensions = region.getMax() = region.getMin();
    }

    if (dimensions.x <= MIN_SIZE && dimensions.y <= MIN_SIZE && dimensions.z <= MIN_SIZE) {
        return;
    }

    ofVec3f min = region.getMin();
    ofVec3f max = region.getMax();
    ofVec3f half = dimensions / 2.0;
    ofVec3f center = min + half;

    BoundingBox octant[8] = {
        BoundingBox(min, center),
        BoundingBox(ofVec3f(center.x, min.y, min.z), ofVec3f(max.x, center.y, center.z)),
        BoundingBox(ofVec3f(center.x, min.y, center.z), ofVec3f(max.x, center.y, max.z)),
        BoundingBox(ofVec3f(min.x, min.y, center.z), ofVec3f(center.x, center.y, max.z)),
        BoundingBox(ofVec3f(min.x, center.y, min.z), ofVec3f(center.x, max.y, center.z)),
        BoundingBox(ofVec3f(center.x, center.y, min.z), ofVec3f(max.x, max.y, center.z)),
        BoundingBox(center, max),
        BoundingBox(ofVec3f(min.x, center.y, center.z), ofVec3f(center.x, max.y, max.z))
    };

    vector<ColorTreeNode> list[8];
    vector<int> delist;

    //go backwards so that removing them later doesnt mess up the order.
    for (int i = objects.size() - 1; i >= 0; i--) {
        for (int a = 0; a < 8; a++) {
            if(octant[a].contains(objects[i].object)) {
                list[a].push_back(objects[i]);
                delist.push_back(i);
                break;
            }
        }
    }

    for (int i:delist) {
        objects.erase(objects.begin() + i);
    }

    for (int a = 0; a < 8; a++) {
        if (list[a].size() != 0) {
            children[a] = new ofxColorTree(octant[a], list[a]);
            children[a] -> parent = this;
            active_nodes |= (bitset<8>)(1<<a);
            children[a] -> buildTree();
        }
    }

    treeBuilt = true;
    treeReady = true;
 };

 template <class ObjectClass>
 void ofxColorTree<ObjectClass>::findEnclosingCube() {

 };

 template <class ObjectClass>
 void ofxColorTree<ObjectClass>::update() {
    if (treeBuilt == true)  {
        //Start a count down death timer for any leaf nodes which don't have objects or children.
        //when the timer reaches zero, we delete the leaf. If the node is reused before death, we double its lifespan.
        //this gives us a "frequency" usage score and lets us avoid allocating and deallocating memory unnecessarily
        if (objects.size() == 0) {
            if (currentLife == -1) { currentLife = maxLifespan; }
            else if (currentLife > 0) { currentLife--; }
        } else {
            if (currentLife != -1) {
                if(maxLifespan <= 64){ maxLifespan *= 2; }
                currentLife = -1;
            }
        }

        vector<int> movedObjects;

        int objectListSize = objects.size();

        //prune any dead objects from the tree.
        for (int i = 0; i < objectListSize; i++) {
            if (!objects[i].shouldDelete) {
                objects.erase(objects.begin() + i);
                objectListSize--;
            }
        }

        objectListSize = objects.size();

        //go backwards so that moving them later doesnt mess up the order.
        for (int i = objects.size() - 1; i >= 0; i--) {
            //we should figure out if an object actually moved so that we know whether we need to update this node in the tree.
            if (objects[i].shouldMove) {
                movedObjects.push_back(i);
            }
        }



        //recursively update any child nodes.
        for (int flags = active_nodes, index = 0; flags > 0; flags >>=1, index++) {
            if ((flags & 1) == 1) children[index] -> update();
        }



        //If an object moved, we can insert it into the parent and that will insert it into the correct tree node.
        //note that we have to do this last so that we don't accidentally update the same object more than once per frame.
        for (int toMove:movedObjects) {
            ofxColorTree current = this;

            //figure out how far up the tree we need to go to reinsert our moved object
            //try to move the object into an enclosing parent node until we've got full containment
            while (!current.region.contains(objects[toMove])) {
                if (current.parent != nullptr) { current = current.parent; }
                else break; //prevent infinite loops when we go out of bounds of the root node region
            }

            //now, remove the object from the current node and insert it into the current containing node.
            current.insert(objects.move(toMove));   //this will try to insert the object as deep into the tree as we can go.
            objects.erase(objects.begin() + toMove);
        }

        //prune out any dead branches in the tree
        for (int flags = active_nodes, index = 0; flags > 0; flags >>= 1, index++) {
            if ((flags & 1) == 1 && children[index] -> currentLife == 0) {
                children[index] = nullptr;
                active_nodes ^= (bitset<8>)(1 << index);       //remove the node from the active nodes flag list
            }
        }
    }
    else
    {

    }
 };

 template <class ObjectClass>
 void ofxColorTree<ObjectClass>::insert(ObjectClass *item) {
    /*make sure we're not inserting an object any deeper into the tree than we have to.
     -if the current node is an empty leaf node, just insert and leave it.*/
    if (objects.size() <= 1 && active_nodes == 0) {
        objects.push_back(ColorTreeNode(item));
        return;
    }

    ofVec3f dimensions = region.getMax() - region.getMin();

    //Check to see if the dimensions of the box are greater than the minimum dimensions
    if (dimensions.x <= MIN_SIZE && dimensions.y <= MIN_SIZE && dimensions.z <= MIN_SIZE) {
        objects.push_back(ColorTreeNode(item));
        return;
    }

    ofVec3f min = region.getMin();
    ofVec3f max = region.getMax();
    ofVec3f half = dimensions / 2.0;
    ofVec3f center = min + half;

    //Find or create subdivided regions for each octant in the current region
    BoundingBox childOctant[8] = {
        (children[0] != nullptr) ? children[0] -> region : BoundingBox(min, center),
        (children[1] != nullptr) ? children[1] -> region : BoundingBox(ofVec3f(center.x, min.y, min.z), ofVec3f(max.x, center.y, center.z)),
        (children[2] != nullptr) ? children[2] -> region : BoundingBox(ofVec3f(center.x, min.y, center.z), ofVec3f(max.x, center.y, max.z)),
        (children[3] != nullptr) ? children[3] -> region : BoundingBox(ofVec3f(min.x, min.y, center.z), ofVec3f(center.x, center.y, max.z)),
        (children[4] != nullptr) ? children[4] -> region : BoundingBox(ofVec3f(min.x, center.y, min.z), ofVec3f(center.x, max.y, center.z)),
        (children[5] != nullptr) ? children[5] -> region : BoundingBox(ofVec3f(center.x, center.y, min.z), ofVec3f(max.x, max.y, center.z)),
        (children[6] != nullptr) ? children[6] -> region : BoundingBox(center, max),
        (children[7] != nullptr) ? children[7] -> region : BoundingBox(ofVec3f(min.x, center.y, center.z), ofVec3f(center.x, max.y, max.z))
    };


    if (region.contains(item)) {
        bool found = false;
        //we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
        for(int a=0;a<8;a++) {
            //is the object fully contained within a quadrant?
            if (childOctant[a].contains(item)) {
                if (children[a] != nullptr) {
                    children[a] -> insert(item);   //Add the item into that tree and let the child tree figure out what to do with it
                } else {
                    vector<ObjectClass*> new_objects;
                    new_objects.push_back(item);
                    children[a] = new ofxColorTree(childOctant[a], new_objects);
                    children[a] -> parent = this;
                    active_nodes |= (bitset<8>)(1<<a);
                }
                found = true;
            }
        }
        if(!found) { objects.push_back(ColorTreeNode(item)); }

    } else {
        //either the item lies outside of the enclosed bounding box or it is intersecting it. Either way, we need to rebuild
        //the entire tree by enlarging the containing bounding box
        //BoundingBox enclosingArea = FindBox();
        buildTree();
    }
 };
