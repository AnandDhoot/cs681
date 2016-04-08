#include <vector> 
#include <list> 
#include <iostream>

#include "jobs.cpp"

using namespace std;

exponential_distribution<double> clientSpeed;

static int clientCount = 0;
class Client
{
public:
	int id;
	int outReq=0;
	int speed;
	list<jobs> buffer;
	jobs *current;

	Client(int s)
	{
		id = clientCount ++;
		speed = s;
		current = new jobs();
	}

	virtual jobs getNextJob() = 0;	
	void removeJob(jobs job){
		buffer.remove(job);
	}	

};

class fifoRRClient : public Client{
public:
	fifoRRClient(int s) : Client(s)
	{
		cout << "NewClient " << "fifoRR " << id << " " << speed << endl;
	}
	jobs getNextJob()
	{
		if(buffer.size()>0){
		jobs temp = buffer.front();
		buffer.pop_front();
		buffer.push_back(temp);
		return temp;}

	}
};
class fifoClient : public Client{
public:
	fifoClient(int s) : Client(s)
	{
		cout << "NewClient " << "fifo " << id << " " << speed << endl;
	}
	jobs getNextJob()
	{
		jobs temp = buffer.front();
		return temp;
	}	
};