echo $date > onlysys_bbb.txt
echo -n "bbb  : " >> onlysys_bbb.txt
freezing=''
for j in $(cat syst_wobbb.txt); do 
    freezing+="$j,"; 
done;  

echo "combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _onlybbb_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${freezing%,}"
combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _onlybbb_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${freezing%,}| grep "Expected\ 50.0\%" >> onlysys_bbb.txt; 


#combine -M Asymptotic -d higgsCombinenicholas2.MultiDimFit.mH120.root -w w -n nicholas3 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${i} | grep "Expected\ 50.0\%" >> removal_effect.txt; done
#  LocalWords:  cOmbine