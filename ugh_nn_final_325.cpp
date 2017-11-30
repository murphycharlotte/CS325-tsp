/*Group 49
 *Dylan Markovic, Charlotte Murphy, Terezie Schaller
 *CS325-400
 *Final
 *Due Date: 12.01.2017
 */
 #include <iostream>
 #include <fstream>
 #include <sstream>
 #include <sstream>
 #include <string>
 #include <vector>
 #include <cmath>


 //change DEBUG to 0 to run without debugging output messages
 #define DEBUG 0

//struct city holds four ints, the id, the x coordinate, the y coordinate, and the visited value
struct city{
	int id;
	int x;
	int y;
	int visited;
};

int distance(struct city a, struct city b);

void nearestNeighborTour(std::vector<int> &myRoute, int size, std::vector<city> &mycityList);

int nearestNeighbor(int size, std::vector<city> &mycityList, int curCity);

int tourLength(std::vector<int> &myRoute, int size, std::vector<city> &mycityList);

std::vector<int> swapEdges(std::vector<int> &myRoute, int i, int k);

std::vector<int> TSP_2opt(std::vector<int> &myRoute, int size, std::vector<city> &mycityList);

int main(int argc, char *argv[]){
	//make sure number of arguments is correct
	if (argc != 3){
		std::cout<<"Invalid number of arguments\n";
		return 0;
	}
		
	std::string line; //var to hold each line of file being read
	std::ifstream inputFile;
	inputFile.open(argv[1]);
	//check for successful file open
	if(!inputFile.is_open()){
		std::cout<< "Could Not Open Input File: " <<argv[1]<<"\n";
		return 0;
	}

	//container to hold cities
	std::vector<city> cityList;
	int count = 0; //used for testing
	while(std::getline(inputFile, line)){
		std::stringstream cityStrm(line);
		struct city temp;
		cityStrm>>temp.id>>temp.x>>temp.y;
		cityList.push_back(temp);
		count++;
	}
	
	int size = (int)cityList.size(); //var to store city list size;

	//vectors of city ids representing a tour
	std::vector<int> nRoute(size);	//classic tour
	std::vector<int> tRoute(size);	// nearest neighbor search tour

	//classic route gets city ids in order per input file
	for (int i = 0; i < size; i++) {
		nRoute[i] = tRoute[i] = cityList[i].id;
		cityList[i].visited = 0;
	}

		
	//close file
	inputFile.close();
	
	//for each city, add closest, unvisited neighbor to next position in the tour
	//indicate closest neighbor as visited
	cityList[nRoute[0]].visited = 1;
	for (int i = 0; i < size - 1; i++) {
		nRoute[i + 1] = nearestNeighbor(size, cityList, nRoute[i]);
		cityList[nRoute[i + 1]].visited = 1;
	}
	int nLength = tourLength(nRoute, size, cityList);
	
	for (int i = 0; i < size; i++) {		
		cityList[tRoute[i]].visited = 0;
	}	
	nearestNeighborTour(tRoute, size, cityList);
	int tLength = tourLength(tRoute, size, cityList);

	//print each tour length
	std::cout << nLength << "\t" << tLength << std::endl;

	//Print each route and distance from previous city side by side
	std::cout << nRoute[0] << "\t0\t\t" << tRoute[0] << "\t0" << std::endl;
	for (int i = 1; i < 20; i++){
		std::cout << nRoute[i] << "\t" << distance(cityList[nRoute[i - 1]], cityList[nRoute[i]]) << "\t\t";
		std::cout << tRoute[i] << " \t" << distance(cityList[tRoute[i - 1]], cityList[tRoute[i]]) << "\t";

		if (nRoute[i] != tRoute[i]) 
			std::cout << "!!!";
		std::cout << std::endl;
	}


	
    
	
	return 0;
	
} //end main


//function definitions
 
int distance(struct city a, struct city b){
	return round(sqrt(pow((b.x - a.x),2) + pow((b.y - a.y),2)));
} 

