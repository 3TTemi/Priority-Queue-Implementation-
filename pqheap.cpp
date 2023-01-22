#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; 

/*
This constructor initiliazes the member variables for the PQHeap class.
Allocated Capacity is set to a constant of 10.
The array of elements is creates with variable DataPoint.
Current size/numFilled it set to 0.
 */
PQHeap::PQHeap() {
    numAllocated = INITIAL_CAPACITY;
    elements = new DataPoint[numAllocated]();
    numFilled = 0;

}

/*
This destructor cleans up used resources by an instant of PQHeap.
Memory is dellocated by being deleted.
 */
PQHeap::~PQHeap() {
    delete[] elements;

}

/*
Enqueue function take in parameter elemetns and puts in the priority queue.
First checks if the array is full in which it calls to enlarge the array
Adds the elements to the end of the array, and propely increments numFilled by 1
Calls bubble function to readjust for Heap Validity
 */
void PQHeap::enqueue(DataPoint elem) {

    if(size() == numAllocated)
    {
        enlargeHeap();
    }

    elements[size()] = elem;
    numFilled++;
    bubble();
}

/*
 Helper function to swap to Datapoints given their indexes
 Uses a temporary Datapoint vairbale to not lose indexes while they are bing swapped
 */
void PQHeap::swap(int index1, int index2)
{
    DataPoint temp = elements[index1];
    elements[index1] = elements[index2];
    elements[index2] = temp;
}

/*
 Function readjusts Heap priority after element is enqueueed to the back
 Element that needs to be checked is inialized to last in the array(Bubble element)
 Iterates through size/2 since only needs to check one parent on each level
 If the bubble Element is not equal to Parent, then the parent Index is Calcualted
 Checks priority of parent to bubble elements priority, and swaps if parents priiority is bigger
 */
void PQHeap::bubble()
{

    int bubbleElement = size() - 1;

    for (int i = 0; i < (size() / 2); i++)
    {
        if (bubbleElement != 0)
        {
            int bubbleParent = getParentIndex(bubbleElement);
            if(elements[bubbleParent].priority > elements[bubbleElement].priority)
            {
                swap(bubbleElement, bubbleParent);
                bubbleElement = bubbleParent;
            }
        }
    }

}

/*
 Function readjusts Heap Priority for element places on the root
 Iterates through size/2 since on Binary Heap we are only checking each level
 Calculates Left and Right Children from Bubble elemnt
 Checks to make sure both the right and left child are not invalid/ not in array
Checks if eithe the left or right child priority is less to prepare for swap
Then checks with if statemtns which of the two chilsd has a smaller priorities and swaps both the Datpaoint and Index
Continues with iteration until At leaf
 */
void PQHeap::reverseBubble()
{
    int bubbleElement = 0;

    for (int i = 0; i < (size() / 2); i++)
    {
        int leftIndex = getLeftChildIndex(bubbleElement);
        int rightIndex = getRightChildIndex(bubbleElement);

        if(validateIndex(leftIndex) == NONE && validateIndex(rightIndex) == NONE)
        {
            break;
        }

        if(elements[leftIndex].priority < elements[bubbleElement].priority || elements[rightIndex].priority < elements[bubbleElement].priority)
        {
            if(elements[leftIndex].priority <= elements[rightIndex].priority && validateIndex(leftIndex) != NONE)
            {
                swap(bubbleElement, leftIndex);
                bubbleElement = leftIndex;
            }
            else if (elements[rightIndex].priority <= elements[leftIndex].priority && validateIndex(rightIndex) != NONE)
            {
                swap(bubbleElement, rightIndex);
                bubbleElement = rightIndex;
            }
        }

    }

}



/*
    Doubles the size of the array for enlargment when full
    Creates new array of size teice the current arrays numbeer Allocation
    Copies elements from old array to new array using for loop of curernt arays size
    Deltes currents array and replaces with new Array pointer
    Updates private vlaue num Allocaiton to be double what it was before
 */
