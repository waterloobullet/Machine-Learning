// QLearner.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//What headers will be included
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <random>
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;


//Class definitions
//Pixels to paint
class pixel
{
public:

	//Where is the pixel
	//X Y locations
	int x;
	int y;
	int color;
	bool visited;
	void initPix();
	double qVal;
	
	//Is the pixel painted or not
	//Bool: Painted

};

class Agent
{
public:
	//initialize vector for agent to store its arms
	// prime vector to best value being element 0
	//Where does the agent want to go?
	int xMax, yMax;
	int curX, curY;
	pixel curr;
	vector <pixel> q;
	vector<pixel> program;
	void initAg();

	//Initialize values for alpha,  and epsilon to
	//balance exploration and exploitation
	//iterations
	int bestArm = -1;
	double reward = 0.0;
	//choice
	int directions;
	double distanceTravelled;
	double alpha = 0.1;
	double epsilon = 0.1;
	double gamma = 0.0;
	int action;
	int choice();


	//randomly choose between exploration and exploitation
	//Make last best action (explore)
	int rewardingAction();
	//Make random action (exploit)
	int randomAction();
	//Create a statiscal update for analysis
	void update();

};

void pixel::initPix()
{
	visited = false;
}

void Agent::initAg()
{
	int a = 0;
	for (a; a < directions; a++)
	{
		double close = ((double)rand() / RAND_MAX) / RAND_MAX;
		curr.qVal = close;
		curr.visited = false;
		q.push_back(curr);
	}
}

int Agent::choice()
{
	int b = 0;
	for (b; b < directions; b++)
	{
		double c = (double)rand() / RAND_MAX;
		if (c < epsilon)
			action = randomAction();
		else
			action = rewardingAction();
	}
	return action;
}

int Agent::randomAction()
{
	//select random direction
	int d;
	d = rand() % directions;
	while (q.at(d).visited == true)
	{
		d = rand() % directions;
	}
	return d;
}

int Agent::rewardingAction()
{
	//find most rewarding arm
	double bestOne = -1000000;
	int d = 0;
	for (d; d < directions ; d++)
	{
		if (q.at(d).visited != true)
		{
			if (q.at(d).qVal > bestOne)
			{

				bestArm = d;
			}
		}
		bestOne = q.at(d).qVal;
	}
	return bestArm;
}
void Agent::update()
{
	q.at(action).qVal = q.at(action).qVal + alpha*(reward + gamma - q.at(action).qVal);
}

//Main Function Definition
double calcDist(pixel &current, pixel &next);
void visitReset(vector<pixel> &pixels, int choices);

//Main



int main()
{
	srand(time(NULL));
	//Initialize Variables
	//Distance, Counts, & Locations
	int i, generation;
	int maxGenerations = 10;
	int j = 0;
	int val;
	int curr, next;
	int x, y;
	double distFromOrigin, distFromLast, totalDist;
	//initialize vectors
	vector <pixel> image;
	vector <pixel> path;
	vector <pixel> currHist;
	pixel current;
	image.clear();
	path.clear();
	//Initialize Agent
	Agent picaso;
	vector <Agent> trials;
	trials.clear();
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Initialize file input/output
	char *inname = "coordinates.txt";
	ifstream BMdata(inname);

	if (!BMdata)
	{
		cout << "There was a problem opening file " << inname << " for reading." << endl;
		system("PAUSE");
		return 0;
	}
	//Pull max values from file
	BMdata >> val;
	picaso.xMax = val;
	BMdata >> val;
	picaso.yMax = val;
	picaso.directions = 0;
	
	//Export data, and select locations to visit
	while (BMdata >> i)
	{
		//cout << "Value from file is " << i << endl;
		//send values to current pixel
		current.x = i;
		BMdata >> i;
		current.y = i;
		BMdata >> i;
		current.color = i;
		image.push_back(current);
		if (current.color == 0)
		{

			path.push_back(current);
			path.at(j).initPix();
			picaso.directions = j;
			j++;
			
			//cout << current.x << " " << current.y << " " << current.color << endl;
		}
				
	}
	BMdata.close();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Now that the wanted path is known, start at 0,0
	picaso.curX = 0;
	picaso.curY = 0;
	picaso.initAg();
	//find state
	ofstream simulation;
	simulation.open("data.txt");
	if (!simulation)
	{
		cout << "There was a problem opening file " << inname << " for reading." << endl;
		system("PAUSE");
		return 0;
	}
	for (generation = 0; generation < maxGenerations; generation++)
	{
	curr = 0;
	//Clear History for current Generation
	currHist.clear();
	//Initialize Agent for Current Generation
	path.at(curr).visited = true;
	picaso.curr = path.at(curr);
	//Send First value to Agent
	currHist.push_back(picaso.curr); 
	totalDist = 0.0;

		for (i = 1; i < picaso.directions; i++)
		{
			//Select which direction the agent will go to
			next = picaso.choice();
			//Check that chosen pixel hasnt been visited
			while (path.at(next).visited == true)
			{
				next = picaso.choice();
			}
			path.at(next).visited = true;
			//Calc Distance Travelled and Reward
			distFromOrigin = calcDist(path.at(0), path.at(next));
			distFromLast = calcDist(path.at(curr), path.at(next));
			totalDist += distFromLast;
			//cout << path.at(next).x << " " << path.at(next).y << " " << totalDist << endl;
			picaso.reward = 100.00 - distFromOrigin;
			curr = next;
			//Store q-value earned for going in that direction and repeat till all spaces visited
			picaso.update();
			picaso.curr = path.at(curr);
			currHist.push_back(picaso.curr);
		}
		picaso.distanceTravelled = totalDist;
		cout << generation << " " << totalDist << endl;
		simulation << generation << " " << totalDist << "\n";
		picaso.program = currHist;
		trials.push_back(picaso);
		//Reset Visited
		visitReset(path, picaso.directions);
		visitReset(picaso.q, picaso.directions);
		currHist.clear();
	}
	simulation.close();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//End Program
	cout << "whatup" << endl;
	system("PAUSE");

    return 0;
}

//Main Functions
double calcDist(pixel &current, pixel &next)
{
	//Pythagorian Theorem
	int a1 = current.x;
	int a2 = next.x;
	int a = (a1 - a2)*(a1 - a2);

	int b1 = current.y;
	int b2 = next.y;
	int b = (b1 - b2)*(b1 - b2);
	int ab = a + b;
	double c = sqrt(ab);
	return c;
}

void visitReset(vector<pixel> &pixels, int choices)
{
	for (int i = 0; i < choices; i++)
	{
		pixels.at(i).visited = false;
	}

}

