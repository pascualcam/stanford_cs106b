/* This file contains the Quick Sort algorithm (recursive)
 * with its corresponding helper functions (iterative).
 * A wide variety of tests are provided to ensure its functionality.
 *
 * Pair programming:
 *
 * Gonzalo Aguilar Sanchez - gonzaloa
 * Pascual Eduardo Camacho = ecam
*/

#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/* This function is passed three lists: less, equal and greater, and it
 * concatenates them by wiring the pointer from the first list (originally nullptr)
 * to the first value of the second list.
 * It returns result, which is less -> equal -> greater as a single list.
 */
ListNode* concatenate(ListNode*& less, ListNode*& equal, ListNode*& greater) {

    // If less is empty, concatenate equal + greater
    if (less == nullptr) {

        // Initialize result list
        ListNode* result = equal;

        // Transverse through *equal* list
        while (equal -> next != nullptr) {
            equal = equal -> next;
        }

        // Concatenate and return
        equal -> next = greater;
        return result;

    // if greater is empty, concatenate less + equal
    } else if(greater == nullptr) {
        // Initialize result list
        ListNode* result = less;

        // Traverse through *less* list
        while (less -> next != nullptr) {
            less = less -> next;
        }
        // Concatenate and return
        less -> next = equal;
        return result;

    // concatenate all
    } else {

        // Initialize result list
        ListNode* result = less;

        // Traverse through *less* list
        while (less -> next != nullptr) {
            less = less -> next;
        }
        // Concatenate
        less -> next = equal;

        // Traverse through *equal* list
        while (equal -> next != nullptr) {
            equal = equal -> next;
        }

        // Concatenate and return
        equal -> next = greater;
        return result;
    }
}

/* This function gets passed a list (front) with all the numbers to be sorted
 * and three pointers to create sublists (less, equal, greater). It works by
 * traversing through the front list and rewiring the pointers based on a pivot
 * value, which in this case is the first value of the front list.
 * The parameters are passed by reference, thus, a void function.
 */
void partition(ListNode* front, ListNode*& less, ListNode*& equal, ListNode*& greater) {

    // Get pivot value
    int pivotValue = front -> data;

    // Traverse through the list
    while (front != nullptr) {
        int curValue = front -> data;
        ListNode* temp = nullptr;

        if (curValue < pivotValue) {         // Less
            temp = less;
            less = front;
            front = front -> next;
            less -> next = temp;
        } else if (curValue == pivotValue) { // Equal
            temp = equal;
            equal = front;
            front = front -> next;
            equal -> next = temp;
        } else if (curValue > pivotValue) {  // Greater
            temp = greater;
            greater = front;
            front = front -> next;
            greater -> next = temp;
        }
    }
}

/* This recursive function takes a list of numbers and sorts them using
 * the quick sort algorithm. It utilizes two helper functions: partition
 * and concatenate. After partitioning the initial list, it makes two
 * recursive calls, one to the values less than the pivot and one to the
 * values greater than the pivot, finally, it concatenates the sublists.
 */
void quickSort(ListNode*& front) {

    // Base case: 0 or 1 element; thus, already sorted
    if (front == nullptr) {
        return;
    }

    // Initialize sublists
    ListNode* less = nullptr;
    ListNode* equal = nullptr;
    ListNode* greater = nullptr;

    // Partition into sublists
    partition(front, less, equal, greater);

    quickSort(less);
    quickSort(greater);

    // Concatenate sorted lists
    ListNode* concatenated = concatenate(less, equal, greater);
    front = concatenated;
}

