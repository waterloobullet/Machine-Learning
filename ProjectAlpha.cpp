// ProjectAlpha.cpp : Defines the entry point for the console application.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////Project Alpha///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////Created by Andy Narvaez Nunez////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;


//Identify Class

//Slot Machine
class slotMachine 
{
	//assign constant value for possible award per pull
	int idealReward; //points
	public:
		slotMachine(int maximum) :idealReward(maximum) {}
		//calculate reward from pull
	double pullReward()
	{
		double reward;
		double armWinFactor = (double)rand() / RAND_MAX; //Value of Win
		double armPullFactor = (double)rand()/ RAND_MAX; //Value pulled 

		//if Value Pulled is above Win Factor, reward randomized factor of award
		if (armPullFactor > armWinFactor)
			reward = armPullFactor / armWinFactor*idealReward;
		//else subtract randomized factor of award
		else
			reward = (armPullFactor / armWinFactor*idealReward)*-1;
		return reward;
		//likeliness of winning is 100% randomized this way
	}
	

		
}



};

//Agent
class agent 
{
public:
	//initialize vector for agent to store its arms
		// prime vector to best value being element 0
	vector<double> ag;
	void initialize();


	//Initialize values for alpha,  and epsilon to
	//balance exploration and exploitation
		//iterations
	int bestArm = -1;
	int pulls = 30;
	int reward = 0;
		//choice
	int slotMachines = 3;
	double alpha = 0.1;
	double epsilon = 0.1;
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

void agent::initialize()
{
	int a = 0;
	for ( a; a < slotMachines; i++ )
	{
		ag.push_back(0); //initializing to no reward, allowing an ARMS race,literally
	}
}

int agent::choice()
{
	int b = 0;
	for (b; b < pulls; b++)
	{
		double c = (double)rand() / RAND_MAX;
		if (c < epsilon)
			action = randomAction();
		else
			action = rewardingAction();
	}
	return action;
}

int agent::rewardingAction()
{
	//find most rewarding arm
	double bestOne = -1000000;
	int d = 0;
	for (d; d < slotMachines < d++)
	{
		if (ag.at(d) > bestOne)
			bestArm = d;
			bestOne = ag.at(d)
	}
	return bestArm;
}

int agent::randomAction()
{
	//select random arm
	int d;
	d = rand() % machines;
	return d;
}
void agent::update()
{

	ag.at(action) = ag.at(action)+ alpha*(reward-ag.at(action))
}
int main()
{
	srand(time(NULL));
	//create the specified number of slot machine arms
	int e = 0;
	slotMachine armOne(100), armTwo(200), armThree(300);

	agent smith;

	vector<slotMachine> newIGTModel;

	newIGTModel.push_back(armOne);
	newIGTModel.push_back(armTwo);
	newIGTModel.push_back(armThree);

	smith.initialize();
	for (e; e < smith.pulls; e++)
	{
		int armPulled = smith.choice();
		reward = newIGTModel.at(armPulled).pullReward();
		smith.ag[e] = reward;
	}


	//create agent

	//initialize vectors of slot machine arms

	//Iterate through choices and make choices based on exploration and exploitation

	//identify action to user

	//when completed all iterations do two tests
		//Test A: Average of Pulls Converges to the arms Average reward
			//function: testA

		//Test B: Agent selected most profitable arm
			//function: testB
	//save vector to document for analysis
	cout << "whatup";
	system("PAUSE");
    return 0;
}

