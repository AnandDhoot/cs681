
class jobs{
public:
	int id;
	int timeNeeded;
	int deadline;
	int spawnTime;
	int runTime;
	bool jobDead;
};
class Client{

public:

	int id;
	int speed;
	list<jobs> buffer;
	jobs current;
	virtual jobs getNextJob() = 0;	
	void removeJob(jobs job){
		buffer.remove(job);
	}	

};
class fifoRRClient : public Client{

public:

	jobs getNextJob()
	{
		jobs temp = buffer.front();
		buffer.pop_front();
		buffer.push_back(temp);
		return temp;

	}	

};
class fifoClient : public Client{

public:

	jobs getNextJob()
	{
		jobs temp = buffer.front();
		return temp;
	}	

};
class event{
public:
	int timeStamp;
	virtual void handle (vector<event> eventList) = 0;	
};

class timerInterrupt : public event {
public:
	Client *cpu;
	void handle (vector<event> eventList)
	{
		
		cpu->current.runTime+=cpu->speed;
		if(cpu->current.runTim>=cpu->current.timeNeeded)
		{
			//print trace
			cpu->removeJob(cpu->current);
			//request server for new job
		}
		cpu->current=cpu->getNextJob();
		//add new timerInterrupt
		timerInterrupt new1;
		new1.timeStamp=timeStamp+1;
		new1.cpu=cpu;
		eventList.push_back(new1);
	}

};
class jobArrival : public event {

	jobs job;

	void handle (vector<event> eventList)
	{
		// add job to server buffer
		//enqueue next job arrival event after calling expon

	}

};