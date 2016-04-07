#include <vector> 
#include <list> 
#include <iostream>

#include "events.cpp"

using namespace std;

// --------------- <UTILS> ----------------
// Exponential random number genrator receipe
default_random_engine generator;
exponential_distribution<double> jobDuration;
exponential_distribution<double> jobSlack;
exponential_distribution<double> jobArr;

// To reset the exponential paramter
template<typename T>
void set_new_lambda(std::exponential_distribution<T> *exp_dis, T val)
{
    typename std::exponential_distribution<T>::param_type new_lambda(val);
    exp_dis->param(new_lambda);
}
// --------------- </UTILS> ---------------


jobs getNewJob()
{
	int jobdur = (int) jobDuration(generator);
	int deadline = currTime + jobdur + (int) jobSlack(generator);
	jobs j(jobdur, deadline, currTime);

	cout << "New job: " << j.id << " " << jobdur << " " << deadline << " " << currTime << endl;
	return j;
}

vector<jobs> serverBuffer;
vector<Client*> requestBuffer;
Client *client[NUM_CLIENTS];
void init()
{
	//initilize eventlist
	jobArrival x(jobArr(generator), getNewJob());
	for(int i=0;i<NUM_CLIENTS;i++){
		eventList.push_back(new timerInterrupt(client[i],1));
	}

}
int main()
{
	// init all the stuff
	set_new_lambda(&jobDuration, 0.5);
	set_new_lambda(&jobSlack, 0.25);
	set_new_lambda(&jobArr, 10.0);
	set_new_lambda(&clientSpeed, 0.1);

	Client *c[5];
	for(int i=0; i<5; i++)
	{
		c[i] = new fifoClient((int) clientSpeed(generator));
	}

	for(int i=0; i<10; i++)
	{
		getNewJob();
	}



}
