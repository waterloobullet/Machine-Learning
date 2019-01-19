#include <iostream>
#include <random>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <math.h>   

using namespace std;

#define LYRAND (double)rand()/RAND_MAX

class city {
public:
	int cityNum;
	double cityCoordX;
	double cityCoordY;
};

class policy {
public:

	int maxCities = 10;
	double travel; //calculated travel distance
	double fitness; //fitness of minimum travel distance
	void mutate(); //randomly select policy to mutate
	vector <int> travelSchedule;
	void selectTravels();
};

class agent {
public:
	double agentCoordX;
	double agentCoordY;

};

void policy::mutate()
{
  {
	int a, b, c, d;
	int tempSchedule1, tempSchedule2, tempSchedule3, tempSchedule4;

		//select 2 random elements and swap them
		a = rand() % maxCities;
		b = rand() % maxCities;
		//check that the random values are not the same
		if (a == b)
		{
			b = rand() % maxCities;
			//assert(a != b);
			if (a == b)
			{
				b = rand() % maxCities;
				//assert(a != b);
			}
		}
		tempSchedule1 = travelSchedule.at(a);
		tempSchedule2 = travelSchedule.at(b);
		travelSchedule.at(a) = tempSchedule2;
		travelSchedule.at(b) = tempSchedule1;

		//select 2 more random elements and swap them
		c = rand() % maxCities;
		d = rand() % maxCities;
		//check that the random values are not the same
		if (c == d)
		{
			d = rand() % maxCities;
			//assert(c != d);
			if (c == d)
			{
				d = rand() % maxCities;
				//assert(c != d);
			}
		}
		//swap
		tempSchedule3 = travelSchedule.at(c);
		tempSchedule4 = travelSchedule.at(d);
		travelSchedule.at(c) = tempSchedule4;
		travelSchedule.at(d) = tempSchedule3;
	}
}


void policy::selectTravels()
{
	int a, i, j, u;
	travelSchedule.clear();
	travelSchedule.resize(maxCities);
	travelSchedule.push_back(0);
	bool unique;

	//Select Path Randomly
	for (i = 1; i < 10; i++)
	{
		u = 0;
		unique = false;
		do
		{
			//Select Random Number between 1 and 10

			a = rand() % maxCities;
			//Compare to each value in vector to see if it is a repeat
			for (j = 0; j <= i; j++)
			{

				if (a == travelSchedule.at(j))
				{
					u++;
				}
			}
			//Then check if copies were found
			if (u == 0)
			{
				unique = true;
			}
			u = 0;
		} 
		while
			(unique == false);
			cout << a << ", ";
			travelSchedule.at(i) = a;


	}

}

double calcDistance(vector <city> cities, vector <int> path, int maxCities);

void sortByShortest(vector <policy> & trials, int maxTrials);


