rm makedatacards.sh
for i in {0..50}; do
    for reg in 3m 4m; do
	echo ./datacards.sh $i $reg >> makedatacards.sh 
done
done