void PQHeap::enlargeHeap()
{
    DataPoint* newArray = new DataPoint[numAllocated * 2];

    for(int i = 0; i < size(); i++)
    {
        newArray[i] = elements[i];
    }

    delete[] elements;

    elements = newArray;

    numAllocated *= 2;

}

/*
Peek  checks and returns the element with highest priority or in the root (Does not remove just check)
Chekcs to make sure array is not empty, if it is it returns an error
Otherwise: Returns the element in the arryas first position
 */
DataPoint PQHeap::peek() const {
    if(isEmpty())
    {
        error("Priority Queue is Empty");
    }
    return elements[0];
}

/*
Deuque removes and reutnrs the elemnt with the highest priority
Checks ot make sure array is not empty, if it is it reutnr an error
Makes temp value to retrieve highest priority element to later be returned
Replaces the Root element with the element at the end of the arrya
Updates private variable numfilled by decreasing by 1 to update size
Calls reverse bubble funciton which retains heap priority from the root down
Returns previous temp valalue that is recieve hwen function is called
 */
DataPoint PQHeap::dequeue() {
    if(isEmpty())
    {
        error("Priority Queue is Empty");
    }

    DataPoint returnValue = elements[0];
    elements[0] = elements[size() - 1];
    numFilled--;
    reverseBubble();

    return returnValue;
}

/*
    Returns True if the Size of the fucntion is 0/Empty, Otherwise reutnrs false
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 Returns the private variable numFilled which represents hw many of the allocaterd coapacity of the array is filled with elemtns
 */
int PQHeap::size() const {
    return numFilled;
}

/*
Sets numfileld to 0 to effectively clear the arary of all its elements
If new elements are enqueed then they wull be replced and numfilled will be reincremented
 */
void PQHeap::clear() {
     numFilled = 0;
}

/*
Prints out messaage put in as parameter
Then iterates through the size of the curernt array and pritns out all the elements
Purpose if for easier debugging
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << elements[i]  << endl;
    }
}


void PQHeap::validateInternalState() const {

    for(int i = 0; i < size(); i++)
    {
        int leftChild = getLeftChildIndex(i);
        int rightChild = getRightChildIndex(i);

        if (leftChild < size())
        {
             if (elements[leftChild].priority < elements[i].priority)
             {
                printDebugInfo("Internal State error");
                error("PQHeap has left child out of order at parent index " + integerToString(i));
             }
        }
        if (rightChild < size())
        {
             if (elements[rightChild].priority < elements[i].priority)
             {
                 printDebugInfo("Internal State error");
                error("PQHeap has right child out of order at parent index " + integerToString(i));
             }
        }
    }

}


int PQHeap::getParentIndex(int child) const {

    int parentIndex = (child - 1) / 2;

    return parentIndex;
}


int PQHeap::getLeftChildIndex(int parent) const {

    int leftChild = (2 * parent) + 1;

    return leftChild;
}


int PQHeap::getRightChildIndex(int parent) const {

    int rightChild = (2 * parent) + 2;

    return rightChild;
}


int PQHeap::validateIndex(int request) const
{
    if(request < 0)
    {
        return NONE;
    }

    if(request > size())
    {
        return NONE;
    }

    return 0;
}


/* * * * * * Test Cases Below This Point * * * * * */


TEST("Making sure Bubbling/Enqueue is working correctly"){
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    for (DataPoint dp : input) {
            pq.enqueue(dp);
    }
    pq.validateInternalState();


}

TEST("Testing Enqueue with 0 element input"){
    PQHeap pq;
    Vector<DataPoint> input = {};

    for (DataPoint dp : input) {
            pq.enqueue(dp);
    }
    pq.validateInternalState();


}

TEST("Testing Enqueue with same priorities element input"){
    PQHeap pq;
    Vector<DataPoint> input = {
         { "R", 5 }, { "A", 5 }, { "B", 2 }, { "K", 2 }
    };

    for (DataPoint dp : input) {
            pq.enqueue(dp);
    }
    pq.validateInternalState();

}

