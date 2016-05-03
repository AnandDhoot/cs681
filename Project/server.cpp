#include <vector>
#include <chrono>
#include <random>
#include <list>
#include <iostream>
#include <chrono>
#include <random>
#include "events.cpp"

using namespace std;

// --------------- <UTILS> ----------------
// Exponential random number genrator receipe
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);
exponential_distribution<double> jobDuration;
exponential_distribution<double> jobSlack;
exponential_distribution<double> jobArr;
int jobsCompleted = 0, jobsDropped = 0,jobsAccepted=0;
double deadlineSlack = 0;
double cpuWaste = 0, cpuIdle = 0, responseTimeCumm = 0, waitingTimeCumm = 0.0;

// To reset the exponential paramter
template<typename T>
void set_new_lambda(std::exponential_distribution<T> *exp_dis, T val)
{
	typename std::exponential_distribution<T>::param_type new_lambda(val);
	exp_dis->param(new_lambda);
}
// --------------- </UTILS> ---------------
vector<int> clientSpeeds;
vector<jobs> jobBuffer;
vector<Client*> requestBuffer;
Client **client;
void init()
{
	//initilize eventlist
	client = new Client*[NUM_FIFO_Clients + NUM_FIFORR_Clients + NUM_SDF_Clients + NUM_SJF_Clients];
	eventList.push_back(new jobArrival(getNewJob(), jobArr(generator)));
	eventList.push_back(new serverInterrupt(2));
	for (int i = 0; i < NUM_FIFO_Clients; i++) {
		// client[i] = new fifoClient(1 + clientSpeed(generator)); 	// 1 to ensure speed is never 0.
		client[i] = new fifoClient(clientSpeeds[i]); 	// 1 to ensure speed is never 0.
		eventList.push_back(new timerInterrupt(client[i], 1));
	}
	for (int i = 0; i < NUM_FIFORR_Clients; i++) {
		// client[i] = new fifoRRClient(1 + clientSpeed(generator)); 	// 1 to ensure speed is never 0.
		client[i] = new fifoRRClient(clientSpeeds[i]); 	// 1 to ensure speed is never 0.
		eventList.push_back(new timerInterrupt(client[i], 1));
	}
	for (int i = 0; i < NUM_SDF_Clients; i++) {
		// client[i] = new sdfClient(1 + clientSpeed(generator)); 	// 1 to ensure speed is never 0.
		client[i] = new sdfClient(clientSpeeds[i]); 	// 1 to ensure speed is never 0.
		eventList.push_back(new timerInterrupt(client[i], 1));
	}
	for (int i = 0; i < NUM_SJF_Clients; i++) {
		// client[i] = new sjfClient(1 + clientSpeed(generator)); 	// 1 to ensure speed is never 0.
		client[i] = new sjfClient(clientSpeeds[i]); 	// 1 to ensure speed is never 0.
		eventList.push_back(new timerInterrupt(client[i], 1));
	}
	eventList.sort(PComp<event>);

}
int simTime;
void parseParams() {
	ifstream fin;
	fin.open ("params.txt");

	string s;
	int x;
	fin >> s;
	fin >> x;
	NUM_FIFO_Clients = x;
	for(int i=0; i<NUM_FIFO_Clients; i++)
	{
		fin >> x;
		clientSpeeds.push_back(x);
	}
	fin >> s;
	fin >> x;
	NUM_FIFORR_Clients = x;
	for(int i=0; i<NUM_FIFORR_Clients; i++)
	{
		fin >> x;
		clientSpeeds.push_back(x);
	}
	fin >> s;
	fin >> x;
	NUM_SJF_Clients = x;
	for(int i=0; i<NUM_SJF_Clients; i++)
	{
		fin >> x;
		clientSpeeds.push_back(x);
	}
	fin >> s;
	fin >> x;
	NUM_SDF_Clients = x;
	for(int i=0; i<NUM_SDF_Clients; i++)
	{
		fin >> x;
		clientSpeeds.push_back(x);
	}
	fin >> s;
	fin >> x;
	NUM_JOBS = x;
	fin >> s;
	fin >> x;
	CLIENT_BUFFER = x;
	fin >> s;
	fin >> x;
	SERVER_BUFFER = x;
	fin >> s;
	fin >> x;
	serverSJF = x;
	fin >> s;
	fin >> x;
	serverSDF = x;
	fin >> s;
	fin >> x;
	serverFastClient = x;
	fin >> s;
	fin >> x;
	serverFairClient = x;
	double d;
	fin >> s;
	fin >> d;
	set_new_lambda(&jobDuration, d);
	fin >> s;
	fin >> d;
	set_new_lambda(&jobSlack, d);
	fin >> s;
	fin >> d;
	set_new_lambda(&jobArr, d);
	fin >> s;
	fin >> d;
	set_new_lambda(&clientSpeed, d);
	fin >> s;
	fin >> d;
	serverInterruptDelay = d;
	fin >> s;
	fin >> d;
	clientInterruptDelay = d;
	fin >> s;
	fin >> simTime;
}
int main()
{
	// init all the stuff
	fout.open ("trace.txt");
	parseParams();
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator (seed);

	init();
	while (eventList.front()->eventServiceTime < simTime) {
		eventList.front()->handle();
		eventList.pop_front();
	}
	cout << "# Jobs Completed : " << jobsCompleted << endl;
	cout << "# Jobs Dropped : " << jobsDropped << endl;
	cout << "# Jobs Entered Server : " << jobsAccepted << endl;	
	cout << "# Jobs Created : " << jobsCreated << endl;
	cout << "# Deadline Slack : " << deadlineSlack << endl;
	cout << "# CPU Cycles Wasted : " << cpuWaste << endl;
	cout << "# CPU Cycles Idle : " << cpuIdle << endl;
	cout << "# Total Time elapsed : " << currTime << endl;
	cout << "Avg Response Time: " << 1.0*responseTimeCumm/jobsCompleted << endl;
	cout << "Avg Waiting Time: " << 1.0*waitingTimeCumm/jobsCompleted << endl;
	return 1;
}
