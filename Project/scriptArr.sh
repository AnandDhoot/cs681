#!/bin/bash

sed -i 's/NUM_FIFO_Clients *$/NUM_FIFO_Clients 0/' params.txt
sed -i 's/NUM_FIFORR_Clients *$/NUM_FIFORR_Clients 0/' params.txt
sed -i 's/NUM_SJF_Clients *$/NUM_SJF_Clients 0/' params.txt
sed -i 's/NUM_SDF_Clients *$/NUM_SDF_Clients 0/' params.txt
echo "FIFO" > out1.txt
sed -i 's/NUM_FIFO_Clients 0/NUM_FIFO_Clients 5 1 1 1 1 1/' params.txt 
for i in {0.1,0.5,1,2,5,10}; do
echo "Arrival Rate: $i" >> out1.txt
sed -i "s/jobArrivalRate.*/jobArrivalRate $i/" params.txt 
python summarize.py >> out1.txt
echo "" >> out1.txt
echo "" >> out1.txt
done;