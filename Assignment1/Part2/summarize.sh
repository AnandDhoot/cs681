#!/bin/bash
for i in {5,10,25,50,75,100,125,150,175,200,225}; do
	cd $i
	/usr/lib/tsung/bin/tsung_stats.pl --stats result.log
	cd -
done