int main()
{

	srand(time(NULL));

	//Create USA Map
	//int numOfCities
	int numOfCities = 10;
	int halfOfCities = numOfCities / 2;
	int i;//For Generating 9 more random paths
	int j;//For running the evolutionary algorithm
	int loops = 100;//for running the evolutionary algorithm

	//Vector <city>
	vector <city> usa;
	usa.clear();

	//San Francisco
	city sanFrancisco;
	sanFrancisco.cityNum = 0;
	sanFrancisco.cityCoordX = 2;
	sanFrancisco.cityCoordY = 12;
	usa.push_back(sanFrancisco);

	//New York City
	city newYorkCity;
	newYorkCity.cityNum = 1;
	newYorkCity.cityCoordX = 18;
	newYorkCity.cityCoordY = 12;
	usa.push_back(newYorkCity);

	//Washington DC
	city washingtonDC;
	washingtonDC.cityNum = 2;
	washingtonDC.cityCoordX = 17;
	washingtonDC.cityCoordY = 10;
	usa.push_back(washingtonDC);

	//Chicago
	city chicago;
	chicago.cityNum = 3;
	chicago.cityCoordX = 10;
	chicago.cityCoordY = 17;
	usa.push_back(chicago);

	//Boston
	city boston;
	boston.cityNum = 4;
	boston.cityCoordX = 19;
	boston.cityCoordY = 13;
	usa.push_back(boston);

	//Los Angeles
	city losAngeles;
	losAngeles.cityNum = 5;
	losAngeles.cityCoordX = 3;
	losAngeles.cityCoordY = 5;
	usa.push_back(losAngeles);

	//Seattle
	city seattle;
	seattle.cityNum = 6;
	seattle.cityCoordX = 2;
	seattle.cityCoordY = 17;
	usa.push_back(seattle);

	//Austin
	city austin;
	austin.cityNum = 7;
	austin.cityCoordX = 10;
	austin.cityCoordY = 4;
	usa.push_back(austin);

	//Houston
	city houston;
	houston.cityNum = 8;
	houston.cityCoordX = 11;
	houston.cityCoordY = 3;
	usa.push_back(houston);

	//San Diego
	city sanDiego;
	sanDiego.cityNum = 9;
	sanDiego.cityCoordX = 4;
	sanDiego.cityCoordY = 3;
	usa.push_back(sanDiego);

//Initialize agent, and always start in San Francisco
	agent salesman;
	salesman.agentCoordX = sanFrancisco.cityCoordX;
	salesman.agentCoordY = sanFrancisco.cityCoordY;

//Run First Trial and initialize loop for 100 trails
	vector <policy> trial;
	trial.clear();
	trial.resize(100);
	trial.at(0).selectTravels();
	trial.at(0).travel = calcDistance(usa, trial.at(0).travelSchedule, numOfCities);
	assert(trial.at(0).travel != 0.0);

	/*This is just to test mutate
	trial.at(0).mutate();
	for (i = 0; i < 10; i++)
	{
		cout << trial.at(0).travelSchedule.at(i) << ", ";
	}
	trial.at(0).travel = calcDistance(usa, trial.at(0).travelSchedule, numOfCities);

	system("PAUSE");
	*/

//Create 9 more Random Paths for 10 total
	for (i = 1; i < numOfCities; i++)
	{
		trial.at(i).selectTravels();
		trial.at(i).travel = calcDistance(usa, trial.at(i).travelSchedule, numOfCities);
		assert(trial.at(i).travel != 0.0);
	}

for (j = 0; j < loops; j++)
{
//Once at 10, sort by shortest distance, and cut off bottom half of values
	sortByShortest(trial, numOfCities);

	//Verify Sorted Paths
	for (i = 1; i < numOfCities; i++)
	{
		cout << trial.at(i).travel << ", ";
	}

//Mutate 5 best paths and replace them into 5 worst paths
	for (i = 0; i < halfOfCities; i++)
	{
		//Replace current path with opposite end of the vector path
		trial.at(i) = trial.at((numOfCities-1) - i);
		//mutate and recalculate distance
		trial.at(i).mutate();
		trial.at(i).travel = calcDistance(usa, trial.at(i).travelSchedule, numOfCities);

	}

}



//Mutate each top half path and replace old
//mutate(trial, 10);

//Repeat
	//Once at 10, sort by shortest distance, and cut off bottom half of values

	//Mutate each top half path and replace old

	cout << "whatup" << endl;
	system("PAUSE");
	return 0;
}

double calcDistance(vector <city> cities, vector <int> path, int maxCities)
{
	int d, e, f;
	double currCity, currCityX, currCityY, nextCity, nextCityX, nextCityY;
	double distance = 0.0;
	double distanceBetween, distanceBetweenX, distanceBetweenY;

	for (d = 0; d < (maxCities-1); d++)
	{
		currCity = path.at(d);
			e = d + 1;
		nextCity = path.at(e);
		for (f = 0; f < maxCities; f++)
		{
			if (currCity == (cities.at(f).cityNum))
			{
				currCityX = cities.at(f).cityCoordX;
				currCityY = cities.at(f).cityCoordY;
				//cout << cities.at(f).cityNum << endl;
			}
			if (nextCity == (cities.at(f).cityNum))
			{
				nextCityX = cities.at(f).cityCoordX;
				nextCityY = cities.at(f).cityCoordY;
				//cout << cities.at(f).cityNum << endl;
			}
		}
			//Calculate and keep numbers positive
			distanceBetweenX = nextCityX - currCityX;
			if (distanceBetweenX < 0)
			{
				distanceBetweenX = distanceBetweenX*(-1);
			}
			distanceBetweenY = nextCityY - currCityY;
			if (distanceBetweenY < 0)
			{
				distanceBetweenY = distanceBetweenY*(-1);
			}
			distanceBetween = sqrt((distanceBetweenX*distanceBetweenX) + (distanceBetweenY*distanceBetweenY));
			distance = distance + distanceBetween;
	}
	cout << endl << distance << endl;
	return distance;
}

void sortByShortest( vector <policy> & trials, int maxTrials)
{
	double currDistance, nextDistance;//for comparison
	int g, h, j;//for loops
	policy tempTrial;

	//Select test element
	for (g = 0; g < maxTrials; g++)
	{
		currDistance = trials.at(g).travel;
		//Compare and swap for test element
		for (h = 0; h < maxTrials; h++)
		{
			nextDistance = trials.at(h).travel;
			if (currDistance > nextDistance)
			{
				tempTrial = trials.at(h);
				trials.at(h) = trials.at(g);
				trials.at(g) = tempTrial;
			}
		}


	}
		//if longer swap and test next element
		//if longer
			//repeat until 
}