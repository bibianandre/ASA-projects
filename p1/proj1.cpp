/*
 *      File: proj1.cpp
 *      Description: SuperMarble problem solved in c++.
 *      Authors: group 106
 *      Bibiana André (ist194158)
 *      Filipe Abreu (ist1106046)
 */
#include <vector>
#include <iostream>
using namespace std;

typedef unsigned long long ull;
vector<vector<ull>> memo;

void baseCase(int x, int y, int n) {
    //Start arrays at zero
    memo = vector<vector<ull>>(x + 1, vector<ull>(y + 1, 0));

    //Store all the base subproblems (pieces) with their respective price in 
    //the corresponding position(s): (height, width), (width, height) or both if possible.
    for (int i = 1; i <= n; i++) {
        int height, width;
        ull price;

        if (fscanf(stdin, "%d %d %llu", &height, &width, &price) == 3) {

            if ((height <= x) && (width <= y)) {
                memo[height][width] = price;
            }
            if ((width <= x) && (height <= y)) {
                memo[width][height] = price;
            }
        }         
    }
}

//Returns the max value betweet num1 or num2 
ull maxValue(ull num1, ull num2) {   
    ull result = (num1 >= num2) ? num1 : num2;
    return result;
} 

//Maximizes the value of the initial marble board and returns its price
void superMarble(int height, int width) {
    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++) {

            if (i > 1) {//Tries every horizontal cut possible 
                for (int n = 1; 2 * n <= i; n++) {
                    memo[i][j] = maxValue(memo[n][j] + memo[i - n][j], memo[i][j]);
                }
            }
            
            if (j > 1) {//Tries every vertical cut possible 
                for (int n = 1; 2 * n <= j; n++) {
                    memo[i][j] = maxValue(memo[i][n] + memo[i][j - n], memo[i][j]);
                }
            }  
        }
    }
}

int main() {
    int x, y, n; //x = height, y = width, n = number of types of pieces

    if (fscanf(stdin, "%d %d", &x, &y) != 2 || fscanf(stdin, "%d", &n) != 1)
        return 0;

    baseCase(x, y, n);
    superMarble(x, y);

    printf("%llu\n", memo[x][y]);

    return 0;
}