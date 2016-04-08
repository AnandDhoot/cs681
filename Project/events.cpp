#include <vector> 
#include <list> 
#include <iostream>
#include <iomanip>

#include "client.cpp"
#define NUM_CLIENTS 5
#define NUM_JOBS 50
#define CLIENT_BUFFER 5
#define SERVER_BUFFER 25
using namespace std;

class event;

double currTime = 0;
list<event*> eventList;
extern vector<jobs> jobBuffer;
extern default_random_engine generator;
extern vector<Client*> requestBuffer;		// Server request buffer
extern exponential_distribution<double> jobDuration;
extern exponential_distribution<double> jobSlack;
extern exponential_distribution<double> jobArr;
extern int jobsCompleted;
extern double deadlineSlack;
extern double cpuWaste,cpuIdle;
int jobsCreated=0;
template <typename T> bool PComp(const T * const & a, const T * const & b)
{
   return *a < *b;
}

class event{
public:
	double eventServiceTime; 	// timestamp
	virtual void handle() {}	

	bool operator<(const event& right) const 
	{ return eventServiceTime < right.eventServiceTime; }
};
class serverInterrupt : public event {
public:
	serverInterrupt(double time){
		eventServiceTime=time;

	}
	void handle()
	{
		currTime = eventServiceTime;
		// cerr << fixed << setprecision(2) << currTime << " Serving serverInterrupt" << endl;
		//some algo 
		eventList.push_back(new serverInterrupt(eventServiceTime + 0.1));
		eventList.sort(PComp<event>);

		//cout<<"serverInterrupt "<<jobBuffer.size()<<" "<<requestBuffer.size()<<endl;
		while(requestBuffer[0]->buffer.size()>=CLIENT_BUFFER && requestBuffer.size()!=0){
			requestBuffer[0]->outReq--;
			requestBuffer.erase(requestBuffer.begin());
		}
		if(jobBuffer.size()==0||requestBuffer.size()==0)
			return;

		cout << fixed << setprecision(2) << currTime << " JobToClient " << jobBuffer[0].id << " " << requestBuffer[0]->id << endl;
		requestBuffer[0]->buffer.push_back(jobBuffer[0]);
		requestBuffer[0]->outReq--;
		requestBuffer.erase(requestBuffer.begin());
		jobBuffer.erase(jobBuffer.begin());
	}

};
class timerInterrupt : public event {
public:
	Client *c;
	timerInterrupt(Client* cl,double tim){
		c=cl;
		eventServiceTime = tim;
	}
	void handle()
	{
		currTime = eventServiceTime;
		// cerr << fixed << setprecision(2) << currTime << " Serving timerInterrupt " 
	  //cout<< c->id << " " << c->outReq<< " "<<c->buffer.size() << " " << c->current->id<<" "<< endl;

		if(c->current->id!=-1){
			c->current->runTime += c->speed;

			if(c->current->runTime >= c->current->timeNeeded)
			{
				// Job complete 
				cout<< fixed << setprecision(2) << currTime << " JobDoneAtClient " << c->current->id << " " << c->id <<endl;
				c->removeJob(*(c->current));
				free(c->current);
				c->current=new jobs();
				jobsCompleted++;
				deadlineSlack += currTime-c->current->deadline;
				if(currTime-c->current->deadline<0)
					cpuWaste+=c->current->runTime;
			}
			else
			c->buffer.push_back(*(c->current));
		}
		for(int i=0;CLIENT_BUFFER - c->outReq - c->buffer.size() >0;i++){
			if(requestBuffer.size()>=SERVER_BUFFER)
				break;
			requestBuffer.push_back(c);
			c->outReq++;
				//cout<<"clientshizz "<<requestBuffer.size();
			}
		if(c->buffer.size()>0)
			*(c->current) = c->getNextJob();
		else
			cpuIdle+=c->speed;

		// add new timerInterrupt
		eventList.push_back(new timerInterrupt(c, eventServiceTime + 1.0));
		eventList.sort(PComp<event>);
	}

};
jobs getNewJob()
{
	double jobdur = jobDuration(generator);
	double deadline = currTime + jobdur + jobSlack(generator);
	jobs j(jobdur, deadline);

	return j;
}

class jobArrival : public event {
public:
	jobs job;
	jobArrival(jobs x, double time){
		job = x;
		eventServiceTime = time;
	}
	void handle ()
	{
		currTime = eventServiceTime;
		// cerr << fixed << setprecision(2) << currTime << " Serving jobArrival " << job.id << endl;
		// add job to server buffer
		if(jobBuffer.size()<NUM_JOBS){
			jobsCreated++;
			job.spawnTime = currTime;
			jobBuffer.push_back(job);
			cout << fixed << setprecision(2) << currTime << " NewJobAtServer " << job.id << " " << job.timeNeeded 
					<< " " << job.deadline << " " << job.spawnTime << endl;
		}
			eventList.push_back(new jobArrival(getNewJob(), currTime + jobArr(generator)));
			eventList.sort(PComp<event>);
		// enqueue next job arrival event after calling expon
		// cerr << "End\n";
	}

};