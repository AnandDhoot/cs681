#include <vector> 
#include <list> 
#include <iostream>

#include "client.cpp"

using namespace std;

int currTime = 1;
list<event*> des;

class event{
public:
	int eventServiceTime; 	// timestamp
	virtual void handle(vector<event*> eventList) = 0;	
};

class timerInterrupt : public event {
public:
	Client *c;

	void handle()
	{
		currTime = eventServiceTime;

		c->current->runTime += c->speed;
		if(c->current->runTime >= c->current->timeNeeded)
		{
			// Job complete 
			c->removeJob(*(c->current));
			serverBuffer.push_back(c);
		}
		*(c->current) = c->getNextJob();

		// add new timerInterrupt
		timerInterrupt ti;
		ti.eventServiceTime = eventServiceTime + 1;
		ti.c = c;
		des.push_back(&ti);
	}

};

class jobArrival : public event {

	jobs job;

	void handle (vector<event*> eventList)
	{
		// add job to server buffer
		// enqueue next job arrival event after calling expon

	}

};