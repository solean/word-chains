/* File "ChainClientCJS.cpp", written by Chris Schnabel Spring '14 for cse250 */

#include <string>
#include <vector>
#include <istream>
#include <fstream>
#include <sstream>
#include "StringWrap.cpp"
#include "PeekDeque3.h"

using namespace std;

//hd1 is needed for the ed1 implementation
bool hd1(const string& x, const string& y) {
    if (x.size() != y.size()) { return false; }
    int count = 0;
    for (int i = 0; i < y.size(); i++) {
        if (x.at(i) != y.at(i)) { count++; }
    }
    return count == 1;
}

//ed1 returns true if the two strings passed in have edit distance 1
bool ed1(const string& x, const string& y) {
    if (x.size() < 1 || y.size() < 1) { return false; }
    int left = 0;
    int right;
    if (x.size() == y.size()) {
        return hd1(x,y);
    } else if (x.size() == 1 + y.size()) {//check for delete in first string
        //LOOP INV: All chars to left of "left" match, all to right of "right"

        //Hence an extra char in x will eventually give right <= left
        right = x.size() - 1;
        while (left < right && x.at(left) == y.at(left)) { left++; }
        while (right > left && x.at(right) == y.at(right-1)) { right--; }
        return right == left; //all chars matched except this extra one.
    } else if (1 + x.size() == y.size()) { //check for delete in first string
        //LOOP INV: all chars to left of "left" match, all to right of "right"

        //Hence an extra char in y will eventually give right <= left.
        right = y.size() - 1;
        while (left < right && x.at(left) == y.at(left)) { left++; }
        while (right > left && x.at(right-1) == y.at(right)) { right--; }
        return right == left; //all chars matched except this extra one
    } else {
        return false;
    }
}

//This method places a word in an existing chain or a new one if it can't be
//placed in an existing chain
void placeWord(const string& word, vector<PeekDeque< StringWrap>* >* chains, const int& count) {

    //"normalize" word, trim of punctuation, and make lowercase
    StringWrap wrap;
    wrap = StringWrap(word);
    wrap.trimNonAlpha();
    wrap.makeLower();

    bool added = false;

    //if word is the first read, add to the single, empty PeekDeque
    if (count == 1) {
        chains->at(0)->pushRear(wrap);
    }
    else {
        //check if there is a match in any existing chains
        for (size_t i = 0; i != chains->size(); i++) {

            PeekDeque<StringWrap>* cpd = chains->at(i); //current PeekDeque
            
            //check rear of current PeekDeque for a match
            if (ed1(wrap.str(), cpd->peekRear().str())) {
                if (wrap.str().size() > 2) {
                    cpd->pushRear(wrap);
                    added = true;
                    break;
                }
            }
            //check front of current PeekDeque for a match
            if (ed1(wrap.str(), cpd->peekFront().str())) {
                if (wrap.str().size() > 2) {
                    cpd->pushFront(wrap);
                    added = true;
                    break;
                }
            }
        }

        //if there is no where to add word to a chain, make a new chain
        if (!added) {
            PeekDeque<StringWrap>* newpd = new PeekDeque<StringWrap>(500);
            newpd->pushRear(wrap);
            chains->push_back(newpd);
            added = true;
        }
    }
    
}

int main(int argc, char *argv[]) {
    vector< PeekDeque<StringWrap>* >* chains = new vector< PeekDeque<StringWrap>* >(); 
    chains->push_back(new PeekDeque<StringWrap>(500));
    string infileName = "";

    if (argc == 2) { infileName = argv[1]; }
    else { 
        cerr << "Error: no file named in command line." << endl;
        return 0;
    }
    ifstream* INFILEp = new ifstream(infileName.c_str(), ios_base::in);
    string word;

    int count = 1;//count represents the index of the current word in the infile
    while ((*INFILEp) >> word) {
        placeWord(word,chains,count);
        count++;
    }

    //find longest chain
    int max = 0;
    int indexOfMax;
    for (int i = 0; i < chains->size(); i++) {
        if (chains->at(i)->size() > max) {
            max = chains->at(i)->size();
            indexOfMax = i;
        }
    }

    //find longest word in a chain
    int longest = 0;
    string longestWord = "";
    int indexOfLong;
    for (int i = 0; i < chains->size(); i++) {
        PeekDeque<StringWrap>* cpd = chains->at(i);
        for (cpd->setPeekToFront(); !cpd->atRear(); cpd->moveRearward()) {
            if ((cpd->peek().str().size() > longest) && (cpd->size() > 1)) {
                longestWord = cpd->peek().str();
                longest = cpd->peek().str().size();
                indexOfLong = i;
            }
        }
    }
            
    //output report on longest chain and longest word in a chain
    cout << "The longest chain found is " << max << " words long." << endl;
    cout << "Here it is: " << endl;
    cout << chains->at(indexOfMax)->toString() << endl << endl;

    cout << "The longest word in a chain is: " << longestWord << endl;
    cout << "The chain with the longest word is: " << endl;
    cout << chains->at(indexOfLong)->toString() << endl << endl;
    
    
    INFILEp->close();
}
