/*
 * This file contains multiple functions
 * that together allow us to implement
 * a sorted array abstract data type.
 *
 * Pair Programming:
 *
 * Gonzalo Aguilar - gonzaloa
 * Pascual Camacho - ecam
 *
 */

#include "pqsortedarray.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

// Program Constant
static const int INITIAL_CAPACITY = 10;

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the PQSortedArray class. The allocated capacity
 * is initialized to a starting constant and a dynamic array of that
 * size is allocated. The number of filled slots is initially zero.
 */
PQSortedArray::PQSortedArray() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated];
    _numFilled = 0;
}

/* The destructor is responsible for cleaning up any resources
 * used by this instance of the PQSortedArray class. The array
 * memory used for elements is deallocated here.
 */
PQSortedArray::~PQSortedArray() {
    delete[] _elements;
}

/*
 * This function takes a DataPoint and enqueues it
 * in a sorted array ADT. After enqueueing the DataPoint
 * it sorts the array using insertion.
 */
void PQSortedArray::enqueue(DataPoint elem) {

    // First term is always enqueued in the first index
    if (isEmpty()) {
        _elements[0] = elem;
        _numFilled++;

    // Sort array using insertion
    } else {
        for (int i = 0; i < _numFilled; i++) {

            // Identify correct index and insert accordingly
            DataPoint temp = _elements[i];
            if (elem.priority > temp.priority) {

                // Shift all elements after insertion to the right
                int j = _numFilled - 1;
                while (j >= i) {
                    _elements[j + 1] = _elements[j];
                    j--;
                }

                // Insert elem and break from loop
                _elements[i] = elem;
                break;
            }

            // Insert at the end if it has higher priority (lower priority number)
            if (i == _numFilled - 1) {
                _elements[i + 1] = elem;
            }
        }
        _numFilled++;
    }

    // Allocate more dynamic memory if the Array is filled
    if (_numFilled == _numAllocated) {

        // Duplicate allocated memory spaces and create a new DataPoint pointer array
        _numAllocated = _numAllocated * 2;
        DataPoint* _NewElements = new DataPoint[_numAllocated];

        // Copy elements from old pointer array to new pointer array
        for (int i = 0; i < _numFilled; i++){
            _NewElements[i] = _elements[i];
        }

        // Give back old _elements memory and reassign to _NewElements
        delete[] _elements;
        _elements = _NewElements;
    }

}

/*
 * The count of enqueued elements is tracked in the
 * member variable _numFilled.
 */
int PQSortedArray::size() const {
    return _numFilled;
}

/*
 * Since the array elements are stored in decreasing sorted order
 * by priority, the frontmost is located in the last filled
 * slot of the array. This function returns the element at that index.
 */
DataPoint PQSortedArray::peek() const {
    if (isEmpty()) {
        error("Cannot peek empty pqueue");
    }
    return _elements[_numFilled - 1];
}

/*
 * Since the array elements are stored in decreasing sorted order
 * by priority, the frontmost element is located in the last filled
 * slot of the array. This function returns the element at that index
 * while decrementing the count of filled slots to record that an
 * element has been removed.
 */
DataPoint PQSortedArray::dequeue() {
    if (isEmpty()) {
        error("Cannot dequeue empty pqueue");
    }
    return _elements[--_numFilled];
}

/*
 * Returns true if there are currently 0 elements in the queue, and
 * false otherwise.
 */
bool PQSortedArray::isEmpty() const {
    return size() == 0;
}

/*
 * Updates internal state to reflect that the queue is empty, e.g. count
 * of filled slots is reset to zero. The array memory stays allocated
 * at current capacity. The previously stored elements do not need to
 * be cleared; those slots will be overwritten when additional elements
 * are enqueued.
 */
void PQSortedArray::clear() {
    _numFilled = 0;
}

/*
 * Prints the contents of internal array.
 */
