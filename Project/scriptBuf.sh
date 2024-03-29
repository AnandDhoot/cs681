#!/bin/bash

sed -i 's/NUM_FIFO_Clients.*$/NUM_FIFO_Clients 0/' params.txt
sed -i 's/NUM_FIFORR_Clients.*$/NUM_FIFORR_Clients 0/' params.txt
sed -i 's/NUM_SJF_Clients.*$/NUM_SJF_Clients 0/' params.txt
sed -i 's/NUM_SDF_Clients.*$/NUM_SDF_Clients 0/' params.txt
sed -i "s/NUM_$1_Clients 0/NUM_$1_Clients 5 1 1 1 1 1/" params.txt 
echo -n "" >out1.txt
for i in {1,2,5,7,10,20}; do
echo -n  "$1 " >> out1.txt
echo -n " $i " >> out1.txt
sed -i "s/CLIENT_Jobs_BUFFER .*/CLIENT_Jobs_BUFFER  $i/" params.txt 
python summarize.py >> out1.txt
echo "" >> out1.txt
done;