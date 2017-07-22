 //
 //  SpaceTree.cpp
 //  tree
 //
 //  Created by kevin ambrosia on 6/30/17.
 //
 //

 #include "ofxColorTree.h"

 template <class ObjectClass>
 void ofxColorTree<ObjectClass>::setup() {
    if (!treeBuilt) {
        while (pending.size() != 0) {
            NodeData<ObjectClass> item = pending.front();
            item.color = region.getColor(item.object);
            objects.push_back(item);
            pending.pop_front();
        }
        buildTree();
    } else {
        while (pending.size() != 0) {
            insert(pending.front());
            pending.pop_front();
        }
    }

    treeReady = true;
 };

 template <class ObjectClass>
 void ofxColorTree<ObjectClass>::buildTree() {
    if (objects.size() < 1) {
        treeBuilt = true;
        return;
    } //we're at a leaf

    ofVec3f dimensions = region.getMax() - region.getMin();

    if (dimensions == ofVec3f::zero()) {
        findEnclosingCube();
        dimensions = region.getMax() = region.getMin();
    }

    if (dimensions.x <= MIN_SIZE && dimensions.y <= MIN_SIZE && dimensions.z <= MIN_SIZE) {
        treeBuilt = true;
        return;
    }

     ofVec3f spectrum = region.getBright() - region.getDim();
     
     ofVec3f min = region.getMin();
     ofVec3f dim = region.getDim();
     ofVec3f max = region.getMax();
     ofVec3f bright = region.getBright();
     
     ofVec3f halfDimension = dimensions / 2.0;
     ofVec3f halfSpectrum = spectrum / 2.0;
     
     ofVec3f center = min + halfDimension;
     ofVec3f mid = dim + halfSpectrum;

    vector<BoundingBox> octant = {
        BoundingBox(min, center, dim, mid),
        BoundingBox(ofVec3f(center.x, min.y, min.z), ofVec3f(max.x, center.y, center.z), ofVec3f(mid.x, dim.y, dim.z), ofVec3f(bright.x, mid.y, mid.z)),
        BoundingBox(ofVec3f(center.x, min.y, center.z), ofVec3f(max.x, center.y, max.z), ofVec3f(mid.x, dim.y, mid.z), ofVec3f(bright.x, mid.y, bright.z)),
        BoundingBox(ofVec3f(min.x, min.y, center.z), ofVec3f(center.x, center.y, max.z), ofVec3f(dim.x, dim.y, mid.z), ofVec3f(mid.x, mid.y, bright.z)),
        BoundingBox(ofVec3f(min.x, center.y, min.z), ofVec3f(center.x, max.y, center.z), ofVec3f(dim.x, mid.y, dim.z), ofVec3f(mid.x, bright.y, mid.z)),
        BoundingBox(ofVec3f(center.x, center.y, min.z), ofVec3f(max.x, max.y, center.z), ofVec3f(mid.x, mid.y, dim.z), ofVec3f(bright.x, bright.y, mid.z)),
        BoundingBox(center, max, mid, bright),
        BoundingBox(ofVec3f(min.x, center.y, center.z), ofVec3f(center.x, max.y, max.z), ofVec3f(dim.x, mid.y, mid.z), ofVec3f(mid.x, bright.y, bright.z))
    };

    vector<NodeData<ObjectClass>> list[8];
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
            active_nodes |= (byte)(1<<a);
            children[a] -> setup();
            hasChildren = true;
        }
    }

    treeBuilt = true;
    treeReady = true;
 };

 template <class ObjectClass>
 void ofxColorTree<ObjectClass>::findEnclosingCube() {

 };

