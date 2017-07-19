 
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
         box = ofBoxPrimitive(abs(min.x - max.x), abs(min.y - max.y), abs(min.z - max.z), 1, 1, 1);
         box.setPosition((min.x + max.x)/2.0, (min.y + max.y)/2.0, (min.z + max.z)/2.0);
     };

     BoundingBox(ofVec3f _min, ofVec3f _max) {
         min = _min;
         max = _max;
         dim = ofVec3f::zero();
         bright = ofVec3f(255,255,255);
         box = ofBoxPrimitive(abs(min.x - max.x), abs(min.y - max.y), abs(min.z - max.z), 1, 1, 1);
         box.setPosition((min.x + max.x)/2.0, (min.y + max.y)/2.0, (min.z + max.z)/2.0);
     };

     BoundingBox(ofVec3f _min, ofVec3f _max, ofVec3f _dim, ofVec3f _bright) {
         min = _min;
         max = _max;
         dim = _dim;
         bright = _bright;
         box = ofBoxPrimitive(abs(min.x - max.x), abs(min.y - max.y), abs(min.z - max.z), 1, 1, 1);
         box.setPosition((min.x + max.x)/2.0, (min.y + max.y)/2.0, (min.z + max.z)/2.0);
     };

     void drawBox() {
         box.drawWireframe();
     }
     
     void setMax(ofVec3f newMax) {
         max = newMax;
         updateBox();
     };
     void setMin(ofVec3f newMin) {
         min = newMin;
         updateBox();
     };
     
     void updateBox() {
         box.setPosition((min.x + max.x)/2.0, (min.y + max.y)/2.0, (min.z + max.z)/2.0);
         box.setWidth(abs(min.x - max.x));
         box.setHeight(abs(min.y - max.y));
         box.setDepth(abs(min.z - max.z));
     };
     
     ofVec3f getColor(ofVec3f point) {
         return ofVec3f(ofMap(point.x, min.x, max.x, dim.x, bright.x), ofMap(point.y, min.y, max.y, dim.y, bright.y), ofMap(point.z, min.z, max.z, dim.z, bright.z));
     };
     ofVec3f getColor(ofVec3f *point) {
         return ofVec3f(ofMap(point -> x, min.x, max.x, dim.x, bright.x), ofMap(point -> y, min.y, max.y, dim.y, bright.y), ofMap(point -> z, min.z, max.z, dim.z, bright.z));
     };
     
     
     bool contains(ofVec3f & point){
         return min.x<=point.x && point.x<=max.x && min.y<=point.y && point.y<=max.y && min.z<=point.z && point.z<=max.z;
     }
     bool contains(ofVec3f * point) {
         return min.x<=point -> x && point -> x<=max.x && min.y<=point -> y && point -> y<=max.y && min.z<=point -> z && point -> z<=max.z;
     };
     bool containsPoint(ofVec3f & point);
     bool containsColor(ofVec3f & color);

     ofVec3f getMax()    { return max; };
     ofVec3f getMin()    { return min; };
     ofVec3f getBright() { return bright; };
     ofVec3f getDim()    { return dim; };
     ofBoxPrimitive box;
 private:
     ofVec3f max;
     ofVec3f min;
     ofVec3f bright;
     ofVec3f dim;
 };
