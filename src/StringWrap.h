/** File "StringWrap.h", by KWR for CSE250, Fall 2010.  Cut down from 2009.
    For use in Assignment 4, and possibly later.
 */

#ifndef STRINGWRAP_H_
#define STRINGWRAP_H_

#include<string>
#include <vector>
#include <iostream>

using namespace std;

class StringWrap {
   string st;             // this is called "wrapping" a single object.
 public:
   explicit StringWrap(string s);      
   StringWrap();          // zero-parameter constructor
    
   //virtual ~StringWrap() { }    //this class intends no inheritance

   void trimNonAlpha();   // not const---it modifies st
   bool isAlpha() const;
   bool isAllCaps() const;
   void makeLower();      // ASCII-specific!
   
   string str() const;
   int size() const;
};

#endif