void PQSortedArray::printDebugInfo(string label) {
    cout << label << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * Confirm the internal state of member variables appears valid
 * Report an error if problems found.
 */
void PQSortedArray::validateInternalState() {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /* Loop over the elements in the array and compare priority of pair of
     * neighboring elements. If current element has larger priority
     * than the previous this indicates array elements are not in
     * in expected decreasing sorted order. Use error to report this problem.
     */
    for (int i = 1; i < size(); i++) {
        if (_elements[i].priority > _elements[i-1].priority) {
            error("Array elements out of order at index " + integerToString(i));
        }
    }
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQSortedArray example from writeup") {
    PQSortedArray pq;
    pq.enqueue( { "Zoe", -3 } );
    pq.enqueue( { "Elmo", 10 } );
    pq.enqueue( { "Bert", 6 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.printDebugInfo("After enqueue 3 elements");
    pq.enqueue( { "Kermit", 5 } );
    EXPECT_EQUAL(pq.size(), 4);
    EXPECT_EQUAL(pq.size(), 4);

    pq.printDebugInfo("After enqueue one more");

    DataPoint removed = pq.dequeue();
    DataPoint expected = { "Zoe", -3 };
    EXPECT_EQUAL(removed, expected);
    pq.printDebugInfo("After dequeue one");
}

PROVIDED_TEST("PQSortedArray, only enqueue, validate at every step") {
    PQSortedArray pq;
    int count = 50;  // TODO: change to larger count when ready to test expand

    pq.validateInternalState();
    for (int i = 0; i < count; i++) {
        pq.enqueue({ "", i*10 });
        pq.validateInternalState();
    }
    pq.validateInternalState();
}

PROVIDED_TEST("PQSortedArray: test size/isEmpty/clear") {
    PQSortedArray pq;
    int count = 8;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    for (int i = 0; i < count; i++) {
        EXPECT_EQUAL(pq.size(), i);
        pq.enqueue({ "", i*10 });
        EXPECT_EQUAL(pq.size(), i + 1);
    }
    pq.clear();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("PQSortedArray: dequeue or peek on empty/cleared queue throws error") {
    PQSortedArray pq;
    DataPoint point = { "Programming Abstractions", 106 };

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.dequeue();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.clear();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("PQSortedArray, dequeue, validate at every step") {
    PQSortedArray pq;
    int count = 7;

    for (int i = 0; i < count; i++) {
        pq.enqueue({ "", i*100 });
    }
    pq.validateInternalState();
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
    pq.clear();
    pq.validateInternalState();
}

PROVIDED_TEST("PQSortedArray, ascending and descending sequences") {
    PQSortedArray pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), 2 * i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), 2 * i + 1 });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 40; i++) {
        DataPoint removed = pq.dequeue();
        EXPECT_EQUAL(removed.priority, i);
    }
}

PROVIDED_TEST("PQSortedArray, duplicate elements") {
    PQSortedArray pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 20; i++) {
        DataPoint one = pq.dequeue();
        DataPoint two = pq.dequeue();

        EXPECT_EQUAL(one.priority, i);
        EXPECT_EQUAL(two.priority, i);
    }
}

PROVIDED_TEST("PQSortedArray, interleave enqueue/dequeue") {
    PQSortedArray pq;
    int n = 100;
    for (int i = n / 2; i < n; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = n / 2; i < n; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);

    pq.validateInternalState();

    for (int i = 0; i < n / 2; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = 0; i < n / 2; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("PQSortedArray stress test, cycle 10000 random elements in and out") {
    PQSortedArray pq;
    int n = 10000;
    DataPoint max = {"max", 106106106};
    DataPoint min = {"min", -106106106};

    pq.enqueue(min);
    pq.enqueue(max);
    for (int i = 0; i < n; i++) {
        int randomPriority = randomInteger(-10000, 10000);
        pq.enqueue({ "", randomPriority });
    }
    EXPECT_EQUAL(pq.size(), n + 2);
    pq.validateInternalState();

    EXPECT_EQUAL(pq.dequeue(), min);
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
    EXPECT_EQUAL(pq.dequeue(), max);
}

static void fillQueue(PQSortedArray& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", i });
    }
}

static void emptyQueue(PQSortedArray& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

PROVIDED_TEST("PQSortedArray timing test, fillQueue and emptyQueue") {
    PQSortedArray pq;

    int n = 100;
    TIME_OPERATION(n, fillQueue(pq, n));
    TIME_OPERATION(n, emptyQueue(pq, n));

    n *= 10;
    TIME_OPERATION(n, fillQueue(pq, n));
    TIME_OPERATION(n, emptyQueue(pq, n));

    n *= 10;
    TIME_OPERATION(n, fillQueue(pq, n));
    TIME_OPERATION(n, emptyQueue(pq, n));

    n *= 10;
    TIME_OPERATION(n, fillQueue(pq, n));
    TIME_OPERATION(n, emptyQueue(pq, n));
}

//PROVIDED_TEST("PQSortedArray timing test, fillQueue and emptyQueue") {
//    PQSortedArray pq;

//    TIME_OPERATION(10000, fillQueue(pq, 10000));
//    TIME_OPERATION(10000, emptyQueue(pq, 10000));
//}
