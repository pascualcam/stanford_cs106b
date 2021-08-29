/* This file contains functions to conduct a binary merge operation
 * over two sorted sequences of numbers.
 *
 * Pair Progamming team:
 *
 * Gonzalo Aguilar - gonzaloa
 * Pascual Camacho - ecam
 *
 */

#include <iostream>    // for cout, endl
#include "error.h"      // for error function
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function takes a queue and checks if it
 * is ordered. If it is not ordered it raises an
 * error.
 */
void checkQueue(Queue<int> a) {

    while (!a.isEmpty()) {

        int first = a.dequeue();

        if (!a.isEmpty()) {
            int second = a.peek();
            if (first > second) error("Queue is not in order!");
        }
    }
}



/*
 * This function takes two ordered queues and returns an
 * ordered merged queue using an iterative process
 */
Queue<int> merge(Queue<int> a, Queue<int> b) {
    // Check if given queues are ordered
    checkQueue(a);
    checkQueue(b);

    // Initialize result
    Queue<int> result;

    // Loop through queues a and b until one of them is empty
    while (!a.isEmpty() && !b.isEmpty()){
        int numberA = a.peek();
        int numberB = b.peek();
        if (numberA < numberB) {
            result.enqueue(a.dequeue());
        } else {
            result.enqueue(b.dequeue());
        }
    }

    // Once one queue is empty enqueue remaing values until both queues are empty
    while (!a.isEmpty() || !b.isEmpty()){
        if (!a.isEmpty()){
            result.enqueue(a.dequeue());
        } else {
            result.enqueue(b.dequeue());
        }
    }

    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined merge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> multiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = merge(q, result);
    }
    return result;
}

/*
 * This function receives as input a vector of queues.
 * The vector is divided into k sequences of queues into
 * halves, then it recursively calls the same function on
 * each half and calls the merge function, providing as
 * input each half.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    // Initialize result
    Queue<int> result;

    // Base case
    if (all.size() == 0) {
        return result;
    }

    // Recursive case
    if (all.size() == 1) {
        return all[0];
    }

    Vector<Queue<int>> firstHalf = all.subList(0, all.size()/2);
    Vector<Queue<int>> secondHalf = all.subList(all.size()/2);
    result = merge(recMultiMerge(firstHalf), recMultiMerge(secondHalf));

    return result;
}

/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("Test binary merge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

PROVIDED_TEST("Test multiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(multiMerge(all), expected);
}

PROVIDED_TEST("Test recMultiMerge by compare to multiMerge") {
    int n = 1000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(multiMerge(all), recMultiMerge(all));
}

PROVIDED_TEST("Time binary merge operation") {
    int n = 1500000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
}

PROVIDED_TEST("Time multiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

/* * * * * * Student Test Case * * * * * */

STUDENT_TEST("Test recMultiMerge base case") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> c = {7, 8, 9};
    Queue<int> expected = {1, 2, 3, 3, 4, 5, 7, 8, 9};
    Vector<Queue<int>> test;
    test.add(a);
    test.add(b);
    test.add(c);
    EXPECT_EQUAL(recMultiMerge(test), expected);
}

STUDENT_TEST("Test multiMerge with empty vector") {
    Vector<Queue<int>> all = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(multiMerge(all), expected);
}

STUDENT_TEST("Test multiMerge with multiple empty vector") {
    Vector<Queue<int>> all = {{},
                              {},
                              {},
                              {}
                             };
    Queue<int> expected = {};
    EXPECT_EQUAL(multiMerge(all), expected);
}

STUDENT_TEST("Test binary merge, one empty queue empty") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {};
    Queue<int> expected = {2, 4, 5};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}
STUDENT_TEST("Test binary merge, with identical queues") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {2, 4, 5};
    Queue<int> expected = {2, 2, 4, 4, 5, 5};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

STUDENT_TEST("Time binary merge operation") {
    int n = 1500000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
}

STUDENT_TEST("Time binary merge operation") {
    int n = 2*1500000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
}

STUDENT_TEST("Time binary merge operation") {
    int n = 3*1500000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
}

STUDENT_TEST("Time multiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}

STUDENT_TEST("Time multiMerge operation") {
    int n = 2*11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}

STUDENT_TEST("Time multiMerge operation") {
    int n = 3*11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}
STUDENT_TEST("Time recMultiMerge operation") {
    int n = 2*90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}
STUDENT_TEST("Time recMultiMerge operation") {
    int n = 3*90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time multiMerge vs recMultiMerge operations with n = 1000") {
    int n = 4000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time multiMerge vs recMultiMerge operations with n = 2000") {
    int n = 4000;
    int k = n/8;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time multiMerge vs recMultiMerge operations with n = 4000") {
    int n = 4000;
    int k = n/6;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time multiMerge vs recMultiMerge operations with n = 8000") {
    int n = 4000;
    int k = n/4;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time multiMerge vs recMultiMerge operations with n = 16000") {
    int n = 4000;
    int k = n/2;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
