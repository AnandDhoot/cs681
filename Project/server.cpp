#include <vector> 
#include <list> 
#include <iostream>
#include <chrono> 
#include <random>
#include "events.cpp"

using namespace std;

// --------------- <UTILS> ----------------
// Exponential random number genrator receipe
default_random_engine generator;
exponential_distribution<double> jobDuration;
exponential_distribution<double> jobSlack;
exponential_distribution<double> jobArr;
int jobsCompleted=0;
double deadlineSlack=0;
double cpuWaste=0,cpuIdle=0;

// To reset the exponential paramter
template<typename T>
void set_new_lambda(std::exponential_distribution<T> *exp_dis, T val)
{
    typename std::exponential_distribution<T>::param_type new_lambda(val);
    exp_dis->param(new_lambda);
}
// --------------- </UTILS> ---------------

vector<jobs> jobBuffer;
vector<Client*> requestBuffer;
Client *client[NUM_CLIENTS];
void init()
{
	//initilize eventlist
	eventList.push_back(new jobArrival(getNewJob(), jobArr(generator)));
	eventList.push_back(new serverInterrupt(2));
	for(int i=0;i<NUM_CLIENTS;i++){
		client[i]=new fifoRRClient(1 + clientSpeed(generator)); 	// 1 to ensure speed is never 0. 
		eventList.push_back(new timerInterrupt(client[i],1));
	}
	eventList.sort(PComp<event>);

}
int main()
{
	// init all the stuff
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);
	set_new_lambda(&jobDuration, 0.5);
	set_new_lambda(&jobSlack, 0.25);
	set_new_lambda(&jobArr, 10.0);
	set_new_lambda(&clientSpeed, 0.1);

	init();
	int enthu=10000;
	while(enthu){
		eventList.front()->handle();
		eventList.pop_front();
		enthu--;
	}
	cout<<"# Jobs Completed : "<<jobsCompleted<<endl;
	cout<<"# Deadline Slack : "<<deadlineSlack<<endl;
	cout<<"# CPU Cycles Wasted : "<<cpuWaste<<endl;
	cout<<"# CPU Cycles Idle : "<<cpuIdle<<endl;
	cout<<"# Total Time elapsed : "<<currTime<<endl;
	return 1;
}
