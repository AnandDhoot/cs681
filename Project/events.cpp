#include <vector> 
#include <list> 
#include <iostream>

#include "client.cpp"
#define NUM_CLIENTS 5

using namespace std;

class event;

int currTime = 1;
list<event*> eventList;
extern vector<jobs> serverBuffer;
extern vector<Client*> requestBuffer;

class event{
public:
	int eventServiceTime; 	// timestamp
	virtual void handle(vector<event*> eventList) = 0;	
};
class serverInterrupt : public event {
public:
	Client *c;
	serverInterrupt(Client* cl,int time){
		c=cl;
		eventServiceTime=time;

	}
	void handle(vector<event*> eventList)
	{
	// 	currTime = eventServiceTime;

	// 	c->current->runTime += c->speed;
	// 	if(c->current->runTime >= c->current->timeNeeded)
	// 	{
	// 		// Job complete 
	// 		c->removeJob(*(c->current));
	// 		serverBuffer.push_back(c);
	// 	}
	// 	*(c->current) = c->getNextJob();

	// 	// add new timerInterrupt
	// 	eventList.push_back(new serverInterrupt(c, eventServiceTime + 1));
	}

};
class timerInterrupt : public event {
public:
	Client *c;
	timerInterrupt(Client* cl,int tim){
		c=cl;
		eventServiceTime = tim;
	}
	void handle(vector<event*> eventList)
	{
		currTime = eventServiceTime;

		c->current->runTime += c->speed;
		if(c->current->runTime >= c->current->timeNeeded)
		{
			// Job complete 
			c->removeJob(*(c->current));
			requestBuffer.push_back(c);
		}
		*(c->current) = c->getNextJob();

		// add new timerInterrupt
		timerInterrupt ti(c, eventServiceTime + 1);
		eventList.push_back(&ti);
	}

};

class jobArrival : public event {
public:
	jobs job;
	jobArrival(int time,jobs x){
		job = x;
		eventServiceTime = time;
	}
	void handle (vector<event*> eventList)
	{
		// add job to server buffer
		// enqueue next job arrival event after calling expon

	}

};