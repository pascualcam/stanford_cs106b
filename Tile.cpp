/*
 * This file contains multiple functions that together
 * solve if one tile is matched to other sorrounding
 * tiles.
 *
 * Pair Programming:
 *
 * Gonzalo Aguilar - gonzaloa
 * Pascual Camacho - ecam
 *
 */

#include "Tile.h"
#include "error.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
#include "vector.h"
#include <cctype>

using namespace std;

// Default Constructor
Tile::Tile() {
    sides.add("X");
    sides.add("X");
    sides.add("x");
    sides.add("x");
    orientation = 0; // top faces up
}

// Constructor
Tile::Tile(string s) {
    // Parse sides
    sides.add(s.substr(0, 1));
    sides.add(s.substr(2, 1));
    sides.add(s.substr(4, 1));
    sides.add(s.substr(6, 1));

    // Parse orientation
    if (s.size() > 8) {
        orientation = stringToInteger(s.substr(8, 1));
    }
    else {
        orientation = 0; //Default optional orientation
    }
}

int Tile::getOrientation() {
    return orientation;
}

void Tile::setOrientation(int newOrientation) {
    orientation = newOrientation;
}

string Tile::sidesStr() {
    string strsides;
    for (int i = 0; i < sides.size(); i++) {
        strsides += sides[i];
    }
    return strsides;
}


/*
 * This function takes one tile as well as conection
 * and returns wether or not the current tile is matched
 * to the tile taken by the function in the specified
 * direction.
 */
bool Tile::isMatched(Tile &other, Connection otherLocation) {

    // Get other tile sides and location
    Vector<string> otherSides = other.sides;
    int otherOrientation = other.orientation;

    // Initialize tile sides
    string currentTileSide;
    string otherTileSide;

    // Check Tile Above
     if (otherLocation==Tile::ABOVE) {
         currentTileSide = sides[(4 + ((4-orientation) % 4)) % 4];
         otherTileSide = otherSides[(4 + ((2-otherOrientation) % 4)) % 4];

         // If both tiles have the same letter AND one letter is uppercase and the other is lowercase, then match
         if (toUpperCase(currentTileSide) == toUpperCase(otherTileSide) && currentTileSide != otherTileSide) {
             return true;
         }
     }

    // Check Tile to the Right
     if (otherLocation==Tile::RIGHT) {
         currentTileSide = sides[(4 + ((1-orientation) % 4)) % 4];
         otherTileSide = otherSides[(4 + ((3-otherOrientation) % 4)) % 4];


         // If both tiles have the same letter AND one letter is uppercase and the other is lowercase, then match
         if (toUpperCase(currentTileSide) == toUpperCase(otherTileSide) && currentTileSide != otherTileSide) {
             return true;
         }
     }

     // Check Tile Below
     if (otherLocation==Tile::BELOW) {
         currentTileSide = sides[(4 + ((2-orientation) % 4)) % 4];
         otherTileSide = otherSides[(4 + ((4-otherOrientation) % 4)) % 4];

         // If both tiles have the same letter AND one letter is uppercase and the other is lowercase, then match
         if (toUpperCase(currentTileSide) == toUpperCase(otherTileSide) && currentTileSide != otherTileSide) {
             return true;
         }
     }

     // Check Tile to the Left
     if (otherLocation==Tile::LEFT) {
         currentTileSide = sides[(4 + ((3-orientation) % 4)) % 4];
         otherTileSide = otherSides[(4 + ((1-otherOrientation) % 4)) % 4];

         // If both tiles have the same letter AND one letter is uppercase and the other is lowercase, then match
         if (toUpperCase(currentTileSide) == toUpperCase(otherTileSide) && currentTileSide != otherTileSide) {
             return true;
         }
     }

    return false;
}

/* The functions below have already been written for you! */

