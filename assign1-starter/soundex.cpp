/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* TThis function takes a string s and returns a version of
 * the string that has all non-letter characters removed
 * from it. A bug in the index has been fixed, by reducing by 1.
*/
string removeNonLetters(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isalpha(s[i])) {
            // remove the character at index i
            s = s.substr(0,i) + s.substr(i+1);
            i--;
        }
    }
    return s;
}

/* converToDigits, a helper function, receives a (full) surname and converts
 * every letter into a number from 0-6, per Soundex guidelines.
 * It achieves this by first initializing a vector with the Soundex
 * table, and then loops through every character in the user's input
 * string to finally assign the corresponding value. Returns a string
 * of digits.
 */
string convertToDigits(string name) {
    // Initialize soundex digit-letter table and string to be returned
    string digits = "";
    string zero = "AEIOUHWY";
    string one = "BFPV";
    string two = "CGJKQSXZ";
    string three = "DT";
    string four = "L";
    string five = "MN";
    string six = "R";
    // Populate vector
    Vector<string> letters;
    letters.add(zero);
    letters.add(one);
    letters.add(two);
    letters.add(three);
    letters.add(four);
    letters.add(five);
    letters.add(six);

    // Loop through characters in input string
    for (int i = 0; i < name.length(); i++) {
        char elemchar = name[i];
        string elemstr = toUpperCase(charToString(elemchar));
        for (int j = 0; j < letters.size(); j++) {
            string value = integerToString(j);
            string sdxstr = letters[j];
            if (sdxstr.find(elemstr) != string::npos) {
                digits.append(value);
                }
            }
    }
    return digits;
}

/* formatDigits is a helper function to appropriately format the
 * code, per soundex guidelines. The steps are as follows:
 * 1. Coalesces adjacent duplicates
 * 2. Replaces first digit by the first letter in the surname.
 * 3. Removes all zeros from the code.
 * 4. Pads the string with zeros at the end if the length is less
 *      than 4, otherwise, it truncates the length of the code by
 *      removing the characters after the 4th digit.
 */
string formatDigits(string first, string converted) {
    // Coalesce adjacent duplicates
    for (int i = 0; i < converted.length(); i++) {
        if (i < converted.length() && converted[i] == converted[i + 1]) {
            converted.erase(i,1);
            i--;
        }
    }
    // Remove first digit, replace with first surname letter
    string del = converted.erase(0,1);
    string temp = toUpperCase(first) + converted;
    // Remove all zeros from code
    string zero = "0";
    string empty = "";
    string final = stringReplace(temp.substr(0),zero,empty);
    // Provide padding with zeros, or truncate excess code5
    while(final.length() < 4) {
        final.append(zero);
    }
    while(final.length() > 4) {
        final.pop_back();
    }
    return final;
}

/* This function receives a string and encodes it according
 * to the Soundex guide. It first converts the string into digits,
 * then retrieves and stores the first letter of the surname.
 * Lastly, it formats the digits to the appropriate configuration.
 * Returns a formated 4-digit code.
 */
string soundex(string s) {
    /* TODO: Fill in this function. */
    string converted = convertToDigits(s);
    string first = toUpperCase(charToString(s[0]));
    string formatted = formatDigits(first,converted);
    return formatted;
}


/* This function receives user input (surname) and converts it
 * to a simplyfied code. Using a database, this surname is matched
 * to other surnames. Finally, the function prints out all the
 * matches found in the database.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames
    string input = getLine("Enter a surname (RETURN to quit): ");
    int flag = 0;
    while (flag != 1) {
        string encode = soundex(input);
        Vector<string> listNames;
        for (int i = 0; i < databaseNames.size(); i++) {
            string name = databaseNames[i];
            string namecode = soundex(databaseNames[i]);
            if (encode == namecode) {
                listNames.add(name);
                listNames.sort();
            }

        }
        // User indicates to quit or continue
        if (input == "") {
            flag++;
            break;
        }
        // Print out matches
        cout << "Soundex code is ";
        cout << encode << endl;
        cout << "Matches from databes: ";
        cout << listNames << endl;
        // Get new user input and repeat
        input = getLine("Enter a surname (RETURN to quit): ");
    }
    cout << "Program ended by user." << endl;
}

/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O''Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// Student tests
STUDENT_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Ha'ra";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "106B CS";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "BCS");
    s = "";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "O'hara&Mwy9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OharaMwy");
}

STUDENT_TEST("Test conversion of multiple names into a string of equivalent digits, per soundex table") {
    EXPECT_EQUAL(convertToDigits("Camacho"),"2050200");
    EXPECT_EQUAL(convertToDigits("Pascual Camacho"),"10220042050200");
    EXPECT_EQUAL(convertToDigits("Pascual-&&--Camacho"),"10220042050200");
    EXPECT_EQUAL(convertToDigits("A12?$%^"),"0");
}

STUDENT_TEST("Test helper function for adding and removing -missing or excess, resp.- zeros to achieve 4 digit code") {
    EXPECT_EQUAL(formatDigits("C","2050200"),"C520");
    EXPECT_EQUAL(formatDigits("S","23051603"),"S351");
}

