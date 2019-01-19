// DeltaBoatSimulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <assert.h>
#include <time.h>
#include <random>
#include "LY_NN.h"
#include "AN_EA.h"

using namespace std;

#define timestep 0.1

//Class ID
//Boat
class boat
{
public:

	//X,Y grid bounds and Set Goal and Boat start: initWater
	int boundMinX;
	int boundMinY;
	int boundMaxX;
	int boundMaxY;
	double goalMinX;
	double goalMinY;
	double goalMaxX;
	double goalMaxY;

	void initWater();


	//int xloc, yloc, u range[-15, 15] degrees per second
	//Starting Position: initBoat

	double boatStartX;
	double boatStartY;
	double v;
	double X;
	double Y;
	double T;
	double Theta;
	double Omega;
	double u;
	double uMax;
	double uMin;
	double dt;
	void initBoat();

	//Motion Simulation
	void calcX();
	void calcY();
	void calcTheta();
	void calcOmega();
	void simulate();

};


//Class Functions
//Boat
	//initiate water
void boat::initWater()
{
	boundMinX = 1;
	boundMinY = 1;
	boundMaxX = 80;
	boundMaxY = 80;
	goalMinX = 45.0;
	goalMinY = 45.0;
	goalMaxX = 65.0;
	goalMaxY = 65.0;

}

//initBoat
void boat::initBoat()
{
	//Motion
	//int v = 3.0, dt=0.2, T=5.0, u range[-15, 15]degrees per second

	boatStartX = rand()% boundMaxX;
	boatStartY = rand()% boundMaxY;
	//If initiated witin goal, reinitiate
	while ((boatStartX >= goalMinX) && (boatStartX <= goalMaxX)){
		boatStartX = rand() % boundMaxX;
	}
	while ((boatStartY >= goalMinY) && (boatStartY <= goalMaxY)) {
		boatStartY = rand() % boundMaxY;
	}

	X = boatStartX;
	Y = boatStartY;
	v = 3.0;
	dt = 0.2;
	T = 5.0;
	Theta = 0.0;
	Omega = 0.0;
	u = 0.0;
	uMax = 15.0;
	uMin = -15.0;

	cout << "Start at " << X << "," << Y << endl;
}

void boat::calcX()
{
	//x(t + 1) = x(t) + v  sin(theta(t))dt
	double Xt = X;
	double sinCalc = sin(Theta);
	double XtPlus = Xt + (v*sinCalc*dt);
	cout << X << "\t" << XtPlus << endl;
	//check for calculation
	//assert(XtPlus == X);
	X = XtPlus;
}
void boat::calcY()
{
	//y(t + 1) = y(t) + v  cos(theta(t))dt
	double Yt = Y;
	double cosCalc = cos(Theta);
	double YtPlus = Yt + v*cosCalc*dt;
	//check for calculation
	//assert(YtPlus != Y);
	Y = YtPlus;
}
void boat::calcTheta()
{
	//theta(t + 1) = theta(t) + omega(t)dt
	double ThetaT = Theta;
	double ThetaTPlus = ThetaT + Omega*dt;
	//check for calculation
	//assert(ThetaTPlus != Theta);
	Theta = ThetaTPlus;
}
void boat::calcOmega()
{
	//omega(t+1) = omega(t)+(u-omega(t))dt/T
	double OmegaT = Omega;
	double OmegaTPlus = OmegaT + (u - OmegaT)*dt / T;
	//check for calculation
	//assert(OmegaTPlus != Omega);
	Omega = OmegaTPlus;
}
void boat::simulate()
{
	calcX();
	calcY();
	calcTheta();
	calcOmega();
}

