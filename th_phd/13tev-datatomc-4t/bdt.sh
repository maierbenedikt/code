th_datatomc_x tHlep_ct.config -varexpr "bdtout[0]" -xtitle "BDT output" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 8 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -datacuts "bdtout[0]<0" -suffix "lep_bdtout0" -down_uncfile ../13tev-yields-4t/mydata/bdtout0_tot_total_down.dat -up_uncfile ../13tev-yields-4t/mydata/bdtout0_tot_total_up.dat
th_datatomc_x tHlep_ct.config -varexpr "best_recbdtout[0]" -xtitle "best BDT output under tHq hypothesis" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 8 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -suffix "lep_recbdtout0"
th_datatomc_x tHlep_ct.config -varexpr "top_best_recbdtout" -xtitle "best BDT output under t#bar{t} hypothesis" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 8 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -suffix "lep_toprecbdtout"
th_datatomc_x tHlep_tHq_tWH.config -varexpr "bdtout[0]" -xtitle "BDT output" -stackmc 0 -data "" -datapath "" -lumi 0 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.21:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -suffix "lep_bdtout0" -ytitle "Norm. to unit area"
th_datatomc_x tHlep_tHq_tWH.config -varexpr "bdtout[12]" -xtitle "BDT output" -stackmc 0 -data "" -datapath "" -lumi 0 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.21:0.74:42:0.04:#it{C}_{t}=+1,  #it{C}_{V}=1" -suffix "lep_bdtout457" -ytitle "Norm. to unit area" -mcweights+ "tHq:weights_syst[457]/weights_syst[0]" "tWH:weights_syst[457]/weights_syst[0]"