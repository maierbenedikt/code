less limits.txt | grep '2.5%' |  awk '{print $5}' > 2p5.dat
less limits.txt | grep '16.0%' |  awk '{print $5}' > 16p0.dat
less limits.txt | grep '50.0%' |  awk '{print $5}' > 50p0.dat
less limits.txt | grep '84.0%' |  awk '{print $5}' > 84p0.dat
less limits.txt | grep '97.5%' |  awk '{print $5}' > 97p5.dat
less limits.txt | grep 'Observed' |  awk '{print $5}' > observed.dat
pr -mts',' cfvalues.txt 2p5.dat 16p0.dat 50p0.dat 84p0.dat 97p5.dat observed.dat > limits.dat
echo "cf,twosigdown,onesigdown,exp,onesigup,twosigup,obs" > limits_1.dat
head -17 limits.dat >>limits_1.dat
echo "cf,twosigdown,onesigdown,exp,onesigup,twosigup,obs" > limits_1p5.dat
head -34 limits.dat |tail -n 17 >>limits_1p5.dat
echo "cf,twosigdown,onesigdown,exp,onesigup,twosigup,obs" > limits_0p5.dat
tail -17 limits.dat >>limits_0p5.dat


sed -n '2p' limits_1.dat  >tmpline
sed -i "7 i $(cat tmpline)" limits_1.dat 
sed -i '2d' limits_1.dat 