/* * * * * * Test Code Below This Point * * * * * */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   while (front != nullptr) {
       cout << front->data;
       if (front->next != nullptr){
           cout << ", ";
       }
       front = front->next;
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {

    // Traverse through list
    while (front != nullptr) {

        // Create temporary pointer and set front to point to
        // next element in list. Deallocate memory afterwards.

        ListNode* temp = front;
        front = front -> next;
        delete temp;
    }
}

/*
 * This utility function is given a vector of values and
 * constructs an equivalent linked list containing the same values
 * in the same order. It can be used to construct a linked list to
 * use as an input to a function being tested.
 */
ListNode* createList(Vector<int> values){

    // Initialize linked list to nullpointer
    ListNode* linkedList = nullptr;

    // Loop through vector starting at the end and create linked list
    for (int i = values.size() - 1; i >= 0; i--) {
        linkedList = new ListNode(values[i], linkedList);
    }

    return linkedList;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){

    // Initialize index
    int idx = 0;
    int sizeList = 0;

    // Get size of linked list
    ListNode* temp = front;
    while (temp != nullptr) {
        temp = temp -> next;
        sizeList++;
    }

    // If vector is not the same size as linked list, return false
    if (sizeList != v.size()) return false;

    // Loop through vector elements and compare to *front* pointer value.
    // If one element from vector does not match pointer value, return false
    while (front != nullptr) {

        // Check if elements are equivalent
        if (v[idx] != front -> data) return false;
        idx++;
        front = front -> next;
    }
    return true;
}

/* * * * * * Student Tests * * * * * */

STUDENT_TEST("Test concatenate cases with empty sublist (less)"){

    ListNode* testList = createList({2, 3, 4, 5, 6, 7});

    ListNode* less = nullptr; // empty
    ListNode* equal = nullptr;
    ListNode* greater = nullptr;

    /* less = ()
     * equal = (1, 1)
     * greater = (2,3,4,5,6,7)
    */

    partition(testList, less, equal, greater);
    concatenate(less, equal, greater);

    deallocateList(testList);
}

STUDENT_TEST("Test quickSort on empty list"){

    /* Initialize vector to sequence of values */
    Vector<int> values = {};

    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);

}

STUDENT_TEST("Test quickSort on one-element list"){

    /* Initialize vector to sequence of values */
    Vector<int> values = {5};

    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);

}

STUDENT_TEST("Test quickSort on already sorted list"){

    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 3, 5};

    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Test quickSort on reverse-sorted list"){

    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 3, 5};

    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Test quickSort on repeated-elements list"){

    /* Initialize vector to sequence of values */
    Vector<int> values = {106, 106, 106, 106, 106};

    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);

}

STUDENT_TEST("Test quickSort on small list"){

    /* Initialize vector to sequence of values */
    Vector<int> values = {10, 3, 2, 5, 9, -2, 1};

    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);

}

