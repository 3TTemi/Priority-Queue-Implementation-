#pragma once
#include "testing/MemoryUtils.h"
#include "datapoint.h"

/**
 * Priority queue of DataPoints implemented using a sorted array.
 */
class PQArray {
public:
    /**
     * Creates a new, empty priority queue.
     */
    PQArray();

    /**
     * Cleans up all memory allocated by this priority queue.
     */
    ~PQArray();

    /**
     * Adds a new element into the queue. This operation runs in time O(N),
     * where n is the number of elements in the queue.
     *
     * @param element The element to add.
     */
    void enqueue(DataPoint element);

    /**
     * Removes and returns the element that is frontmost in this priority queue.
     * The frontmost element is the one with the most urgent priority. A priority
     * of 1 is more urgent than priority 2 which is more urgent than priority 7
     * and so on. If the priority queue contains two or more elements of equal
     * priority, the order those elements are dequeued is arbitrary, i.e. there
     * is no required tie-break handling.
     *
     * If the priority queue is empty, this function calls error().
     *
     *
     * @return The frontmost element, which is removed from queue.
     */
    DataPoint dequeue();

    /**
     * Returns, but does not remove, the element that is frontmost.
     *
     * If the priority queue is empty, this function calls error().
     *
     *
     * @return frontmost element
     */
    DataPoint peek() const;

    /**
     * Returns whether this priority queue is empty.
     *
     *
     * @return true if contains no elements, false otherwise.
     */
    bool isEmpty() const;

    /**
     * Returns the count of elements in this priority queue.
     *
     *
     * @return The count of elements in the priority queue.
     */
    int size() const;

    /**
     * Removes all elements from the priority queue.
     *
     */
    void clear();


    void printDebugInfo(std::string msg) const;


    void validateInternalState() const;

private:
    DataPoint* _elements;   // dynamic array
    int _numAllocated;      // number of slots allocated in array
    int _numFilled;         // number of slots filled in array

    void enlargeArray();

    void swap(int indexA, int indexB);


    DISALLOW_COPYING_OF(PQArray);
};