//Main
int main()
{

	double reward;
	//initialize simulation
	int MAX_GENERATIONS = 150;
	int pop_size = 30;
	srand(time(NULL));
	//initialize boat
	boat titanic;
	titanic.initWater();
	titanic.initBoat();
	//initialize NN
	neural_network NN;
	NN.setup(3, 9, 1);

	//Inputs: X,Y, Theta
	NN.set_in_min_max(titanic.boundMinX, titanic.boundMaxX); // X
	NN.set_in_min_max(titanic.boundMinY, titanic.boundMaxY); // Y
	NN.set_in_min_max(0, 360); // Theta
	//Outputs: u
	NN.set_out_min_max(titanic.uMin, titanic.uMax);//u

	//Initialize population
	vector<policy> population;
	for (int i = 0; i < pop_size; i++)
	{
		policy A;
		A.initialize(NN.get_number_of_weights());
		population.push_back(A);
	}
	assert(population.size() == pop_size);

	//Evaluate: Fitness based on rewards and penalties taken to reach goal
	for (int generation = 0; generation < MAX_GENERATIONS; generation++)
	{
		for (int sim = 0; sim < pop_size; sim++)
		{
		//Simulate dynamic response of boat
			NN.set_weights(population.at(sim).weights, true);
			int tstep = 0;
			reward = 1000;
			//Run Simulation until Out of Bounds or In Goal
			while ((titanic.X < titanic.goalMinX) || (titanic.X > titanic.goalMaxX) || (titanic.Y < titanic.goalMinY) || (titanic.Y > titanic.goalMaxY))
			{

				//State Variable
				vector <double> state;
				state.push_back(titanic.X);
				state.push_back(titanic.Y);
				state.push_back(titanic.Theta);
				NN.set_vector_input(state);

				//NN Compute 
				NN.execute();
				titanic.u = NN.get_output(0);
				cout << "Calculated u = " << titanic.u << endl;
					//Simulate timestep
					titanic.simulate();
					tstep++;
					reward = reward - (100*tstep);

					population.at(sim).ts = tstep;
						 
					//Check if the boundaries are crossed

					if (titanic.X > titanic.boundMaxX)
					{
						cout << "Crossed X Max" << endl;
						reward = reward - (100*tstep);
						cout << "landed at " << titanic.X << ", " << titanic.Y << ", gaining " << reward << " points" << endl;
						population.at(sim).fitness = reward;
						break;
					}
					else if (titanic.X < titanic.boundMinX)
					{
						cout << "Crossed X Min" << endl;
						reward = reward - (100 * tstep);
						cout << "landed at " << titanic.X << ", " << titanic.Y << ", gaining " << reward << " points" << endl;
						population.at(sim).fitness = reward;
						break;
					}
					else if (titanic.Y > titanic.boundMaxY)
					{
						cout << "Crossed Y Max" << endl;
						reward = reward - (100 * tstep);
						cout << "landed at " << titanic.X << ", " << titanic.Y << ", gaining " << reward << " points" << endl;
						population.at(sim).fitness = reward;
						break;
					}
					else if (titanic.Y < titanic.boundMinY)
					{
						cout << "Crossed Y Min" << endl;
						reward = reward - (100 * tstep);
						cout << "landed at " << titanic.X << ", " << titanic.Y << ", gaining " << reward << " points" << endl;
						population.at(sim).fitness = reward;
						break;

					}
					//Check if the sumulation times out
					else if (tstep > 1000)
					{
						cout << "Timed Out" << endl;
						reward = reward - (100 * tstep);
						cout << "landed at " << titanic.X << ", " << titanic.Y << ", gaining " << reward << " points" << endl;
						population.at(sim).fitness = reward;
						break;
					}
					//Reward for every time the boat is still in play
					else
					{
						reward = reward + (100/tstep);
						cout << "landed at " << titanic.X << ", " << titanic.Y << ", gaining " << reward << " points" << endl;
						population.at(sim).fitness = reward;
					}
			}

			
		}
		//Downselect
		population = EA_downselect(population, pop_size);
		//Repopulate
		population = EA_replicate(population, pop_size);

		cout << "Generation " << generation << endl;
	}

	sort(population.begin(), population.end(), less_than_key());
	for (int i = 0; i < pop_size; i++)
	{
		cout << i << "\t" << population.at(i).fitness << "\t" << population.at(i).ts << endl;
	}

	//End Program
	cout << "whatup" << endl;
	system("PAUSE");
	return 0;
}

//Funtions