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
 #define DEBUG 1

//struct city holds three ints, the id, the x coordinate, and the y coordinate
struct city{
	int id;
	int x;
	int y;
};

int distance(struct city a, struct city b);

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
		std::cout<< "Could Not Open File: " <<argv[1]<<"\n";
		return 0;
	}

	//container to hold cities
	std::vector<city> cityList;
	int count = 0; //used for testing
	while(!inputFile.eof()){
		std::getline(inputFile, line);
		std::stringstream cityStrm(line);
		struct city temp;
		cityStrm>>temp.id>>temp.x>>temp.y;
		cityList.push_back(temp);
		count++;
	}
	
	int listSize = (int)cityList.size();

	//vector of cityList indices representing a tour
	std::vector<int> route(listSize);
	for (int i = 0; i < listSize; i++) {
		route[i] = i;
	}

	if (DEBUG){
		struct city a, b, c;
		a.id = 1;
		a.x = a.y = 0;
		b.id = 2;
		b.x = 1;
		b.y = 3;
		c.id = 3;
		c.x = 6;
		c.y = 0;
		std::cout<< "distance from a to b should be 3.  It is calculated to be: " << distance(a,b) << "\n";
		std::cout<< "distance from b to c should be 6.  It is calculated to be: " << distance(b,c) << "\n";
		std::cout<< "distance from c to a should be 6.  It is calculated to be: " << distance(c,a) << "\n";
	}
	
	if(DEBUG){
		if(count == listSize)
			std::cout << "count matches cityList size \n";
		else
			std::cout << " count is " << count << " and cityList's size is " << cityList.size() << " !!!!!!!!\n";
	}
	
	if(DEBUG){
		std::cout << tourLength(route, listSize, cityList) << "\n";
		for(int i = 0; i < listSize; i++)
			std::cout<< cityList[i].id << " " << cityList[i].x <<" " << cityList[i].y << "\n";
	}
	
	
	//close file
	inputFile.close();
	
	/*
	 * begin solving tsp problem below
	 */
	int tourLen = -1;
	int tourLen2 = -1;

	do{

        //initial tour length
        tourLen = tourLength(route, listSize, cityList);
        //compares edge switches to see if they improve length
        route = TSP_2opt(route, listSize, cityList);
        //improved on 1 pass length
        tourLen2 = tourLength(route, listSize, cityList);

    }
    while (tourLen != tourLen2);

    if(DEBUG) {
	    //print improved tour
	    for (int i = 0; i < listSize; i++){
	        std::cout << route[i] << " ";
	    }
	    std::cout << "length: " << tourLen << std::endl;
	}

	//output improved tour distance and route to file
	std::ofstream outputFile;
	outputFile.open(argv[2]);
	//check for successfull file open
	if(!outputFile.is_open()){
		std::cout<<"Could Not Open File: " << argv[2] << "\n";
		return 0;
	}
	
	outputFile << tourLen << std::endl;

	for (int j = 0; j < listSize; j++){
		outputFile << cityList[j].id << std::endl;
	}

	outputFile.close();
	
	return 0;
	
} //end main


//function definitions
 
int distance(struct city a, struct city b){
	return round(sqrt(pow((b.x - a.x),2) + pow((b.y - a.y),2)));
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