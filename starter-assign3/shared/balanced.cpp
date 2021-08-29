/*
 * This file contains three functions that together
 * check if operators in a string (passed as parameter)
 * are balanced.
 *
 * Pair Programming:
 *
 * Gonzalo Aguilar - gonzaloa
 * Pascual Camacho - ecam
 *
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/* This function receives a string as input and takes
 * a recursive approach to only returning operators, which
 * are: ()[]{}.
 * The base case is an empty string, and the recursive case
 * is comprised of two different cases: one where the first
 * character of the string is an operator, and one where it
 * is not. The function then returns a string of all the
 * operators in the original string.
 */
string operatorsFrom(string str) {
    string operators = "{[()]}";
    if (str == "") { // base case
        return "";
    }
    else { // recursive case
        if (stringContains(operators,str[0])) { // first character is useful
            string temp = str.substr(1);
            return str[0] + operatorsFrom(temp);
        } else { // discard first character
            return operatorsFrom(str.substr(1));
        }
    }
    return str;
}

/*
 * This function takes a string of operators and checks
 * that they are matched. This is true if the string is
 * empty or if the string contains either "()", "[]", or
 * "{}" and the rest of the string is balanced after
 * removing those substrings.
 */
bool operatorsAreMatched(string ops) {
    if (ops == "") { // base case
        return true;
    }
    else { // recursive cases
        // return value of find() functions: index of the substr (ie index of "{}")
        size_t foundcur = ops.find("{}");
        size_t foundsq = ops.find("[]");
        size_t foundpar = ops.find("()");

        if (foundcur != string::npos) {
            ops.erase(foundcur,2);
            return operatorsAreMatched(ops);
        } else if (foundsq != string::npos) {
            ops.erase(foundsq,2);
            return operatorsAreMatched(ops);
        } else if (foundpar != string::npos) {
            ops.erase(foundpar,2);
            return operatorsAreMatched(ops);
        }
    }
    return false;
}


/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

// Student tests
STUDENT_TEST("operatorsFrom on an empty string") {
    EXPECT_EQUAL(operatorsFrom(""),"");
}

STUDENT_TEST("operatorsFrom long set with mixed symbols") {
    EXPECT_EQUAL(operatorsFrom("($%^)[&*;]{:=-+}"),"()[]{}");
}

STUDENT_TEST("operatorsFrom with letters") {
    EXPECT_EQUAL(operatorsFrom("abcdef"),"");
}

STUDENT_TEST("operatorsAreMatched on simple string") {
    EXPECT(operatorsAreMatched("[{()}]"));
}

STUDENT_TEST("operatorsAreMatched NOT matched") {
    EXPECT(!operatorsAreMatched("[{(})]"));
}

STUDENT_TEST("operatorsAreMatched on empty string") {
    EXPECT(operatorsAreMatched(""));
}

STUDENT_TEST("operatorsAreMatched NOT matched, case 2") {
    EXPECT(!operatorsAreMatched("[]]"));
}
