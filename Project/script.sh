#!/bin/bash

sed -i 's/NUM_FIFO_Clients *$/NUM_FIFO_Clients 0/' params.txt
sed -i 's/NUM_FIFORR_Clients *$/NUM_FIFORR_Clients 0/' params.txt
sed -i 's/NUM_SJF_Clients *$/NUM_SJF_Clients 0/' params.txt
sed -i 's/NUM_SDF_Clients *$/NUM_SDF_Clients 0/' params.txt

sed -i 's/NUM_FIFO_Clients 0/NUM_FIFO_Clients 5 1 1 1 1 1/' params.txt 
echo "FIFO" > out1.txt
python summarize.py >> out1.txt
echo "" >> out1.txt
echo "" >> out1.txt

sed -i 's/NUM_FIFO_Clients 5 1 1 1 1 1/NUM_FIFO_Clients 0/' params.txt 
sed -i 's/NUM_FIFORR_Clients 0/NUM_FIFORR_Clients 5 1 1 1 1 1/' params.txt
echo "FIFORR" >> out1.txt
python summarize.py >> out1.txt
echo "" >> out1.txt
echo "" >> out1.txt

sed -i 's/NUM_FIFORR_Clients 5 1 1 1 1 1/NUM_FIFORR_Clients 0/' params.txt
sed -i 's/NUM_SJF_Clients 0/NUM_SJF_Clients 5 1 1 1 1 1/' params.txt
echo "SJF" >> out1.txt
python summarize.py >> out1.txt
echo "" >> out1.txt
echo "" >> out1.txt

sed -i 's/NUM_SJF_Clients 5 1 1 1 1 1/NUM_SJF_Clients 0/' params.txt
sed -i 's/NUM_SDF_Clients 0/NUM_SDF_Clients 5 1 1 1 1 1/' params.txt
echo "SDF" >> out1.txt
python summarize.py >> out1.txt
echo "" >> out1.txt
echo "" >> out1.txt