/*
 * This file contains two functions. The main function is called
 * to draw a Sierpinski triangle, while the other is a helper function
 * utilized to fill a single triangle with the color black.
 * The drawSierpinskiTriangle function utilizes recursion. A more detailed
 * description of each function is provided before each.
 *
 * Pair Programming:
 *
 * Gonzalo Aguilar - gonzaloa
 * Pascual Camacho - ecam
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("green");
    window.fillPolygon( {one, two, three} );
}

/*
 * This function uses a recursive approach to draw fractals based on Sierpinski's
 * triangle. As input, it receives a GUI window, thre points to indicate the triangle's
 * corners, and an integer that indicates the order of the triangle. If the order is
 * less than zero, the function returns a problem, otherwise, the function has a base case
 * when the order is equal to zero and it draws a triangle and fills it with color black.
 * In the recursive case the function finds the midpoints of the input triangle coordinates,
 * which are passed recursively into the same function, and the order is reduced by one.
 * Three steps are necessary in the recursive case: one for each side of a triangle.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    int countTriangles;
    // Flag error if order is negative
    if (order < 0) {
        error("Order must be greater than or equal to zero.");
    }
    // Base case
    if (order == 0) {
        window.drawLine(one,two);
        window.drawLine(one, three);
        window.drawLine(two, three);
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    // First compute the triangle's middle points of current iteration
    GPoint midpointLeft((one.x + two.x)/2, (one.y + two.y)/2);
    GPoint midpointRight((two.x + three.x)/2, (two.y + three.y)/2);
    GPoint midpointBottom((one.x + three.x)/2, (one.y + three.y)/2);

    // Recursive cases
    countTriangles = drawSierpinskiTriangle(window, one, midpointLeft, midpointBottom, order-1);
    countTriangles += drawSierpinskiTriangle(window, midpointLeft, two, midpointRight, order-1);
    countTriangles += drawSierpinskiTriangle(window, midpointBottom, midpointRight, three, order-1);

    // Return count of triangles drawn
    return countTriangles;
}

/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

