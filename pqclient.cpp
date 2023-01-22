#include "pqclient.h"
#include "pqarray.h"
#include "pqheap.h"
#include "vector.h"
#include "strlib.h"
#include <sstream>
#include "testing/SimpleTest.h"
using namespace std;


void pqSort(Vector<DataPoint>& v) {
    PQHeap pq;

    /* Using the Priority Queue data structure as a tool to sort, neat! */

    /* Add all the elements to the priority queue. */
    for (int i = 0; i < v.size(); i++) {
        pq.enqueue(v[i]);
    }

    /* Extract all the elements from the priority queue. Due
     * to the priority queue property, we know that we will get
     * these elements in sorted order, in order of increasing priority
     * value. Store elements back into vector, now in sorted order.
     */
    for (int i = 0; !pq.isEmpty(); i++) {
        v[i] = pq.dequeue();
    }
}


Vector<DataPoint> topK(istream& stream, int k) {

    PQHeap pq;

    DataPoint cur;
    while (stream >> cur) {
        if(pq.size() < k)
        {
            pq.enqueue(cur);
        }
        else
        {
           if (cur.priority > pq.peek().priority)
           {
                pq.dequeue();
                pq.enqueue(cur);
           }

        }
    }

    Vector<DataPoint> data(pq.size());

    for (int i = pq.size() - 1; i >= 0; i--)
    {
        if(!pq.isEmpty())
        {
            data[i] = pq.dequeue();
        }
    }

    return data;
}




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
        DataPoint pt = { "", double(i) };
        result << pt;
    }
    return result;
}

/* Helper function to fill vector with n random DataPoints. */
void fillVector(Vector<DataPoint>& vec, int n) {
    vec.clear();
    for (int i = 0; i < n; i++) {
        DataPoint pt = { "", randomReal(0, 100) };
        vec.add(pt);
    }
}



TEST("topK: time trial, n stays the same and k changes") {
    int n = 300000;
    int k = 3000;
    for (int i = 0; i < 4; i++) {
        k *= 2;
        Vector<DataPoint> input;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(k, topK(stream, k));
    }
}



TEST("Time trial of pqSort using PQ Array") {
    Vector<DataPoint> input;


    int n = 12500;
    fillVector(input, n);
    TIME_OPERATION(n, pqSort(input));

    n = 25000;
    fillVector(input, n);
    TIME_OPERATION(n, pqSort(input));

    n = 50000;
    fillVector(input, n);
    TIME_OPERATION(n, pqSort(input));

    n = 100000;
    fillVector(input, n);
    TIME_OPERATION(n, pqSort(input));

}

TEST("Time trial of pqSort using PQHeap") {
    Vector<DataPoint> input;

    int n = 30000;

    for (int i = 0; i < 4; i++)
    {
        fillVector(input, n);
        TIME_OPERATION(n, pqSort(input));
        n*= 2;
    }

}

TEST("topK: time trial, k stays the same and n changes using PQ HEAP") {
    int n = 200000;
    int k = 10;
    for (int i = 0; i < 4; i++) {
        n *= 3;
        Vector<DataPoint> input;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

TEST("topK: time trial, n stays the same and k changes") {
    int n = 300000;
    int k = 3000;
    for (int i = 0; i < 4; i++) {
        k *= 2;
        Vector<DataPoint> input;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(k, topK(stream, k));
    }
}

TEST("topK: time trial, k stays the same and n changes") {
    int n = 200000;
    int k = 10;
    for (int i = 0; i < 4; i++) {
        n *= 3;
        Vector<DataPoint> input;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}




TEST("pqSort: vector of random elements") {
    setRandomSeed(137); 

    Vector<DataPoint> input;
    Vector<double> expected;
    fillVector(input, 100);
    for (DataPoint dp : input) {
        expected.add(dp.priority);
    }
    expected.sort();

    pqSort(input);
    for (int i = 0; i < input.size(); i++) {
        EXPECT_EQUAL(input[i].priority, expected[i]);
    }
}

TEST("pqSort: time trial") {
    int startSize = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        fillVector(v, n);
        TIME_OPERATION(n, pqSort(v));
    }
}


/* Constant used for sizing the tests below this point. */
const int kMany = 100000;

TEST("topK: stream 0 elements, ask for top 1") {
    stringstream emptyStream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(emptyStream, 1), expected);
}

TEST("topK: stream 1 element, ask for top 1") {
    stringstream stream = asStream({ { "" , 1 } });
    Vector<DataPoint> expected = { { "" , 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

TEST("topK: small hand-constructed input") {
    Vector<DataPoint> input = { { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 } };

    stringstream stream = asStream(input);
    Vector<DataPoint> expected = { { "D", 4 }, { "C", 3 } };
    EXPECT_EQUAL(topK(stream, 2), expected);

     stream = asStream(input);
    expected = { { "D", 4 }, { "C", 3 }, { "B", 2 }, { "A", 1 } };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

TEST("topK: stream many elements, ask for top 1") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = { { "" , kMany } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

TEST("topK: stream many elements, ask for top 5") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = {
        { "" , kMany     },
        { "" , kMany - 1 },
        { "" , kMany - 2 },
        { "" , kMany - 3 },
        { "" , kMany - 4 },
    };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

TEST("topK: stress test, many elements, random values") {
    Vector<double> expected;
    Vector<DataPoint> points;
    fillVector(points, 10000);
    for (DataPoint dp : points) {
        expected.add(dp.priority);
    }
    sort(expected.begin(), expected.end(), greater<double>());

    stringstream stream = asStream(points);
    int k = 10;
    Vector<DataPoint> result = topK(stream, k);
    EXPECT_EQUAL(result.size(), k);
    for (int i = 0; i < k; i++) {
        EXPECT_EQUAL(result[i].priority, expected[i]);
    }
}

TEST("topK: stress test - many elements, ask for top half") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> result = topK(stream, kMany/2);
    EXPECT_EQUAL(result.size(), kMany/2);
    EXPECT_EQUAL(result[0].priority, kMany);
    EXPECT_EQUAL(result[result.size()-1].priority, kMany - result.size() + 1);
}