void nearestNeighborTour(std::vector<int> &myRoute, int size, std::vector<city> &mycityList){

	int minDist;
	int tempDist;
	int newRouteIdx;
	int tempCity;
	//indicate start city as visited
	mycityList[myRoute[0]].visited = 1;
	//for each city, add closest, unvisited neighbor to next position in tour
	//indicate closest neighbor as visited
	for (int i = 0; i < size - 1; i++) {
		//minDist gets distance to next city in tour
		minDist = distance(mycityList[myRoute[i]], mycityList[myRoute[i + 1]]);
		//newRouteIdx gets index of next city in tour
		newRouteIdx = i + 1;
		//if a closer, unvisited city is found, update minDist and newRouteIdx 
		for (int j = i + 1; j < size; j++) {
			tempDist = distance(mycityList[myRoute[i]], mycityList[myRoute[j]]);

			if (tempDist < minDist && mycityList[myRoute[j]].visited != 1){
				minDist = tempDist;
				newRouteIdx = j;
			}
		}
		//if a closer city than initial nearest neighbor is found, swap city positions
		if (newRouteIdx > (i + 1)) {
			tempCity = myRoute[i + 1];
			myRoute[i + 1] = myRoute[newRouteIdx];
			myRoute[newRouteIdx] = tempCity;
		}
		//indicate neighbor as visited
		mycityList[myRoute[i + 1]].visited = 1;
	}
}

//given an index of a city in cityList, finds index of closest, unvisted city
int nearestNeighbor(int size, std::vector<city> &mycityList, int curCity){
	int minDist = 0;
	int tempDist;
	int nearestNeighbor;
	//loop through all cities
	for (int i = 0; i < size; i++) {
		if (mycityList[i].visited != 1)	{
			tempDist = distance(mycityList[curCity], mycityList[i]);
			//if city at index i is unvisited & closest neighbor 
			if (minDist == 0 || tempDist < minDist) {
				minDist = tempDist;
				nearestNeighbor = i;
			}
		}
	}
	
	return nearestNeighbor;
}

//helper for 2 opt
//finds the length of a tour given by a vector of integers
//integers in vectors are indexes of cities in cityList
//includes the edge connecting end to start
int tourLength(std::vector<int> &myRoute, int size, std::vector<city> &mycityList){

    //loop through all edges
    int length = 0;
    for (int i = 0; i < size-1; i++){

        int nextEdge = distance(mycityList[myRoute[i]], mycityList[myRoute[i+1]]);
        length += nextEdge;

    }

    //add edge from end to start
    int lastEdge = distance(mycityList[myRoute.back()], mycityList[myRoute[0]]);
    length += lastEdge;

    return length;

}

//helper function for 2 opt
//takes a vector and 2 integers that are indices within the vector
//swaps edges at those indices
std::vector<int> swapEdges(std::vector<int> &myRoute, int i, int k){
    std::vector<int> newRoute;
    for(int p = 0; p < i; p++){
        newRoute.push_back(myRoute[p]);
    }

    for(int p = k; p > i-1; p--){
        newRoute.push_back(myRoute[p]);
    }

    for(int p = k+1; p < (int)(myRoute.size()); p++){
        newRoute.push_back(myRoute[p]);
    }

    return newRoute;
}

//main 2OPT function
//need to continue this as long as it improves tour length
//put it in a do-while loop
//tests if edge swithces are an improvement
std::vector<int> TSP_2opt(std::vector<int> &myRoute, int size, std::vector<city> &mycityList){
    std::vector<int> approxRoute(size);
    std::vector<int> bestRoute(size);
    bestRoute = myRoute;
    int curDist = tourLength(myRoute, size, mycityList);
    int newDist = 0;
    for (int i = 1; i < size; i++){
        for (int k = i + 1; k < size; k++){
            approxRoute = swapEdges(myRoute, i, k);
            newDist = tourLength(approxRoute, size, mycityList);
            if (newDist < curDist){
                bestRoute = approxRoute;
            }
        }
    }

    return bestRoute;
}