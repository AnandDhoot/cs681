#include <vector>
#include <list>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <random>

using namespace std;

static int jobCount = 0;

class jobs {
public:
	int id;
	double timeNeeded;		// Difference
	double runTime;			// Difference
	double deadline;		// Absolute
	double spawnTime;		// Absolute, the time when job was added to server buffer
	bool jobDead;

	jobs(double tn, double dl)
	{
		id = jobCount ++;
		timeNeeded = tn;
		deadline = dl;
		spawnTime = -1; // Set later in interrupt
		runTime = 0;
		jobDead = false;
	}

	jobs()
	{
		id = -1;
		timeNeeded = -1;
		runTime = -1;
		deadline = -1;
		spawnTime = -1;
		jobDead = true;
	}

	bool operator==(const jobs& b)
	{
		return this->id == b.id;
	}
		bool operator<(const jobs& b)
	{
		return this->deadline< b.deadline;
	}
};

