 
 //  BoundingBox.hpp
 //  tree
 //
 //  Created by kevin ambrosia on 7/11/17.
 //
 //

 #pragma once
 #include "ofMain.h"

 class BoundingBox {
 public:
     BoundingBox() {
         min = ofVec3f::zero();
         max = ofVec3f::zero();
         dim = ofVec3f::zero();
         bright = ofVec3f(255,255,255);
     };

     BoundingBox(ofVec3f _min, ofVec3f _max) {
         min = _min;
         max = _max;
         dim = ofVec3f::zero();
         bright = ofVec3f(255,255,255);
     };

     BoundingBox(ofVec3f _min, ofVec3f _max, ofVec3f _bright, ofVec3f _dim) {
         min = _min;
         max = _max;
         dim = _dim;
         bright = _bright;
     };

     void setMax(ofVec3f newMax) { max = newMax; };
     void setMin(ofVec3f newMin) { min = newMin; };
     bool contains(ofVec3f & point){
         return min.x<=(point.x)<=max.x && min.y<=(point.y)<=max.y && min.z<=(point.z)<=max.z;
     }
     bool contains(ofVec3f * point) {
         return min.x<=(point -> x)<=max.x && min.y<=(point -> y)<=max.y && min.z<=(point -> z)<=max.z;
     };
     bool containsPoint(ofVec3f & point);
     bool containsColor(ofVec3f & color);

     ofVec3f getMax()    { return max; };
     ofVec3f getMin()    { return min; };
     ofVec3f getBright() { return bright; };
     ofVec3f getDim()    { return dim; };

 private:
     ofVec3f max;
     ofVec3f min;
     ofVec3f bright;
     ofVec3f dim;
 };
