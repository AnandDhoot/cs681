#include <vector> 
#include <list> 
#include <iostream>

#include "client.cpp"
#define NUM_CLIENTS 5
#define NUM_JOBS 5
#define CLIENT_BUFFER 5
using namespace std;

class event;

int currTime = 1;
list<event*> eventList;
extern vector<jobs> jobBuffer;
extern default_random_engine generator;
extern vector<Client*> requestBuffer;
extern exponential_distribution<double> jobDuration;
extern exponential_distribution<double> jobSlack;
extern exponential_distribution<double> jobArr;
class event{
public:
	int eventServiceTime; 	// timestamp
	virtual void handle() {}	
};
class serverInterrupt : public event {
public:
	serverInterrupt(int time){
		eventServiceTime=time;

	}
	void handle()
	{
		//some algo 
		eventList.push_back(new serverInterrupt(eventServiceTime + 2));
		if(jobBuffer.size()==0||requestBuffer.size()==0)
			return;
		requestBuffer[0]->buffer.push_back(jobBuffer[0]);
		requestBuffer[0]->outReq--;
		requestBuffer.erase(requestBuffer.begin());
		jobBuffer.erase(jobBuffer.begin());

	}

};
class timerInterrupt : public event {
public:
	Client *c;
	timerInterrupt(Client* cl,int tim){
		c=cl;
		eventServiceTime = tim;
	}
	void handle()
	{
		// cerr << "Serving timerInterrupt" << endl;
		currTime = eventServiceTime;

		if(c->current->id!=-1){
					c->current->runTime += c->speed;
		if(c->current->runTime >= c->current->timeNeeded)
		{
			// Job complete 
			cout<<"Job Complete"<<c->current->id<<endl;
			c->removeJob(*(c->current));
		}
	}
		for(int i=0;CLIENT_BUFFER-c->outReq>0;i++){
				requestBuffer.push_back(c);
				c->outReq++;
			}
	  if(c->buffer.size()>0)
		*(c->current) = c->getNextJob();

		// add new timerInterrupt
		eventList.push_back(new timerInterrupt(c, eventServiceTime + 1));
	}

};
jobs getNewJob()
{
	int jobdur = (int) jobDuration(generator);
	int deadline = currTime + jobdur + (int) jobSlack(generator);
	jobs j(jobdur, deadline, currTime);

	cout << "New job: " << j.id << " " << jobdur << " " << deadline << " " << currTime << endl;
	return j;
}

class jobArrival : public event {
public:
	jobs job;
	jobArrival(int time,jobs x){
		job = x;
		eventServiceTime = time;
	}
	void handle ()
	{
		// cerr << "Serving jobArrival" << endl;
		// add job to server buffer
		if(jobBuffer.size()<NUM_JOBS){
			jobBuffer.push_back(job);
			eventList.push_back(new jobArrival( currTime+ jobArr(generator), getNewJob()));
		}
		// enqueue next job arrival event after calling expon
		// cerr << "End\n";
	}

};