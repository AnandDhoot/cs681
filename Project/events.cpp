#include <vector> 
#include <list> 
#include <iostream>

#include "client.cpp"

using namespace std;

int currTime = 1;

class event{
public:
	int timeStamp;
	virtual void handle(vector<event*> eventList) = 0;	
};
class serverInterrupt : public event {
public:
	Client *c;
	timerInterrupt(Client* cl,int time){
		c=cl;
		timeStamp=time;

	}
	void handle(vector<event*> eventList)
	{
		c->current->runTime += c->speed;
		if(c->current->runTime >= c->current->timeNeeded)
		{
			//print trace
			c->removeJob(*(c->current));
			//request server for new job
		}
		*(c->current) = c->getNextJob();
		//add new timerInterrupt
		timerInterrupt new1;
		new1.timeStamp=timeStamp+1;
		new1.c = c;
		eventList.push_back(&new1);
	}

};
class timerInterrupt : public event {
public:
	Client *c;
	timerInterrupt(Client* cl,int time){
		c=cl;
		timeStamp=time;

	}
	void handle(vector<event*> eventList)
	{
		c->current->runTime += c->speed;
		if(c->current->runTime >= c->current->timeNeeded)
		{
			//print trace
			c->removeJob(*(c->current));
			//request server for new job
		}
		*(c->current) = c->getNextJob();
		//add new timerInterrupt
		timerInterrupt new1;
		new1.timeStamp=timeStamp+1;
		new1.c = c;
		eventList.push_back(&new1);
	}

};

class jobArrival : public event {

	jobs job;
	jobArrival(int time,jobs x){
		job=x;
		timeStamp=time;
	}
	void handle (vector<event*> eventList)
	{
		// add job to server buffer
		//enqueue next job arrival event after calling expon

	}

};