ostream &operator<<(ostream &out, Tile const &tile) {
    // prints as follows:
    // A B C D 0
    // Which means that with an orientation of 0, the piece
    // would look like this:
    //   A
    // C   B
    //   D
    //
    // If we had this:
    // A B C D 3
    // The piece would look like this, because it has been
    // rotated clockwise by 270 degrees:
    //   B
    // A   C
    //   D
    //
    for (string s : tile.sides) {
        out << s << " ";
    }
    out << tile.orientation;
    return out;
}

string Tile::displayTileStr() {
    string s;
    int start = (4 - orientation) % 4;
    s += "  " + sides[start] + "\n";
    s += sides[(start + 3) % 4];
    s += "   " + sides[(start + 1) % 4] + "\n";
    s += "  " + sides[(start + 2) % 4] + "\n";
    return s;
}

PROVIDED_TEST("Testing Display Form") {
    Tile t1("A a B b 3");
    stringstream ss;
    ss << t1.displayTileStr();
    EXPECT("  a\nA   B\n  b\n" == ss.str());
}

PROVIDED_TEST("Testing Display Form") {
    Tile t1("C c B a");
    stringstream ss;
    ss << t1.displayTileStr();
    EXPECT("  C\na   c\n  B\n" == ss.str());
}

PROVIDED_TEST("Testing << overload") {
    Tile t1("A a B b 3");
    stringstream ss;
    ss << t1;
    EXPECT_EQUAL(ss.str(), "A a B b 3");
}

PROVIDED_TEST("Testing << overload") {
    Tile t1("C c B a");
    stringstream ss;
    ss << t1;
    EXPECT_EQUAL(ss.str(), "C c B a 0");
}

PROVIDED_TEST("Testing getOrientation()") {
    Tile t1("A a B b 3");
    Tile t2("C c B a");
    EXPECT_EQUAL(3, t1.getOrientation());
    EXPECT_EQUAL(0, t2.getOrientation());
}

PROVIDED_TEST("Testing setOrientation()") {
    Tile t1("A a B b 3");
    Tile t2("C c B a");
    t1.setOrientation(1);
    t2.setOrientation(1);
    stringstream ss1, ss2;
    ss1 << t1;
    ss2 << t2;
    EXPECT_EQUAL(1, t1.getOrientation());
    EXPECT_EQUAL(1, t2.getOrientation());
    EXPECT_EQUAL("A a B b 1", ss1.str());
    EXPECT_EQUAL("C c B a 1", ss2.str());
}

PROVIDED_TEST("Testing isMatched()") {
    Tile t1("A a B b 3");
    Tile t2("C c B a 2");

    /*
        t1:
           a
         A   B
           b

    t2:
        B
                    c   a
                        C
                            */

    cout << endl << "t1:" << endl;
    cout << t1.displayTileStr() << endl;
    cout << endl << "t2:" << endl;
    cout << t2.displayTileStr() << endl;

    cout << "if t2 is above t1, no match" << endl;
    EXPECT(!t1.isMatched(t2, Tile::ABOVE));

    cout << "if t2 is below t1, match" << endl;
    EXPECT(t1.isMatched(t2, Tile::BELOW));

    cout << "if t2 is to the left of t1, match" << endl;
    EXPECT(t1.isMatched(t2, Tile::LEFT));

    cout << "if t2 is to the right of t1, no match" << endl;
    EXPECT(!t1.isMatched(t2, Tile::RIGHT));

    cout << "if t1 is above t2, match" << endl;
    EXPECT(t2.isMatched(t1, Tile::ABOVE));

    cout << "if t1 is below t2, no match" << endl;
    EXPECT(!t2.isMatched(t1, Tile::BELOW));

    cout << "if t1 is to the left of t2, no match" << endl;
    EXPECT(!t2.isMatched(t1, Tile::LEFT));

    cout << "if t1 is to the right of t2, match" << endl;
    EXPECT(t2.isMatched(t1, Tile::RIGHT));
}
