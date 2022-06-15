/**
 * Designing a perfect hash function is a non-trivial task. In addition, the performance of a hash function 
 * highly depends on the properties of data set. For “separate chaining” hash table, one goal of the hash 
 * function is to reduce the maximum length of the linked list among all the table bins.
 * 
 * One simple variant of the above hash table is called “2-choice hashing”, where you use two 
 * independent hash functions (h1 and h2) instead of just one. For each object, you will use the two hash 
 * functions to give you two choices of positions in the single table. Please note this is different from 
 * “double hashing”.
 * 
 * Insert: With two bins to choose in the table, your strategy is to first check which bin contains the shorter 
 * linked list, then insert the new object to the front of that shorter linked list. Otherwise, if two linked lists 
 * are of the same length, you insert to the bin returned by your first hash function h1.
 * 
 * Search: like insert, you use the two hash functions to find out the two bin locations in the table, you will 
 * search both two bins in the table since you won’t know which bin contains your search object. 
 * 
 * Please note: the hash table should not contain duplicate objects.
 * 
 * It is possible that two hash functions may sometimes return the same bin position. Then just do insert 
 * and search only for that bin. That’s why we want the two hash functions to be independent, so that this 
 * won’t happen too regularly. 
 * 
 * For more details on “2-choice hashing”, please refer to https://en.wikipedia.org/wiki/2-choice_hashing
 * 
 * Data set: We will use the data file (Grocery_UPC_database.csv) provided. You will insert the items in 
 * the same order as in the file. Don’t modify this file in any way because your output will depend on that. 
 * Here are just some samples from it:
 * 
 * ----------------------------------------------------------------------------------------------------
 * 773743500068,Beveri Golden Flaxseed Fine Milled With Mixed Berries - 1 Lb
 * 895172001487,Pure Life Shampoo Lavender - 14.9 Fl Oz
 * 773743500051,Beveri Golden Flaxseed Fine Milled With Cranberry - 1 Lb
 * 895172001432,Pure Life Body Lotion Coconut And Mango - 14.9 Fl Oz
 * ----------------------------------------------------------------------------------------------------
 * 
 * 2-choice hash functions: Each object contains the UPC number as upc and the description string as 
 * desc, we define two independent hash functions:
 * 
 * h1(upc) = upc % tableSize
 * h2(desc) = abs(desc[0]+27*desc[1]+729*desc[2]) % tableSize
 * 
 * We know the first 3 characters (could be number, letter, punctuation, or even space) of the description 
 * usually are from the brand of the item, so we expect lots of collisions for h2. We also know the UPC 
 * format’s last few digits are independent of brand. So h1 and h2 should be independent from each other. 
 * Table size: Your program will test different table sizes: 1000, 10,000, 100,000. With total around 
 * 110,000 items from the data file, if we choose a table of size 100,000, then ideally, we hope to have 
 * each bin contains only one or two objects. If table size is 10,000, ideally the bin length should be around 
 * 10 for each bin, and so on. Your code will report “standard deviation” (code provided) to describe the 
 * difference from the ideal cases.
 * 
 * Output: 
 * There are already some sample tests (using test_data.csv) included in the main function. 
 * You get 1 point after the sample tests pass.
 * After that, your program will be checked with the full database. It will print 3 standard deviations for 3 
 * different table sizes. Each correct output earns 4 points, total 12 points.
 * Your program will also print out 18 positions (6 test items for each table size), each correct position 
 * earns 1.5 points, total 27 points
 * 1 + 12 + 27 = 40 point in total.
 */

#include <iostream>

#include "twoHashTable.h"
#include "UPCEntry.h"
#include "list.cpp"
#include <assert.h>
using namespace std;

float grade = 0;

void autograde(string name, bool result, float points)
{
    if (result)
        grade += points;
    else
    {
        cout << "error: " << name << endl;
    }
}

int main()
{
    int testSize = 6;
    UPCEntry *items[testSize];
    items[0] = new UPCEntry("753950001954,Doctor's Best Best Curcumin C3 Complex 1000mg Tablets - 120 Ct");
    items[1] = new UPCEntry("025800024117,Weight Watchers Smart Ones Smart Creations");
    items[2] = new UPCEntry("079927020217,\"Unique \"\"splits\"\" Split-open Pretzel Extra Dark\"");
    items[3] = new UPCEntry("1638098830,Weleda Bar Soap Rose - 3.5 Oz");
    items[4] = new UPCEntry("895172001432,Pure Life Body Lotion Coconut And Mango - 15.0 Fl Oz");
    items[5] = new UPCEntry("995172001432,Pure Life Body Lotion Coconut And Mango - 14.9 Fl Oz");

    TwoHashTable *table = new TwoHashTable("test_data.csv", 3);
    cout << "-- sample run start --" << endl;
    assert(abs(table->getStdDev() - 0.471404f) < 1e-6);
    Position pos = table->search(*items[0]);
    assert(pos.indexInTable == 0 && pos.indexInBin == 3);
    pos = table->search(*items[1]);
    assert(pos.indexInTable == -1 && pos.indexInBin == -1);
    pos = table->search(*items[2]);
    assert(pos.indexInTable == 1 && pos.indexInBin == 0);
    cout << "-- sample run pass --" << endl;

    grade += 1;

    int tableSize[3] = {100000, 1000, 100};
    float correct_std[3] = {1.78235, 21.457, 15.4253};
    int correct_pos[3][6][2] = {{{1954, 5}, {24117, 3}, {20217, 3}, {98830, 0}, {-1, -1}, {-1, -1}},
                                {{954, 49}, {117, 109}, {217, 128}, {830, 0}, {-1, -1}, {-1, -1}},
                                {{54, 1105}, {17, 916}, {17, 1108}, {30, 0}, {-1, -1}, {-1, -1}}};
    for (int n = 0; n < 3; n++)
    {
        TwoHashTable *table = new TwoHashTable("grocery_upc_database.csv", tableSize[n]);
        float std = table->getStdDev();
        cout << "Table size = " << tableSize[n] << ", stddev = " << std << endl;
        autograde("std", abs(std - correct_std[n]) < 1e-3, 4);

        for (int i = 0; i < testSize; i++)
        {
            Position pos = table->search(*items[i]);
            cout << "      [" << pos.indexInTable << "," << pos.indexInBin << "]" << endl;
            autograde("index in table", pos.indexInTable == correct_pos[n][i][0], 1);
            autograde("index in bin", pos.indexInBin == correct_pos[n][i][1], 0.5);
        }
    }
    cout << "Your total grade is: " << grade << endl;
    return 0;
}

/**
 * Your output should look like this:
 * 
 * -- sample run start --
 * -- sample run pass --
 * Table size = 100000, stddev = 1.78235
 *       [1954,5]
 *       [24117,3]
 *       [20217,3]
 *       [98830,0]
 *       [-1,-1]
 *       [-1,-1]
 * Table size = 1000, stddev = 21.457
 *       [954,49]
 *       [117,109]
 *       [217,128]
 *       [830,0]
 *       [-1,-1]
 *       [-1,-1]
 * Table size = 100, stddev = 15.4253
 *       [54,1105]
 *       [17,916]
 *       [17,1108]
 *       [30,0]
 *       [-1,-1]
 *       [-1,-1]
 * Your total grade is: 40
 */