STUDENT_TEST("Test quickSort on medium (random) list"){

    /* Initialize vector to sequence of values */
    // Note: retrieved form random.org
    Vector<int> values = {-37, -21, 52, -32, -67, 86, 19,
                          28, -54, 24, 16, 50, 17, 2, 69,
                          27, -83, 6, -23, -28, -25, -10,
                          -100, -24, 98, -66, -11, -42, 75,
                          -90, -16, 88, -44, -56, 49, 11, 25,
                          -65, 32, 67, 63, 72, -20, 91, -52,
                          94, 10, 87, -6, 60, -69, -73, -64,
                          44, -18, -88, 35, -86, 12, 0, -48,
                          -14, -53, -30, 97, -57, 39, 8, 77,
                          57, -95, -17, 14, 1, 92, -12, -92,
                          43, -50, -68, -78, -41, 4, 21, -3,
                          -79, -4, -71, -19, -7, -5, 95, -59,
                          38, -45, 65, -1, 93, -72, -60};

    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Test quickSort on large (random) list"){

    /* Initialize vector to sequence of values */
    // Note: retrieved form random.org
    Vector<int> values = {87, -46, 40, 99, 97, 38, -2, 8, -81, 39, -7, 28, 54, 68, -42, -57, 13, -79, 85, -39, 36, 91, -1, 43, 58, 41, -27, 98, -9, 64, -71, -13, -10, 72, 59, -83, 25, 24, -20, -98, 16, 15, -60, 42, 69, 10, -84, -96, -52, -40, -86, 66, 52, 6, 65, -32, 23, 76, -61, 92, 61, 56, -95, 86, -82, -56, -24, 57, 0, -59, -91, 35, -41, 2, 51, 14, 89, -8, 84, 27, 88, -25, 81, -92, -73, -3, 26, 46, 47, 96, -44, -72, 29, -4, 82, -31, 77, -68, 73, -93, 71, -77, -30, -74, 67, 75, 37, 1, -33, -18, -26, -97, -64, -11, -88, -99, -34, -12, -49, -48, 90, -51, -14, -29, 45, -50, 50, -53, -5, -17, 22, 18, -43, -70, -15, -47, 12, -6, 63, 33, 93, 49, 34, -78, 79, 95, 100, 55, -23, -22, -69, -87, -85, -67, 44, 53, -54, -16, -38, 62, 94, 74, -28, -21, -63, -100, 83, 80, 5, -80, 30, 9, 60, 17, 3, -35, -76, 48, 20, 21, 7, 31, 78, -62, -37, -94, 11, -55, 70, 19, -65, -58, -89, 32, -45, -19, -90, -66, -36, -75};

    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Test quickSort on very large (random) list"){

    /* Initialize vector to sequence of values */
    // Note: retrieved form random.org
    Vector<int> values = {158, 109, -484, 571, 406, 283, -345, -475, 506, 319, 465, -402, -241, 370, 596, -20, -520, 209, 382, 126, 390, 132, 462, 639, -19, 167, -518, -399, -376, -5, -302, -362, 261, 444, -502, 472, 346, -252, -219, 279, 20, -499, -723, 40, -416, -243, -91, -198, -454, -25, -511, -741, -329, -697, -125, 340, 413, -682, -469, 252, -307, 476, -678, -374, 673, -390, -296, -535, -33, 525, 21, 603, -183, 307, -440, 247, 432, -645, 420, -326, -680, 373, -16, -297, 28, -136, -394, 349, 93, -687, -734, -126, 310, -291, 726, 211, -330, -451, 357, 327, 348, -690, -62, -614, 530, 61, -277, 362, 695, -506, -547, -689, -1, 388, 203, 245, -189, -323, 231, -292, 263, -223, -71, -455, 143, 337, -67, -686, 711, 437, -676, 379, 105, 602, 463, -123, -117, 550, 629, 393, -630, -526, 426, -248, 380, -42, -31, 433, -426, -322, 555, -398, 129, 496, 594, 399, 540, -336, -729, -412, 87, 242, 578, 679, 542, 622, 464, 294, -482, 244, 172, 134, 216, -83, 705, -163, 97, 661, 49, 485, -351, -132, 121, 29, -78, 701, 423, -474, -699, -481, 355, -242, -400, -236, -531, 194, -523, -100, 270, -612, -272, 336, 607, -231, -352, -79, 120, -646, -644, -617, 123, -672, -93, -558, -39, -683, 480, -335, -55, 659, -212, 91, -280, 269, 215, 416, -477, 439, -592, 741, -140, -355, 312, 32, 171, 668, -59, -166, 170, -253, 549, -619, 185, -118, 1, -674, 42, 213, -121, 335, 68, 556, 258, 541, 274, -89, 517, 46, 207, 532, -684, 526, 389, -719, 150, -515, -367, -203, -383, 332, -288, 649, -357, 675, -2, -430, -179, 697, -92, -681, 352, 364, -737, -667, -538, -46, 43, 208, -23, -641, 273, 266, 26, -413, 356, 2, -691, -9, -628, -423, 430, -378, 457, -110, -610, -24, 376, 633, -570, -27, -636, 285, 229, 56, -497, -638, -221, -363, 627, -517, 259, 147, 521, -229, 415, -127, -718, 583, -701, 714, -227, -15, 660, -76, -533, -103, 453, 450, -341, 451, -325, 318, -728, 383, -652, -569, 468, -332, -194, 419, 624, -69, 5, 368, -403, 156, 581, 186, -498, 240, -34, -149, -72, 494, -339, 579, -539, -460, -373, -74, -306, 467, -131, -146, 528, 334, 681, -707, 493, -559, 302, 27, 280, 291, -240, -563, -51, 112, -543, 140, -191, 729, 656, -255, 333, -148, 45, -391, -115, -77, 410, 155, 381, 584, -66, 325, 700, -545, -522, -519, 315, -622, -726, 618, 191, -594, 151, 12, -188, -501, -192, 206, -597, 374, -84, 251, 53, -731, -660, 477, 503, 108, 0, 460, 11, -420, -201, 289, -408, -116, 471, -12, -706, -99, -739, 529, 667, -29, 237, -250, 214, -717, -442, 456, 96, -602, -235, -94, -382, -193, 669, 385, 658, 619, -63, 562, 570, -527, 544, 670, -470, 343, -575, -521, 17, -165, 664, 688, 51, -143, 184, 117, -480, 617, 534, -441, 635, 663, -282, 720, 35, -263, -170, -358, 375, 626, -304, -350, 735, 475, -10, -541, 317, 595, 702, -656, -434, -139, -385, -180, 107, -6, 731, -573, -491, 615, -406, 98, 287, -128, 59, -713, 654, -61, 686, 401, -600, 176, -73, 82, -106, 205, 256, 182, -41, -459, 470, -105, 693, -629, 736, -97, 497, 89, -650, 187, -175, 169, 69, 495, 429, -246, 377, -375, 728, 104, 727, 197, 620, -356, -668, 324, 715, -384, 671, -129, 228, 250, 412, -693, 653, 513, -590, -70, 516, 180, 149, 500, 447, -90, -410, -716, 347, 296, -141, 278, -679, 114, -596, 363, 344, 345, -425, -56, -190, -635, 392, -677, 118, 6, 111, -489, -715, -338, 567, -159, 249, 243, 353, 740, -613, 178, -411, 64, 262, 478, -174, -40, 79, -207, -206, -281, 511, 553, 365, 146, 535, -186, -11, -137, -258, -626, -419, -478, 225, -237, 589, 217, -634, 537, 651, 431, -87, 321, -28, 253, 438, 708, 682, -675, 316, 644, -295, -424, 743, 125, -703, 67, 676, -58, 160, 36, 360, -181, 642, -512, 292, 565, -493, -710, -8, 459, -366, 62, 135, 73, -688, -666, 306, -548, 24, 188, 7, -445, -313, -119, -324, 74, 168, -138, -308, 218, 264, -342, 88, 241, -214, -80, -369, 102, -153, -108, -224, -7, 361, -372, -113, 226, 14, 706, -500, -436, -26, -347, -435, -361, -446, -595, 683, -177, -554, 717, -577, 492, 301, 405, -432, 510, -305, -38, 164, -157, 398, 499, -273, 502, -607, -733, 265, 484, -631, 612, 314, -476, -473, 672, 284, -461, 434, -450, -536, -14, -293, 198, 236, 687, 19, -68, -431, -196, 320, 371, 31, -264, -722, 394, 13, 391, 110, -514, -609, 407, -318, 703, -283, -18, 90, -658, -36, 341, 533, 234, 400, -574, -749, -130, -17, 358, 71, -213, 636, -615, 409, -228, 698, 331, -102, -567, 99, -395, -265, 539, -444, 546, 339, -467, 238, -712, 623, 223, -582, 712, -601, 116, -695, -388, 599, -32, 57, -112, 446, -565, 536, -509, 441, 173, 268, 233, 204, -57, -487, 144, -724, -169, -43, 424, 449, -605, -164, -380, 479, -542, 41, -278, 716, -581, 422, -664, 66, 23, -448, -333, 558, 508, -284, 630, 127, -386, 749, -327, 585, 746, 175, -396, 222, 220, -583, -587, 277, -120, 628, -44, -218, 47, 267, 254, -463, 195, -433, -721, 159, 474, -4, 77, -556, -167, -568, 691, -720, -354, 350, 75, 115, 395, -176, 272, -551, 397, 454, 610, -379, 587, -639, -317, -109, 386, 30, 326, 734, -479, -239, 396, -30, 52, -48, -561, -294, -184, 722, 193, -312, 162, 16, -230, 72, -407, 699, -671, 452, 621, 411, 625, -649, 163, 154, 18, 248, -714, -346, -546, -496, 58, 255, 54, 387, -552, 707, -368, -625, 507, -344, 560, -692, 616, -537, 732, 509, -334, -742, -409, -437, 605, 588, -669, 342, 750, -220, 648, -465, -744, 76, -286, -492, 63, -155, -154, -404, 548, -187, 572, -315, -532, 545, 131, 680, -711, 448, -348, 4, -178, -562, 482, 684, -185, -623, -134, 359, -50, -182, -254, 37, 10, -534, 224, 632, 674, 39, -222, -162, 504, 677, -735, 694, 103, 522, 569, -503, -145, 638, 232, 179, -321, -654, -303, -490, 609, -370, 604, 101, -249, 613, 286, -429, 577, 308, 199, -405, 9, -747, 119, -290, 646, 481, -651, -47, -648, -279, 552, 130, -483, 300, 200, 704, 309, 747, -275, -260, -647, -585, 591, -657, 733, -392, -494, 322, -216, -462, -616, 520, -331, 709, 690, -202, -75, -555, -269, -727, 136, -745, -579, -208, 547, 83, -86, 257, -486, -328, 519, -633, 297, 443, 461, -750, 281, -571, -45, -603, -172, -725, 745, -387, -746, -88, 165, 685, -464, -52, 142, -152, -197, -267, 351, 44, 593, -300, 212, -21, -107, 531, 403, 86, -566, 299, 141, 354, 645, 421, 527, 128, 487, -160, 192, 275, -627, -104, 501, 662, 124, -591, -217, 505, -371, -632, -343, 153, 469, 710, 445, -349, 641, 161, 85, -549, 436, 597, -572, 666, 152, -309, -270, -251, -161, -505, 366, -173, 611, -655, 647, 70, 518, -274, 235, 304, -271, 80, -544, -507, 514, 557, 724, -694, 260, 580, -709, -472, -60, -397, -133, 210, 174, 402, -606, -98, -524, 606, -618, 157, -285, -289, 148, 742, -225, 92, -124, -530, 139, -598, 564, 568, -276, -168, -748, 738, 138, -215, 523, -418, -685, 538, 145, -310, -54, 239, 288, -624, 378, -377, -337, 657, -211, 100, -122, 442, -738, 303, 330, 543, 372, -82, -364, 665, -453, 489, 293, -428, 490, 614, 404, -488, -540, 271, 33, -210, 202, 425, -111, -238, -528, -663, -457, 435, -620, 313, -81, 650, 719, -320, 48, 196, 524, -53, -696, -550, 106, -504, -443, 678, -621, 737, -200, -456, -85, -233, -298, 221, 219, 554, 201, -513, 133, -608, 488, -171, -743, 575, 227, -439, -659, -422, 739, -580, -564, -259, -359, -700, -705, -22, 65, -643, -508, -316, -13, -393, -466, -427, -529, -311, -205, -415, 369, -245, -510, 598, 276, 181, -604, -257, -560, 282, 652, -578, -142, -96, -698, -736, 574, -414, 458, -114, 25, -199, -144, -586, -485, 491, -653, 50, -438, 637, 466, 367, -516, -389, -588, 166, 563, -553, -584, -740, -319, 305, 417, 748, -49, -147, -525, -35, -360, 486, 498, -471, 455, -673, -256, 561, 725, 640, -611, 631, 246, 418, -232, -599, 414, 483, -299, 428, -135, -732, 723, 8, 515, -287, 582, 586, 634, -468, -365, 177, 696, 78, -670, 551, 3, -314, -708, -452, 94, -234, -268, -209, 473, -340, 183, -95, -665, -204, -642, 55, 22, -261, -593, 15, -495, -101, -65, 573, -401, -247, 576, 38, -640, 122, -353, -158, -730, -661, -704, -447, 328, 643, 427, -637, 290, 608, -151, 512, -449, 713, 692, 329, -156, 440, -301, -662, -266, -576, 338, 95, 230, 34, -150, 311, -421, 408, 189, 295, 559, 113, 744, -37, 730, 137, -195, 190, 81, -262, 298, 655, -702, -381, 590, 566, -589, -3, -458, -417, 600, 689, 592, -226, 601, 721, 84, 323, 60, 384, -557, -244, 718};

    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

/* * * * * * Provided Tests * * * * * */

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 500000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */
        deallocateList(list);
    }
}
