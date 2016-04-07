#include <vector> 
#include <list> 
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <random>

using namespace std;

static int jobCount = 0;

class jobs{
public:
	int id;
	int timeNeeded;		// Difference
	int runTime;		// Difference
	int deadline;		// Absolute
	int spawnTime;		// Absolute
	bool jobDead;

	jobs(int tn, int dl, int st)
	{
		id = jobCount ++;
		timeNeeded = tn;
		deadline = dl;
		spawnTime = st;
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
};

