/* File "PeekDeque3.h", written by Chris Schnabel for cse250
   Assignment #5, Spring '14 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using std::vector;
using std::endl;
using std::cerr;
using std::ostringstream;
using std::string;
using std::cout;


template <typename T>
class PeekDeque;

template <typename T>
class Deque{
    //friend class PeekDeque<T>;

    vector<T>* elements;
    int rearSpace;
    int frontItem;
    int upperBound;

  public:

    friend class PeekDeque<T>;
    
    explicit Deque<T>(int guaranteedCapacity)
        : elements(new vector<T>(2*guaranteedCapacity)),
          frontItem(guaranteedCapacity),
          rearSpace(guaranteedCapacity),
          upperBound(2*guaranteedCapacity) {}

    virtual ~Deque<T>() {
        delete elements;
        cout << "It's Deque Season!" << endl;
    }

    virtual bool empty() const { return frontItem == rearSpace; }

    virtual bool full() const { return rearSpace == upperBound; }

    virtual size_t size() const { return rearSpace - frontItem; }

    virtual void pushFront(T newItem) {
        if (full()) {
            cerr << "Are you playing with a full Deque?" << endl;
            cerr << toString() << endl;
        } else {
            elements->at(--frontItem) = newItem;
        }
    }

    virtual void pushRear(T newItem) {
        if (full()) {
            cerr << "Are you playing with a full Deque?" << endl;
            cerr << toString() << endl;
        } else {
            elements->at(rearSpace++) = newItem;
        }
    }

    virtual T popFront() {
        if (empty()) {
            cerr << "Too lazy to throw an EmptyDequeException." << endl;
            return T();
        } else {
            return elements->at(frontItem++);
        }
    }

    virtual T popRear() {
        if (empty()) {
            cerr << "Too lazy to throw an EmptyDequeException." << endl;
            return T();
        } else {
            return elements->at(--rearSpace); //translates Java "next()"
        }
    }

    virtual string toString() const {
        ostringstream OUT;
        //string out = "";
        for (int i = frontItem; i < rearSpace; i++) {
            //out += elements->at(i).str() + " ";
            OUT << elements->at(i).str() << " ";
        }
        //return out
        return OUT.str();
    }
};


template <class T>
class PeekDeque : public Deque<T> {
    int peekIndex;
    
   public:
    
    explicit PeekDeque<T>(int guaranteedCapacity) 
        : Deque<T>(guaranteedCapacity), peekIndex(this->frontItem) {}

    virtual ~PeekDeque<T>() {
        cerr << "No peeking..." << endl;  // automatically calls ~Deque()
    }

    virtual void moveFrontward() {
        peekIndex--;
    }

    virtual void moveRearward() {
        peekIndex++;
    }

    virtual T popFront() {
        if (this->empty()) {
            cerr << "Attempt to pop from empty PeekDeque" << endl;
            return T();
        }
        else {
            return Deque<T>::popFront();
        }
    }

    virtual T popRear() {
        if (this->empty()) {
            cerr << "Attempt to pop from empty PeekDeque" << endl;
            return T();
        }
        else {
            return Deque<T>::popRear();
        }
    }
    
    virtual T peekFront() {
        if (this->empty()) {
            cerr << "Attempt to peek at empty PeekDeque" << endl;
            return T();
        }
        else {
            return this->elements->at(this->frontItem);
        }
    }

    virtual T peekRear() {
        if (this->empty()) {
            cerr << "Attempt to peek at empty PeekDeque" << endl;
            return T();
        }
        else {
            return this->elements->at(this->rearSpace);
        }
    }

    virtual T peek() const {
       if(this->empty()) {
           cerr << "Can't peek becuase PeekDeque is empty..." << endl;
       }
       else {
           return this->elements->at(peekIndex); 
       }
       return T();
    }

    //Extra functionality

    virtual void setPeekToFront() { peekIndex = this->frontItem; }

    virtual bool atRear() const { return peekIndex == this->rearSpace; }

};
