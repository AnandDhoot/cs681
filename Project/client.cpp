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
	int outReq = 0;
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
	void removeJob(jobs job) {
		buffer.remove(job);
	}
	bool operator<(const Client& b)
	{
		return this->speed< b.speed;
	}

};

class fifoRRClient : public Client {
public:
	fifoRRClient(int s) : Client(s)
	{
		cout << "NewClient " << "fifoRR " << id << " " << speed << endl;
	}
	jobs getNextJob()
	{
		if (buffer.size() > 0) {
			jobs temp = buffer.front();
			buffer.pop_front();
			buffer.push_back(temp);
			temp = buffer.front();
			buffer.pop_front();
			return temp;
		}

	}
};
class fifoClient : public Client {
public:
	fifoClient(int s) : Client(s)
	{
		cout << "NewClient " << "fifo " << id << " " << speed << endl;
	}
	jobs getNextJob()
	{
		jobs temp = buffer.front();
		buffer.pop_front();
		return temp;
	}
};
class sdfClient : public Client {
public:
	sdfClient(int s) : Client(s)
	{
		cout << "NewClient " << "Shortest Deadline First " << id << " " << speed << endl;
	}
	jobs getNextJob()
	{
		buffer.sort();
		jobs temp = buffer.front();
		buffer.pop_front();
		return temp;
	}
};
bool sjfComp( jobs &a,  jobs &b)
{
	return a.timeNeeded-a.runTime < b.timeNeeded-b.runTime;
}
class sjfClient : public Client {
public:
	sjfClient(int s) : Client(s)
	{
		cout << "NewClient " << "Shortest Job First " << id << " " << speed << endl;
	}
	jobs getNextJob()
	{
		buffer.sort(sjfComp);
		jobs temp = buffer.front();
		buffer.pop_front();
		return temp;
	}
};