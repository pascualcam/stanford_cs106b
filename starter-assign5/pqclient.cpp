/*
 * This file contains multiple functions
 * that together allow us to solve the topK
 * problem using abstract data type. It
 * allow us to use sorted arrays as well
 * as heaps and let us compare both ADT's.
 *
 * Pair Programming:
 *
 * Gonzalo Aguilar - gonzaloa
 * Pascual Camacho - ecam
 *
 */

#include "pqclient.h"
#include "pqsortedarray.h"
#include "pqheap.h"
#include "vector.h"
#include "strlib.h"
#include <sstream>
#include "testing/SimpleTest.h"
using namespace std;

/* Using the Priority Queue data structure as a tool to sort, neat! */
void pqSort(Vector<DataPoint>& v) {
    PQSortedArray pq;

    /* Add all the elements to the priority queue. */
    for (int i = 0; i < v.size(); i++) {
        pq.enqueue(v[i]);
    }

    /* Extract all the elements from the priority queue. Due
     * to the priority queue property, we know that we will get
     * these elements in sorted order, in order of increasing priority
     * value. Store elements back into vector, now in sorted order.
     */
    for (int i = 0; i < v.size(); i++) {
        v[i] = pq.dequeue();
    }
}

/*
 * This function takes an istream of DataPoints.
 * Using sorted ADT's it finds the k biggest values
 * in the istream.
 */
Vector<DataPoint> topK(istream& stream, int k) {

    // Initialize result vector, current data point and pq sorted array
    Vector<DataPoint> result;
    DataPoint cur;
    PQSortedArray pq;

    // Loop over each DataPoint in istream
    while (stream >> cur) {

        // Enqueue elements until sorted array reaches size = k
        if (pq.size() < k) {
            pq.enqueue(cur);
        }

        // Once array reaches size = k only enqueue values with
        // priority larger than the smallest priority of sorted array
        else {
            DataPoint smallest = pq.peek();
            if (cur.priority > smallest.priority){
                pq.enqueue(cur);
                pq.dequeue();
            }
        }
    }

    // Fill result vector with top k values and then revese result
    while (!pq.isEmpty()) {
        result.add(pq.dequeue());
    }

    result.reverse();
    return result;
}



/* * * * * * Test Cases Below This Point * * * * * */

/* Helper function that, given a list of data points, produces a stream from them. */
stringstream asStream(const Vector<DataPoint>& dataPoints) {
    stringstream result;
    for (const DataPoint& pt: dataPoints) {
        result << pt;
    }
    return result;
}

/* Helper function that, given range start and stop, produces a stream of values. */
stringstream asStream(int start, int stop) {
    stringstream result;
    for (int i = start; i <= stop; i++) {
        DataPoint pt = { "", i };
        result << " " << pt << " ";
    }
    return result;
}

/* TODO: Add your own custom tests here! */












/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("pqSort 100 random elements") {
    Vector<DataPoint> input;
    Vector<int> expectedOutput;
    for (int i = 0; i < 100; i++) {
        int randomValue = randomInteger(1, 10);
        input.add({ "", randomValue});
        expectedOutput.add(randomValue);
    }
    expectedOutput.sort();
    pqSort(input);
    for (int i = 0; i < 100; i++) {
        EXPECT_EQUAL(input[i].priority, expectedOutput[i]);
    }
}

PROVIDED_TEST("pqSort time trial") {
    int startSize = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        for (int i = 0; i < n; i++) {
            v.add({ "", randomInteger(1, n) });
        }
        TIME_OPERATION(n, pqSort(v));
    }
}


/* Constant used for sizing the tests below this point. */
const int kMany = 100000;

PROVIDED_TEST("topK: stream 0 elements, ask for top 1") {
    stringstream stream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: stream 1 element, ask for top 1") {
    stringstream stream = asStream({ { "" , 1 } });
    Vector<DataPoint> expected = { { "" , 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: small hand-constructed input") {
    Vector<DataPoint> input = { { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 } };
    stringstream stream = asStream(input);
    Vector<DataPoint> expected = { { "D", 4 }, { "C", 3 } };
    EXPECT(topK(stream, 2) == expected);

    stream = asStream(input);
    expected = { { "D", 4 }, { "C", 3 }, { "B", 2 } };
    EXPECT(topK(stream, 3) == expected);

    stream = asStream(input);
    expected = { { "D", 4 }, { "C", 3 }, { "B", 2 }, { "A", 1 } };
    EXPECT(topK(stream, 4) == expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 1") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = { { "" , kMany - 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 5") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = {
        { "" , kMany - 1 },
        { "" , kMany - 2 },
        { "" , kMany - 3 },
        { "" , kMany - 4 },
        { "" , kMany - 5 }
    };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stress test, many elements, random values") {
    Vector<int> sorted;
    Vector<DataPoint> points;
    for (int i = 0; i < 10000; i++) {
        int weight = randomInteger(0, 100000);
        sorted.add(weight);
        points.add({ "" , weight});
    }

    stringstream stream = asStream(points);
    sort(sorted.begin(), sorted.end(), greater<int>());
    Vector<DataPoint> result = topK(stream, 10);

    EXPECT_EQUAL(result.size(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQUAL(result[i].priority, sorted[i]);
    }
}

PROVIDED_TEST("topK: stress test - many elements, ask for top half") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> result = topK(stream, kMany/2);
    EXPECT_EQUAL(result.size(), kMany/2);
    EXPECT_EQUAL(result[0].priority, kMany);
    EXPECT_EQUAL(result[result.size()-1].priority, kMany - result.size() + 1);
}

PROVIDED_TEST("topK: time trial") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}
