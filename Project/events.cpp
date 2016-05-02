#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <math.h>
#include "client.cpp"
int NUM_FIFO_Clients, NUM_FIFORR_Clients, NUM_SJF_Clients, NUM_SDF_Clients;
int NUM_JOBS , CLIENT_BUFFER, SERVER_BUFFER;
double serverInterruptDelay, clientInterruptDelay;
bool serverSJF, serverSDF, serverFastClient, serverFairClient;
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
extern int jobsCompleted, jobsDropped,jobsAccepted;
extern double deadlineSlack;
extern double cpuWaste, cpuIdle;
int jobsCreated = 0;
template <typename T> bool PComp(const T * const & a, const T * const & b)
{
	return *a < *b;
}

class event {
public:
	double eventServiceTime; 	// timestamp
	virtual void handle() {}

	bool operator<(const event& right) const
	{ return eventServiceTime < right.eventServiceTime; }
};
bool fair( Client *a,  Client *b)
{
	return a->buffer.size() < b->buffer.size();
}
bool speed( Client *a,  Client *b)
{
	return *a < *b;
}
class serverInterrupt : public event {
public:
	serverInterrupt(double time) {
		eventServiceTime = time;

	}
	void handle()
	{
		currTime = eventServiceTime;
		// cerr << fixed << setprecision(2) << currTime << " Serving serverInterrupt" << endl;
		//some algo
		//purge dead jobs
		for (int i = 0; i < jobBuffer.size(); i++) {
			if (currTime > jobBuffer[i].deadline) {
				fout << fixed << setprecision(2) << currTime << " JobDroppedAtServer " <<jobBuffer[i].id << endl;
				jobBuffer.erase(jobBuffer.begin() + i);
				jobsDropped++;

			}
		}
		eventList.push_back(new serverInterrupt(eventServiceTime + serverInterruptDelay));
		eventList.sort(PComp<event>);
		//fastest server first
		if (serverFastClient)
			sort(requestBuffer.begin(), requestBuffer.end(), speed);
		//fair scheduling
		if (serverFairClient)
			stable_sort(requestBuffer.begin(), requestBuffer.end(), fair);
		//fout<<"serverInterrupt "<<jobBuffer.size()<<" "<<requestBuffer.size()<<endl;
		while (requestBuffer[0]->buffer.size() >= CLIENT_BUFFER && requestBuffer.size() != 0) {
			requestBuffer[0]->outReq--;
			requestBuffer.erase(requestBuffer.begin());
		}
		if (jobBuffer.size() == 0 || requestBuffer.size() == 0)
			return;

		fout << fixed << setprecision(2) << currTime << " JobToClient " << jobBuffer[0].id << " " << requestBuffer[0]->id << endl;
		requestBuffer[0]->buffer.push_back(jobBuffer[0]);
		requestBuffer[0]->outReq--;
		requestBuffer.erase(requestBuffer.begin());
		jobBuffer.erase(jobBuffer.begin());
	}

};
class timerInterrupt : public event {
public:
	Client *c;
	timerInterrupt(Client* cl, double tim) {
		c = cl;
		eventServiceTime = tim;
	}
	void handle()
	{
		currTime = eventServiceTime;
		// cerr << fixed << setprecision(2) << currTime << " Serving timerInterrupt "
		//fout<< c->id << " " << c->outReq<< " "<<c->buffer.size() << " " << c->current->id<<" "<< endl;
		double time = 1.0;
		if (c->current->id != -1) {
			c->current->runTime += time * c->speed;

			if (c->current->runTime >= c->current->timeNeeded && c->current->deadline > currTime)
			{
				// Job complete
				fout << fixed << setprecision(2) << currTime << " JobDoneAtClient " << c->current->id << " " << c->id << endl;
				jobsCompleted++;
				deadlineSlack += c->current->deadline - currTime ;
				c->removeJob(*(c->current));
				free(c->current);
				c->current = new jobs();
			}
			else if (c->current->deadline < currTime)
			{
				// Job Failed to complete before dedaline
				fout << fixed << setprecision(2) << currTime << " JobDroppedAtClient " << c->current->id << " " << c->id << endl;
				cpuWaste += c->current->runTime;
				c->removeJob(*(c->current));
				free(c->current);
				jobsDropped++;
				c->current = new jobs();
			}
			else {
				c->buffer.push_front(*(c->current));
				time = min(time, time * (c->current->timeNeeded - c->current->runTime) / c->speed);
			}
		}
		else
			cpuIdle += c->speed;

		for (int i = 0; CLIENT_BUFFER - c->outReq - c->buffer.size() > 0; i++) {
			if (requestBuffer.size() >= SERVER_BUFFER)
				break;
			requestBuffer.push_back(c);
			c->outReq++;
		}
		if (c->buffer.size() > 0)
			*(c->current) = c->getNextJob();

		// add new timerInterrupt

		eventList.push_back(new timerInterrupt(c, eventServiceTime + time));
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
	jobArrival(jobs x, double time) {
		job = x;
		eventServiceTime = time;
	}
	void handle ()
	{
		currTime = eventServiceTime;
		// cerr << fixed << setprecision(2) << currTime << " Serving jobArrival " << job.id << endl;
		// add job to server buffer
		if (jobBuffer.size() < NUM_JOBS) {
			job.spawnTime = currTime;
			jobBuffer.push_back(job);
			// SJF
			if (serverSJF)
				sort(jobBuffer.begin(), jobBuffer.end(), sjfComp);
			//shortest deadline first
			if (serverSDF)
				sort(jobBuffer.begin(), jobBuffer.end());
			fout << fixed << setprecision(2) << currTime << " NewJobAtServer " << job.id << " " << job.timeNeeded
			     << " " << job.deadline << " " << job.spawnTime << endl;
			     jobsAccepted++;
		}
		jobsCreated++;
		eventList.push_back(new jobArrival(getNewJob(), currTime + jobArr(generator)));
		eventList.sort(PComp<event>);
		// enqueue next job arrival event after calling expon
		// cerr << "End\n";
	}

};