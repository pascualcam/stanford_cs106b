#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;

/*
 * The CONSTRUCTOR initializes all of the member variables needed for
 * an instance of the PQHeap class.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated];
    _numFilled = 0;
}

/* The DESTRUCTOR is responsible for cleaning up any resources
 * used by this instance of the PQHeap class.
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::enqueue(DataPoint elem) {
    if (isEmpty()) {
        _elements[0] = elem;
        _numFilled++;

    // Sort array
    } else {
        _elements[_numFilled] = elem;
        _numFilled++;
        bubbleUp();
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
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("Cannot peek empty pqheap");
    }
    return _elements[0];
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::dequeue() {
    if (isEmpty()) {
        error("Cannot dequeue empty pqheap");
    }

    DataPoint dequeued = _elements[0];
    bubbleDown();
    return dequeued;

}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::printDebugInfo() {
    /* TODO: Implement this function. */
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() {
    if (_numFilled > _numAllocated) error ("Too many elements in not enough space!");

    /* TODO: update this function! */
//    for (int i = 1; i < size(); i++) {
//        if (_elements[i].priority > _elements[i-1].priority) {
//            error("Array elements out of order at index " + integerToString(i));
//    }
}

/*
 * Calculates the index of the parent of the element with the
 * provided index.
 */
int PQHeap::getParentIndex(int curIndex) {
    int parentIndex = (curIndex - 1) / 2;
    return parentIndex;
}

/*
 * Calculates the index of the left child of the element with the
 * provided index.
 */
int PQHeap::getLeftChildIndex(int curIndex) {
    int leftChildIndex = 2*curIndex + 1;
    return leftChildIndex;
}

/*
 * Calculates the index of the right child of the element with the
 * provided index.
 */
int PQHeap::getRightChildIndex(int curIndex) {
    int rightChildIndex = 2*curIndex + 2;
    return rightChildIndex;
}

/* Helper function
 */
void PQHeap::bubbleUp() {

    // Get indices for enqueued and parent elements
    int indexEnqueued = _numFilled - 1;
    int indexParent = getParentIndex(_numFilled - 1);

    // Get enqueued and parent elements
    DataPoint enqElem = _elements[indexEnqueued];
    DataPoint parent = _elements[indexParent];


    // While enqueued element is smaller or equal than parent:
    while (enqElem.priority <= parent.priority) {

        // Swap elements
        DataPoint temp = parent;

        _elements[indexParent] = _elements[indexEnqueued];
        _elements[indexEnqueued] = temp;

        parent = enqElem;
        enqElem = temp;

        // Set updated indices
        indexEnqueued = indexParent;
        indexParent = getParentIndex(indexEnqueued);

        // If indexed value reaches top break loop
        if (indexEnqueued == 0) break;
    }
}

/* Helper function
 */
void PQHeap::bubbleDown() {

    // Put last element on first index and reduce _numFilled
    _elements[0] = _elements[_numFilled - 1];
    _numFilled--;

    // Special case only one element left
    if(_numFilled == 1) return;

    // Initialize curIndex
    int curIndex = 0;

    while (true) {
        // Update current
        DataPoint cur = _elements[curIndex];
        DataPoint leftChild;
        DataPoint rightChild;

        int leftChildIndex = getLeftChildIndex(curIndex);
        int rightChildIndex = getRightChildIndex(curIndex);


        // Check if left child is in bounds and return if not
        if (leftChildIndex < _numFilled) {
            leftChild = _elements[leftChildIndex];
        } else return;

        // Check if right child is in bounds and if so compare with left and swap if necessary
        if (rightChildIndex < _numFilled) {
            rightChild = _elements[rightChildIndex];

            // If right child is in bounds compare with left
            if (leftChild.priority < rightChild.priority) {

                // If left is smaller compare with current and swap if necessary
                if (cur.priority > leftChild.priority) {
                    DataPoint temp = cur;
                    _elements[curIndex] = _elements[leftChildIndex];
                    _elements[leftChildIndex] = temp;
                    curIndex = leftChildIndex;
                } else return;

            // If right is smaller compare with current and swap if necessary
            } else {
                if (cur.priority > rightChild.priority) {
                    DataPoint temp = cur;
                    _elements[curIndex] = _elements[rightChildIndex];
                    _elements[rightChildIndex] = temp;
                    curIndex = rightChildIndex;
                } else return;
            }

        // If right is out of bounds only compare with left child
        } else {
            if (cur.priority > leftChild.priority) {
                DataPoint temp = cur;
                _elements[curIndex] = _elements[leftChildIndex];
                _elements[leftChildIndex] = temp;
                curIndex = leftChildIndex;
            } else return;
        }
     }
}

/* Helper function
 * Check for edge cases... what if only one child?
 */
void PQHeap::swap() {

}

/* * * * * * Test Cases Below This Point * * * * * */

/* Custom tests here! */

STUDENT_TEST("PQHeap example from assignment validate enqueue") {
    PQHeap pq;

    pq.enqueue( { "Zoe", 5 } );
    pq.enqueue( { "Elmo", 8 } );
    pq.enqueue( { "Bert", 9 } );
    pq.enqueue( { "Gonzalo", 10} );
    pq.enqueue( { "Ed", 11} );
    pq.enqueue( { "Dani", 12} );
    pq.enqueue( { "Javier", 13} );
    pq.enqueue( { "Lea", 14} );
    pq.enqueue( { "Alex", 22} );
    pq.enqueue( { "Pascual", 43} );

    EXPECT_EQUAL(pq.size(), 10);
}

STUDENT_TEST("PQHeap example from assignment validate dequeue") {
    PQHeap pq;
    pq.enqueue( { "Zoe", 5 } );
    pq.enqueue( { "Elmo", 8 } );
    pq.enqueue( { "Bert", 9 } );
    pq.enqueue( { "Gonzalo", 10} );
    pq.enqueue( { "Ed", 11} );
    pq.enqueue( { "Dani", 12} );
    pq.enqueue( { "Javier", 13} );
    pq.enqueue( { "Lea", 14} );
    pq.enqueue( { "Alex", 22} );
    pq.enqueue( { "Pascual", 43} );
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    EXPECT_EQUAL(pq.size(), 0);
}
/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (auto dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

static void fillQueue(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", i });
    }
}

static void emptyQueue(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

PROVIDED_TEST("PQHeap timing test, fillQueue and emptyQueue") {
    PQHeap pq;

    TIME_OPERATION(40000, fillQueue(pq, 40000));
    TIME_OPERATION(40000, emptyQueue(pq, 40000));
}
