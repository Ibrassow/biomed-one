# -*- coding: utf-8 -*-
"""
@author: ibrah
"""

from simulation import Simulator
import numpy as np
import matplotlib.pyplot as plt


#Each setting is in HOURS"
Simu = Simulator(age=36, total_duration=4, DBname="Simulation")
Simu.setAggravator(fever=False, resp_trb=False, severity=2, start_h=1, total_duration_h=3, transition_p_h=1, recovery_p_h=1)
Simu.saveConfig()
Simu.launchSimulation() 

hr = Simu.hr

time = np.arange(0,1000)/3600 #time in hours
plt.plot(time, hr[0:1000])
plt.xlabel("Time (h)")
plt.ylabel("Heart rate (bpm)")





#Simu.sendToInfluxDB(name="Luc", surname="Bertiste", mail="", gender="", sleep_time="")
    

    



    
    
    
    
    