TEST("Testing Enlarging Heap"){
    PQHeap pq;

    int index = 0;
    for (char letter = 'a'; letter <= 'z'; letter++)
    {
        index = randomInteger(0,26);
        pq.enqueue({charToString(letter), index});;
    }

    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 26);
    pq.printDebugInfo("test");

}

TEST("Testing Enlarging Heap on large amount of numbers"){
    PQHeap pq;

    int index = 0;
    for (int i = 0; i < 300; i++)
    {
        index = randomInteger(0,300);
        pq.enqueue({"Hi" + integerToString(index), index});;

    }
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 300);
    pq.printDebugInfo("test");

}

TEST("Testing Enlarging Heap to see Allocated Array Size is correct"){
    PQHeap pq;

    int index = 0;
    for (int i = 0; i < 50; i++)
    {
        index = randomInteger(0,50);
        pq.enqueue({"Cool" + integerToString(randomInteger(0,100)), index});;
    }
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 300);

}

TEST("Testing the dequeue function on regular input")
{
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    for (DataPoint dp : input) {
            pq.enqueue(dp);
    }

    EXPECT_EQUAL(pq.dequeue(), { "T", 1 });
    EXPECT_EQUAL(pq.dequeue(), { "G", 2 });
    EXPECT_EQUAL(pq.peek(), { "B", 3 });
    pq.validateInternalState();

}


TEST("Testing the dequeue function on empty input")
{
    PQHeap pq;
    Vector<DataPoint> input = {{ "L", 92 }};

    for (DataPoint dp : input) {
            pq.enqueue(dp);
    }

   EXPECT_EQUAL(pq.dequeue(), { "L", 92 });
   pq.validateInternalState();
   EXPECT_ERROR(pq.dequeue());
}

TEST("Testing the dequeue function at each level")
{
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 12 }, { "A", 8 }, { "B", 3 }, { "K", 6 }, { "G",22 },
         { "V", 31 }, { "T", 4 }, { "O", 9 }, { "S", 6 } };

    for (DataPoint dp : input) {
            pq.enqueue(dp);
    }

   EXPECT_EQUAL(pq.dequeue(), { "B", 3 });
   EXPECT_EQUAL(pq.peek(), { "T", 4 });
    EXPECT_EQUAL(pq.dequeue(), { "T", 4 });
    EXPECT_EQUAL(pq.peek(), { "K", 6 });
    EXPECT_EQUAL(pq.dequeue(), { "K", 6 });
    EXPECT_EQUAL(pq.peek(), { "S", 6 });
    EXPECT_EQUAL(pq.dequeue(), { "S", 6 });
    EXPECT_EQUAL(pq.peek(), { "A", 8 });
    EXPECT_EQUAL(pq.dequeue(), { "A", 8 });
    EXPECT_EQUAL(pq.peek(), { "O", 9 });
    EXPECT_EQUAL(pq.dequeue(), { "O", 9 });
    EXPECT_EQUAL(pq.peek(), { "R", 12 });
    EXPECT_EQUAL(pq.dequeue(), { "R", 12 });
    EXPECT_EQUAL(pq.peek(), { "G",22 });
    EXPECT_EQUAL(pq.dequeue(), { "G",22 });
    EXPECT_EQUAL(pq.peek(), { "V", 31 });
    EXPECT_EQUAL(pq.size(), 1);

}

TEST("Testing the dequeue function using clear function")
{
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 12 }, { "A", 8 }, { "B", 3 }, { "K", 6 }, { "G",22 },
         { "V", 31 }, { "T", 4 }, { "O", 9 }, { "S", 6 } };

    for (DataPoint dp : input) {
            pq.enqueue(dp);
    }

    pq.clear();

    EXPECT_ERROR(pq.dequeue());

}

