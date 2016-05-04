This directory contains
1) The project proposal - 'des_proposal_Anchit_Anand.pdf'
2) The presentation - 'Presentation_Part1.pdf'
3) The code, in files appropriately named. 
	To execute the code, run 'make' followed by './a.out > tr.txt'
	This generates the trace in the file 'tr.txt'. 
	The trace has the follwing general format :- 'timestamp eventType description', 
		with the description varying with the type of the event. 
4) Parameters are in params.txt. 
	To have 5 FIFO clients, each with speed equal to 1, corresponding line in params.txt would be: 
			NUM_FIFO_Clients 5 1 1 1 1 1. 
	In general
			<type of client> <num> <speed of individual clients>
	To make server use the SJF algorithm, set 'serverSJF 1' in params.txt
	Similarly for SDF, Fair Client and Fast Client. 
	All other parameters are numerical values. 
	Lambdas correspond to the rate of the corresponding exponential distributions. 