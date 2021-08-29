/* The primary goal of this file is to count the number of potential paths
 * that can take Karel from a given position to the origin, which is at the
 * intersection of Street 1 and Avenue 1.
 *
 * Pair Progamming team:
 *
 * Gonzalo Aguilar - gonzaloa
 * Pascual Camacho - ecam
 *
 */

#include <iostream>     // for cout, endl
#include "recursion.h"
#include "error.h"      // for error function
#include "set.h"        // for sets
#include "vector.h"     // for vectors
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function return the number of routes in between Karel and the origin
 * using a recursive method.
 */

int countRoutes(int street, int avenue, Vector<Vector<char>>& routes, Vector<char>& route) {

    // Determine if initial position is valid
    if (street <= 0 || avenue <= 0) error("Invalid initial position. Initial position must be a positive coordinate");

    // Define origin coordinate
    int originStreet = 1;
    int originAvenue = 1;

    // Determine if route is alredy at the origin
    if (street == originStreet && avenue == originAvenue){
        routes.add(route);
        route.clear();

    // Recursive case exploring different routes
    } else {
        if (street > 1) {
            route.add('S');
            countRoutes(street - 1, avenue, routes, route);
        }
        if (avenue > 1) {
            route.add('W');
            countRoutes(street, avenue - 1, routes, route);
        }
    }
    return routes.size();
}

/*
 * This function takes two integers with the coordinates
 * of the original position of Karel and returns the number
 * of possible routes in between Karel's position and the
 * origin using a helper function with the same name but
 * additional variables.
 */
int countRoutes(int street, int avenue) {

    Vector<Vector<char>> routes;
    Vector<char> route;

    return countRoutes(street, avenue, routes, route);

}



/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test countRoutes") {
   EXPECT_EQUAL(countRoutes(3, 2), 3);
}

/* * * * * * Student Test Cases * * * * * */

STUDENT_TEST("Test countRoutes with negative values") {
   EXPECT_ERROR(countRoutes(-2, 3));
}

STUDENT_TEST("Test countRoutes") {
   EXPECT_EQUAL(countRoutes(1, 100), 1);
}

STUDENT_TEST("Test countRoutes") {
   EXPECT_EQUAL(countRoutes(100, 1), 1);
}

STUDENT_TEST("Test countRoutes") {
   EXPECT_EQUAL(countRoutes(3, 3), 6);
}

STUDENT_TEST("Test countRoutes") {
   EXPECT_EQUAL(countRoutes(4, 2), 4);
}
