# -*- coding: utf-8 -*-
"""
@author: ibrah
"""

from simulation import Simulator




Simu = Simulator(age=36, total_duration=6, DBname="Simulation")
Simu.setAggravator(fever=True, resp_trb=False, severity=2, start_h=2, total_duration_h=4, transition_p_h=1, recovery_p_h=1)
Simu.saveConfig()
Simu.launchSimulation() 
Simu.getFullData()


Simu.sendToInfluxDB(name="Luc", surname="Bertiste", mail="", gender="", sleep_time="")
    

    



    
    
    
    
    
