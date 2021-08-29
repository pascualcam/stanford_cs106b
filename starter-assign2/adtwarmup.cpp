#include "testing/SimpleTest.h"
#include "map.h"
#include "queue.h"
#include "stack.h"
using namespace std;

/* This function correctly reverses the elements of a queue.
 */
void reverse(Queue<int>& q) {
    Stack<int> s;
    int val;

    while (!q.isEmpty()) {
        val = q.dequeue();
        s.push(val);
    }
    while (!s.isEmpty()) {
        val = s.pop();
        q.enqueue(val);
    }
}

/* This function is intended to modify a queue of characters to duplicate
 * any negative numbers.
 *
 * WARNING: the given code is buggy. See exercise writeup for more
 * information on how to test and diagnose.
 */
void duplicateNegatives(Queue<int>& q) {
    // Get size of queue before FOR loop iterations.
    int x = q.size();
    for (int i = 0; i < x; i++) {
        int val = q.dequeue();
        q.enqueue(val);
        if (val < 0) {
            q.enqueue(val);   // double up on negative numbers
        }
    }
}

// This function is intended to remove key/value pairs from
// map where key == value
// WARNING: the given code is buggy. See exercise writeup for more
// information on how to test and diagnose.
void removeMatchPairs(Map<string, string>& map) {
    for (string key: map) {
        if (map[key] == key) {
            map.remove(key);
        }
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("reverse queue") {
    Queue<int> q = {1, 2, 3, 4, 5};
    Queue<int> expected = {5, 4, 3, 2, 1};

    reverse(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has no negatives") {
    Queue<int> q = {2, 10};
    Queue<int> expected = q;

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has single negative") {
    Queue<int> q = {-6, 7};
    Queue<int> expected = {-6, -6, 7};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has some negatives") {
    Queue<int> q = {-3, 4, -5, 10};
    Queue<int> expected = {-3, -3, 4, -5, -5, 10};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("removeMatchPair, no change") {
    Map<string, string> m = {{"Thomas", "Tom"}, {"Margaret", "Meg"}};
    Map<string, string> expected = m;

    removeMatchPairs(m);
    EXPECT_EQUAL(m, expected);
}

PROVIDED_TEST("removeMatchPair, remove one") {
    Map<string, string> m = {{"Thomas", "Tom"}, {"Jan", "Jan"}, {"Margaret", "Meg"}};
    Map<string, string> expected = {{"Thomas", "Tom"},  {"Margaret", "Meg"}};

    removeMatchPairs(m);
    EXPECT_EQUAL(m, expected);
}

// Student Tests
STUDENT_TEST("duplicateNegatives, input is all negatives") {
    Queue<int> q = {-1,-2,-3,-4};
    Queue<int> expected = {-1,-1,-2,-2,-3,-3,-4,-4};

    duplicateNegatives(q);
    EXPECT_EQUAL(q,expected);
}