template <class ObjectClass>
void ofxColorTree<ObjectClass>::drawRegion() {
    ofPushMatrix();
    ofSetColor(255,255,255);
    region.drawBox();
    ofPopMatrix();
    if(hasChildren) {
        for (int flags = active_nodes, index = 0; flags > 0; flags >>=1, index++) {
            if ((flags & 1) == 1) {
                children[index] -> drawRegion();
            }
        }
    }

    for (NodeData<ObjectClass> entry : objects) {
        ofPushMatrix();
        ofSetColor(entry.color.x, entry.color.y, entry.color.z);
        ofDrawSphere(entry.object -> x, entry.object -> y, entry.object -> z, 0.1);
        ofPopMatrix();
    }
}

 template <class ObjectClass>
 void ofxColorTree<ObjectClass>::update() {
     
    if (treeBuilt == true)  {
        //Start a count down death timer for any leaf nodes which don't have objects or children.
        //when the timer reaches zero, we delete the leaf. If the node is reused before death, we double its lifespan.
        //this gives us a "frequency" usage score and lets us avoid allocating and deallocating memory unnecessarily
        if (objects.size() == 0) {
            if (hasChildren == false) {
                if (currentLife == -1) { currentLife = maxLifespan; }
                else if (currentLife > 0) { currentLife--; }
            }
        } else {
            if (currentLife != -1) {
                if(maxLifespan <= 64){ maxLifespan *= 2; }
                currentLife = -1;
            }
        }

        vector<int> movedObjects;

        int objectListSize = objects.size();

        //prune any dead objects from the tree.
        for (int i = 0; i < objects.size(); i++) {
            
//            if (objects[i].object -> shouldDelete) {
//                objects.erase(objects.begin() + i);
//                objectListSize--;
//            }
        }

        objectListSize = objects.size();

        //go backwards so that moving them later doesnt mess up the order.
        for (int i = objects.size() - 1; i >= 0; i--) {
            //we should figure out if an object actually moved so that we know whether we need to update this node in the tree.
//            if (objects[i].object -> shouldMove) {
//                movedObjects.push_back(i);
//            }
        }


        //recursively update any child nodes.
        for (int flags = active_nodes, index = 0; flags > 0; flags >>=1, index++) {
            if ((flags & 1) == 1) {
                
                children[index] -> update();
            }
        }



        //If an object moved, we can insert it into the parent and that will insert it into the correct tree node.
        //note that we have to do this last so that we don't accidentally update the same object more than once per frame.
        for (int toMove:movedObjects) {
            ofxColorTree *current = this;

            //figure out how far up the tree we need to go to reinsert our moved object
            //try to move the object into an enclosing parent node until we've got full containment
            while (!current -> region.contains(objects[toMove].object)) {
                if (current -> parent != nullptr) { current = current -> parent; }
                else break; //prevent infinite loops when we go out of bounds of the root node region
            }

            //now, remove the object from the current node and insert it into the current containing node.
            current -> insert(objects[toMove]);   //this will try to insert the object as deep into the tree as we can go.
            objects.erase(objects.begin() + toMove);
        }

        int childCount = 0;
        int deletedCount = 0;
        //prune out any dead branches in the tree
        for (int flags = active_nodes, index = 0; flags > 0; flags >>= 1, index++) {
            if((flags & 1) == 1 ) {
                childCount++;
                if (children[index] -> currentLife == 0) {
                    children[index] = nullptr;
                    active_nodes ^= (byte)(1 << index);       //remove the node from the active nodes flag list
                    deletedCount++;
                }
            }
        }
        
        if (deletedCount > 0 && deletedCount == childCount) {
            hasChildren = false;
        }
    } else {
        buildTree();
    }
 };

template <class ObjectClass>
ofxColorTreeNode<ObjectClass> ofxColorTree<ObjectClass>::insert(ObjectClass *item) {
     NodeData<ObjectClass> node = NodeData<ObjectClass>(item);
     insert(node);
     
     return ofxColorTreeNode<ObjectClass>(&node);
};

template <class ObjectClass>
void ofxColorTree<ObjectClass>::insert(NodeData<ObjectClass> & item) {
    /*make sure we're not inserting an object any deeper into the tree than we have to.
     -if the current node is an empty leaf node, just insert and leave it.*/
    if (objects.size() < 1 && active_nodes == 0) {
        item.color = region.getColor(item.object);
        objects.push_back(item);
        return;
    }
    
    ofVec3f dimensions = region.getMax() - region.getMin();
    
    //Check to see if the dimensions of the box are greater than the minimum dimensions
    if (dimensions.x <= MIN_SIZE && dimensions.y <= MIN_SIZE && dimensions.z <= MIN_SIZE) {
        item.color = region.getColor(item.object);
        objects.push_back(item);
        return;
    }
    
    ofVec3f spectrum = region.getBright() - region.getDim();
    
    ofVec3f min = region.getMin();
    ofVec3f dim = region.getDim();
    ofVec3f max = region.getMax();
    ofVec3f bright = region.getBright();
    
    ofVec3f halfDimension = dimensions / 2.0;
    ofVec3f halfSpectrum = spectrum / 2.0;

    ofVec3f center = min + halfDimension;
    ofVec3f mid = dim + halfSpectrum;
    
    //Find or create subdivided regions for each octant in the current region
    vector<BoundingBox> childOctant;
    for (int flags = active_nodes, index = 0; index < 8; flags >>=1, index++) {
        if ((flags & 1) == 1) {
            childOctant.push_back(children[index]->region);
        } else {
            switch(index) {
                case 0:
                    childOctant.push_back(BoundingBox(min, center, dim, mid));
                    break;
                case 1:
                    childOctant.push_back(BoundingBox(ofVec3f(center.x, min.y, min.z), ofVec3f(max.x, center.y, center.z), ofVec3f(mid.x, dim.y, dim.z), ofVec3f(bright.x, mid.y, mid.z)));
                    break;
                case 2:
                    childOctant.push_back(BoundingBox(ofVec3f(center.x, min.y, center.z), ofVec3f(max.x, center.y, max.z), ofVec3f(mid.x, dim.y, mid.z), ofVec3f(bright.x, mid.y, bright.z)));
                    break;
                case 3:
                    childOctant.push_back(BoundingBox(ofVec3f(min.x, min.y, center.z), ofVec3f(center.x, center.y, max.z), ofVec3f(dim.x, dim.y, mid.z), ofVec3f(mid.x, mid.y, bright.z)));
                    break;
                case 4:
                    childOctant.push_back(BoundingBox(ofVec3f(min.x, center.y, min.z), ofVec3f(center.x, max.y, center.z), ofVec3f(dim.x, mid.y, dim.z), ofVec3f(mid.x, bright.y, mid.z)));
                    break;
                case 5:
                    childOctant.push_back(BoundingBox(ofVec3f(center.x, center.y, min.z), ofVec3f(max.x, max.y, center.z), ofVec3f(mid.x, mid.y, dim.z), ofVec3f(bright.x, bright.y, mid.z)));
                    break;
                case 6:
                    childOctant.push_back(BoundingBox(center, max, mid, bright));
                    break;
                case 7:
                    childOctant.push_back(BoundingBox(ofVec3f(min.x, center.y, center.z), ofVec3f(center.x, max.y, max.z), ofVec3f(dim.x, mid.y, mid.z), ofVec3f(mid.x, bright.y, bright.z)));
                    break;
            }
        }
    }

    
    
    if (region.contains(item.object)) {
        bool found = false;
        //we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
        for (int flags = active_nodes, a = 0; a < 8; flags >>=1, a++) {
            //is the object fully contained within a quadrant?
            if (childOctant[a].contains(item.object)) {
                if ((flags & 1) == 1) {
                    children[a] -> insert(item);   //Add the item into that tree and let the child tree figure out what to do with it
                } else {
                    vector<NodeData<ObjectClass>> new_objects;
                    new_objects.push_back(item);
                    children[a] = new ofxColorTree(childOctant[a], new_objects);
                    children[a] -> parent = this;
                    active_nodes |= (byte)(1<<a);
                    children[a] -> setup();
                    hasChildren = true;
                }
                found = true;
            }
        }
        if(!found) {
            item.color = region.getColor(item.object);
            objects.push_back(item);
        }
        
    } else {
        //either the item lies outside of the enclosed bounding box or it is intersecting it. Either way, we need to rebuild
        //the entire tree by enlarging the containing bounding box
        //BoundingBox enclosingArea = FindBox();
        buildTree();
    }
};