TEST("PQHEAP enqueuing element last with lowest priority")
{
    PQHeap pq;

    pq.enqueue({ "Hi", 6 });
    pq.enqueue({ "I", 3 });
    pq.enqueue({ "am", 7});
    pq.enqueue({ "am", 10});

    DataPoint expected = { "am", 10 };
    while (pq.size() > 1) {
        pq.dequeue();
    }
    EXPECT_EQUAL(pq.peek(), expected);

}

TEST("PQHeap checking the size of an enqueued fucntion")
{
    PQHeap pq;

    for(int i = 0; i < 22; i++)
    {
        pq.enqueue({i + "Hi", i });
    }

    pq.dequeue();
    EXPECT_EQUAL(pq.size(), 21);

}

TEST("PQHeap example from writeup") {
    PQHeap pq;

    pq.enqueue( { "Zoe", -3 } );
    pq.enqueue( { "Elmo", 10 } );
    pq.enqueue( { "Bert", 6 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.printDebugInfo("After enqueue 3 elements");

    pq.enqueue( { "Kermit", 5 } );
    EXPECT_EQUAL(pq.size(), 4);
    pq.printDebugInfo("After enqueue one more");

    DataPoint removed = pq.dequeue();
    DataPoint expected = { "Zoe", -3 };
    EXPECT_EQUAL(removed, expected);
    pq.printDebugInfo("After dequeue one");
}

TEST("PQHeap, enqueue only, validate at every step") {
    PQHeap pq;

    pq.enqueue({ "e", 2.718 });
    pq.validateInternalState();
    pq.enqueue({ "pi", 3.14 });
    pq.validateInternalState();
    pq.enqueue({ "phi", 1.618 });
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 3);
}

TEST("PQHeap: operations size/isEmpty/clear") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({ "", 7 });
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 3);
    pq.clear();
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

TEST("PQHeap: dequeue or peek on empty queue raises error") {
    PQHeap pq;
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


TEST("PQHeap, dequeue, validate at every step") {
    PQHeap pq;

    pq.enqueue({ "e", 2.718 });
    pq.enqueue({ "pi", 3.14 });
    pq.enqueue({ "phi", 1.618 });

    for (int i = 0; i < 3; i++) {
        pq.dequeue();
        pq.validateInternalState();
    }
}


TEST("PQHeap, test enlarge array memory") {
    for (int size = 5; size <= 500; size *= 5) {
        PQHeap pq;

        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", double(i) });
        }
        pq.validateInternalState();

        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", double(i) };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
    }
}

TEST("PQHeap, sequence of mixed operations") {
    PQHeap pq;
    int size = 30;
    double val = 0;

    for (int i = 0; i < size; i++) {
        pq.enqueue({"", --val });
    }
    val = 0;
    for (int i = 0; i < pq.size(); i++) {
        DataPoint front = pq.peek();
        EXPECT_EQUAL(pq.dequeue(), front);
        pq.enqueue( { "", ++val });
    }
    EXPECT_EQUAL(pq.size(), size);
    val = 0;
    while (!pq.isEmpty()) {
        DataPoint expected = { "", ++val };
        EXPECT_EQUAL(pq.dequeue(), expected);
    }
}

TEST("PQHeap stress test, cycle many random elements in and out") {
    PQHeap pq;
    int n = 0, maxEnqueues = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    setRandomSeed(42); // make test behavior deterministic

    DataPoint mostUrgent = { "", 0 };
    pq.enqueue(mostUrgent);
    while (true) {
        if (++n < maxEnqueues && randomChance(0.9)) {
            DataPoint elem = { "", randomInteger(-10, 10) + 0.5 };
            if (elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            if (pq.isEmpty()) break;
            mostUrgent = pq.peek();
        }
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}



PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
            pq.enqueue(dp);
            pq.validateInternalState();

    }

        while (!pq.isEmpty()) {
            pq.dequeue();
            pq.validateInternalState();
        }
}
