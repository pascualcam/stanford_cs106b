#include "datapoint.h"
#include "testing/SimpleTest.h"
#include <string>
using namespace std;

/* The theme of this exercise will be building a grocery list of DataPoints,
 * where each point is an item that you want to buy at the store, with its
 * associated priority of how important it is to get.
 *
 * The DataPoint struct is defined in datapoint.h if you want to inspect the
 * declaration of the struct.
 */

PROVIDED_TEST("Test valid use of array, allocate/access/deallocate") {
    DataPoint* shoppingList = new DataPoint[6]; // allocate

    for (int i = 0; i < 3; i++) {
        shoppingList[i].name = "eggs"; // set struct field by field
        shoppingList[i].priority = 10 + i;
    }
    shoppingList[0].priority += 5;
    delete[] shoppingList; // deallocate
}

//PROVIDED_TEST("Test case that dereferences null pointer") {
//    /* Often we initialize pointers to a special value called nullptr to
//     * indicate a pointer that is not yet pointing to valid memory. The
//     * nlllptr is intended to be used as a sentinel, attempting to
//     * dereference and access the zero location is an illegal operation.
//     */
//    DataPoint* shoppingList = nullptr; // no memory allocated
//    shoppingList[0].name = "yogurt";   // executing this line dereferences nullptr
//}

//PROVIDED_TEST("Test case that access indexes beyond array bounds") {
//    DataPoint* shoppingList = new DataPoint[5];
//    shoppingList[0].name += "pickles";    // index is valid
//    shoppingList[5].name += "peppers";    // index one beyond bound
//    shoppingList[999].name += "potatoes"; // index far out of bounds
//}

//PROVIDED_TEST("Test case that accesses memory after it was deallocated") {
//    DataPoint* shoppingList = new DataPoint[4]; // allocate
//    shoppingList[0].name = "bread";
//    shoppingList[0].priority = 1000;
//    delete[] shoppingList; // shoppingList holds address of memory that is now deallocated
//    // executing the line below will access deallocated memory
//    if (shoppingList[0].name == "bread")
//         cout << "deallocated memory contents have not changed" << endl;
//}

//PROVIDED_TEST("Test case that deallocates same memory address twice") {
//    DataPoint* shoppingList = new DataPoint[3]; // allocate
//    DataPoint* ptr = shoppingList; // ptr holds same memory address as shoppingList
//    delete[] shoppingList;  // deallocate memory once
//    delete[] ptr;           // executing this line deallocates same memory again
//}


