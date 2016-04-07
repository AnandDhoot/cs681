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

vector<jobs> jobBuffer;
vector<Client*> requestBuffer;
Client *client[NUM_CLIENTS];
void init()
{
	//initilize eventlist
	eventList.push_back(new jobArrival( jobArr(generator), getNewJob()));
	eventList.push_back(new serverInterrupt(2));
	for(int i=0;i<NUM_CLIENTS;i++){
		client[i]=new fifoRRClient(1);
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
	init();
	int enthu=100;
	while(enthu){
		eventList.front()->handle();
		eventList.pop_front();
		enthu--;
	}

	return 1;
}
