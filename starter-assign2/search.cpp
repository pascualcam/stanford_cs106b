// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/* This function receives a string token and removes non alphabetic
 * characters, including punctuation and numbers.
 * If an empty string is passed, or a string with no alphabetic characaters,
 * the function returns an empty string.
 */
string cleanToken(string s) {
    for (int i=0; i < s.length(); i++) {
        if(ispunct(s[i]) || !isalpha(s[i])) {
            s.erase(i,1);
            i--;
        }
    }
    s = toLowerCase(s);
    return s;
}

/* This function recieves the body of a text as a string and
 * splits the contents by a space delimiter. Each token created
 * is then cleaned from punctuation/numbers with the helper
 * function cleanToken and if the string is not empty, it is
 * added to the Set "tokens". The nature of a set is perfect to
 * only add new items to the Set.
 */
Set<string> gatherTokens(string text){
    Set<string> tokens;
    Vector<string> split = stringSplit(text, " ");
    for (int i = 0; i < split.size(); i++) {
        string cleaned = cleanToken(split[i]);
        if (cleaned != "") {
            tokens.add(cleaned);
        }
    }
    return tokens;
}

/* The parameterrs for this function are the name of a file to read
 * (as a string) and a Map, with keys (as strings) and values ( a Set
 * of strings). The function then opens and processes the lines in the file.
 * It creates tokens (unique words) from the odd lines in the file, which
 * are passed as the keys to the Map. The values of each key are websites, which
 * correspond to even lines in the file read.
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index) {
    ifstream in;
    // File processing
    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);

    // Count websites processed
    int webcount = 0;

    // Extract each URL and BODYTEXT
    // NOTE: this loop operates by pairs; ie, 0 and 1, 2 and 3, etc.
    for (int i = 0; i < lines.size(); i += 2) {

        // For each token in the Set, update index
        Set<string> bodyTokens = gatherTokens(lines[i+1]);
        string website = lines[i];
        for (string word : bodyTokens) {
            // If first time seeing word as KEY
            if (!index.containsKey(word)) {
                Set<string> urls;
                urls.add(website);
                index.put(word, urls);
            // If word already exists as KEY
            } else {
                index[word].add(website);
            }
        }
        webcount++;
    }
    return webcount;
}

/* This function receives two parameters: an index by reference and string provided
 * by the user of the program. It first parses through the users input and stores each
 * word (separated by a space) in a vector (query). It then loops through each word
 * finding the websites where each word was find. The user has the ability to further
 * personalize the search by providing the following prefixes to any additional word:
 *        no symbol returns all websites where each word was found
 *      + returns the websites where both words were found
 *      - returns the websites where the first word was found but NOT the second
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string sentence) {
    Set<string> result;
    Vector<string> query = stringSplit(sentence, " "); // Split user input by space
    for (int i = 0; i < query.size(); i++) {
        string firstword = query[0];
        if (i == 0) {
            result = index[cleanToken(firstword)];
        }
        if (i > 0) {
            string nextword = query[i];

            // Intersection of two sets
            if (startsWith(nextword,"+")) {
                string word = cleanToken(nextword);
                Set<string> sett = index[word];
                result = sett*result;

            // Subtract one set from the other
            } else if (startsWith(nextword,"-")) {
                string word = cleanToken(nextword);
                Set<string> sett = index[word];
                result = result-sett;

            // Union of two sets
            } else if (!stringContains(nextword,"-") || !stringContains(nextword,"+")) {
                Set<string> sett = index[cleanToken(nextword)];
                result = result + sett;
            }
        }
    }
    return result;
}

/* This function receives a file of "websites" and builds
 * an index from such files, mapping every word within a website
 * to the indicated website url. It then gets a user input as a
 * string and provides a list of websited that contain the user's
 * input as output.
 */
void searchEngine(string dbfile) {
    // Initialize Map
    Map<string, Set<string>> webindex;

    // Preliminary information for user
    cout << "Stand by while building index..." << endl;
    int webcount = (buildIndex(dbfile,webindex));
    string n = integerToString(webindex.size());
    cout << "Indexed " << webcount << " pages containing " << n << " unique terms." << endl;
    cout << "" << endl;

    // Prompt user until program is quit
    while (true) {
        string userinput = getLine("Enter query sentence (RETURN to quit): ");
        if (userinput  == "") { // user quits program
            break;
        }

        Set<string> websites = findQueryMatches(webindex, userinput);
        string foundnum = integerToString(websites.size());
        cout << "Found " << foundnum << " matching websites" << endl;
        cout << websites << endl;
        cout << "" << endl;
        websites.clear();
        foundnum.erase();

    }

    cout << "" << endl;
    cout << "Program ended by the user." << endl;
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on tokens with no punctuation") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
}

PROVIDED_TEST("cleanToken on tokens with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word tokens"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> tokens = gatherTokens("to be or not to be");
    EXPECT_EQUAL(tokens.size(), 4);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> seuss = gatherTokens("One Fish Two Fish Red fish Blue fish!!! 123");
    EXPECT_EQUAL(seuss.size(), 5);
    EXPECT(seuss.contains("fish"));
    EXPECT(!seuss.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 20 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 20);
    EXPECT(index.containsKey("fish"));
    EXPECT(!index.containsKey("@"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 3);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// Student tests
PROVIDED_TEST("cleanToken on empty string and all punctuation") {
    EXPECT_EQUAL(cleanToken(""), "");
    EXPECT_EQUAL(cleanToken("~!@#$$%^%^%&^&_+-0=13216598+-*/=/.,,>"), "");
}
PROVIDED_TEST("cleanToken on token with consecutive repeats") {
    EXPECT_EQUAL(cleanToken("C106S"), "cs");
    EXPECT_EQUAL(cleanToken("St4nf03d"), "stnfd");
}