template <class ObjectClass>
void ofxColorTree<ObjectClass>::getClosest(ObjectClass point, Closest & closestInfo) {
    if (hasChildren) {
        
        // we have no starting point yet
        // so get one
        if (closestInfo.item == NULL) {
            // check only active octants
            for (int flags = active_nodes, i = 0; i < 8; flags >>=1, i++) {
                if ((flags & 1) == 1) {
                    children[i] -> getClosest(point, closestInfo);
                    if (closestInfo.item != NULL) {
                        // now we have a starting point
                        getClosest(point, closestInfo);
                        return;
                    }
                }
            }
        } else {
            // we have a item set as closest
            // we should check if this octant is
            // closer then the current closest distance
            for (int flags = active_nodes, i = 0; i < 8; flags >>=1, i++) {
                if ((flags & 1) == 1) {
//                    ofVec3f center = children[i] -> region.getCenter();
//                    float dist = ofDistSquared(point.x, point.y, point.z, center.x, center.y, center.z);
                    //check if region is contained within radius of dist
                    if (region.intersects(point, closestInfo.dist)) {
                        children[i] -> getClosest(point, closestInfo);
                    }

                }
            }
        }
        
    } else {
        
        for (int i = 0; i < objects.size(); i++) {
            
            ObjectClass * item = objects[i].object;
            
            float dist = ofDistSquared(point.x, point.y, point.z, item -> x, item -> y, item -> z);
            
            if (dist < closestInfo.dist) {
                closestInfo.dist = dist;
                closestInfo.item = item;
                closestInfo.tree = this;
            }
            
        }
    }
    
}

template <class ObjectClass>
ObjectClass * ofxColorTree<ObjectClass>::getClosestByColor(ObjectClass color) {
    ofVec3f point = region.getPoint(color);
    
    return getClosestByPoint(point);
};

template <class ObjectClass>
ObjectClass * ofxColorTree<ObjectClass>::getClosestByColor(ObjectClass *color) {
    ofVec3f point = region.getPoint(color);
    return getClosestByPoint(point);
};

template <class ObjectClass>
ObjectClass * ofxColorTree<ObjectClass>::getClosestByColor(float r, float g, float b) {
    ofVec3f point = region.getPoint(ofVec3f(r, g, b));
    return getClosestByPoint(point);
};

template <class ObjectClass>
ObjectClass * ofxColorTree<ObjectClass>::getClosestByPoint(ObjectClass *point) {
    return getClosestByPoint(ObjectClass(point -> x, point -> y, point -> z));
};

template <class ObjectClass>
ObjectClass * ofxColorTree<ObjectClass>::getClosestByPoint(float x, float y, float z) {
    return getClosestByPoint(ObjectClass(x, y, z));
};

template <class ObjectClass>
ObjectClass * ofxColorTree<ObjectClass>::getClosestByPoint(ObjectClass point) {
    Closest closest = Closest();
    getClosest(point, closest);
    
    return closest.item;
};




