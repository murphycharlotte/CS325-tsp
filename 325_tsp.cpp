#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sstream>
#include <cmath>

using namespace std;

struct city{
    int id;
    int x;
    int y;
};

int distance(struct city a, struct city b);

int tourLength(vector<int> &myVector, int size, vector<city> &mycityList);

vector<int> swapEdges(vector<int> &myVector, int i, int k);

vector<int> TSP_2opt(vector<int> &myVector, int size, vector<city> &mycityList);

int main() {

    //hardcode in a vector and city list
    //use Dylan's code to get it from txt files in final version
    //make a vector representing an arbitrary tour
    int size = 5;
    vector<int> route(size);
    route[0] = 0;
    route[1] = 1;
    route[2] = 2;
    route[3] = 3;
    route[4] = 4;

    //hardcode some cities
    city cityA;
    city cityB;
    city cityC;
    city cityD;
    city cityE;

    cityA.id = 0;
    cityA.x = 2;
    cityA.y = 2;

    cityB.id = 1;
    cityB.x = 5;
    cityB.y = 8;

    cityC.id = 2;
    cityC.x = 5;
    cityC.y = 3;

    cityD.id = 3;
    cityD.x = 8;
    cityD.y = 6;

    cityE.id = 4;
    cityE.x = 2;
    cityE.y = 7;

    //need a hardcoded city list
    vector<city> cityList;
    cityList.push_back(cityA);
    cityList.push_back(cityB);
    cityList.push_back(cityC);
    cityList.push_back(cityD);
    cityList.push_back(cityE);

    //print original tour

    int tourLen = tourLength(route, size, cityList);
    for (int i = 0; i < size; i++){
        cout << route[i] << " ";
    }
    cout << "length: " << tourLen << endl;

    int tourLen2 = 0;

    //BEGIN 2-OPT
    //repeat edge switching until no more improvement is found

    do{

        //initial tour length
        tourLen = tourLength(route, size, cityList);
        //compares edge switches to see if they improve length
        route = TSP_2opt(route, size, cityList);
        //improved on 1 pass length
        tourLen2 = tourLength(route, size, cityList);

    }
    while (tourLen != tourLen2);

    //print improved tour
    tourLen = tourLength(route, size, cityList);
    for (int i = 0; i < size; i++){
        cout << route[i] << " ";
    }
    cout << "length: " << tourLen << endl;

}//end main


//function definitions

//finds distance between 2 cities
int distance(struct city a, struct city b){
    return round(sqrt(pow((b.x - a.x),2) + pow((b.y - a.y),2)));
}

//helper for 2 opt
//finds the length of a tour given by a vector of integers
//integers in vectors are indexes of cities in cityList
//includes the edge connecting end to start
int tourLength(vector<int> &myVector, int size, vector<city> &mycityList){

    //loop through all edges
    int length = 0;
    for (int i = 0; i < size-1; i++){

        int nextEdge = distance(mycityList[myVector[i]], mycityList[myVector[i+1]]);
        length += nextEdge;

    }

    //add edge from end to start
    int lastEdge = distance(mycityList[myVector.back()], mycityList[myVector[0]]);
    length += lastEdge;

    return length;

}

//helper function for 2 opt
//takes a vector and 2 integers that are indices within the vector
//swaps edges at those indices
vector<int> swapEdges(vector<int> &myVector, int i, int k){
    vector<int> newRoute;
    for(int p = 0; p < i; p++){
        newRoute.push_back(myVector[p]);
    }

    for(int p = k; p > i-1; p--){
        newRoute.push_back(myVector[p]);
    }

    for(int p = k+1; p < myVector.size(); p++){
        newRoute.push_back(myVector[p]);
    }

    return newRoute;
}

//main 2OPT function
//need to continue this as long as it improves tour length
//put it in a do-while loop
//tests if edge swithces are an improvement
vector<int> TSP_2opt(vector<int> &myVector, int size, vector<city> &mycityList){
    vector<int> approxRoute(size);
    vector<int> bestRoute(size);
    bestRoute = myVector;
    int curDist = tourLength(myVector, size, mycityList);
    int newDist = 0;
    for (int i = 1; i < size; i++){
        for (int k = i + 1; k < size; k++){
            approxRoute = swapEdges(myVector, i, k);
            newDist = tourLength(approxRoute, size, mycityList);
            if (newDist < curDist){
                bestRoute = approxRoute;
            }
        }
    }

    return bestRoute;
}
