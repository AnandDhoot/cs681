for i in {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75}; do
	echo -n $i,
	cat ./One/$i.log | cut -d' ' -f2 | awk '{ num += 1; sum += $1 } END { print sum/num }'
done;