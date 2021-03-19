# -*- coding: utf-8 -*-
"""

@author: ibrah
"""

import numpy as np
import matplotlib.pyplot as plt
import os
import time
import threading
import subprocess


from influxdb import InfluxDBClient







class Simulator:
    def __init__(self, age=20, total_duration=2, DBname):
        self.age = age
        self.total_duration_simu = total_duration
        self.filename = "config.txt"
        self.fever = False
        self.resp_trb = False
        self.severity = None
        self.start_h = None
        self.total_duration_h = None
        self.transition_period_h = None
        self.recovery_period = None
        
        self.simu_running = threading.Event()
        

        self.db = DataHandler(DBname)
        
        
        self.temperature = np.array([])
        self.hr = np.array([])
        self.spo2 = np.array([])
        
        

        
    def setAggravator(self, fever=False, resp_trb=True, severity=None, start_h=None, total_duration_h=None, transition_p_h=None, recovery_p_h=None):
        """
        Parameters
        ----------
        fever : bool
            DESCRIPTION.
        resp_trb : bool
            DESCRIPTION.
        severity : int
            DESCRIPTION.
        start_h : int
            DESCRIPTION.
        total_duration_h :int
            DESCRIPTION.
        transition_p_h : int
            DESCRIPTION.
        recovery_p_h : int
            DESCRIPTION.

        Returns
        -------
        None.

        """
        self.fever = fever
        self.resp_trb = resp_trb
        self.severity = severity
        self.start_h = start_h
        self.total_duration_h = total_duration_h
        self.transition_period_h = transition_p_h
        self.recovery_period = recovery_p_h
        
        


    def saveConfig(self):
        
        with open(self.filename, 'w') as file:
            
            config_info = [self.age, self.total_duration_simu, self.fever, self.resp_trb, self.severity, self.start_h, \
                           self.total_duration_h, self.transition_period_h, self.recovery_period]
            
            
            for elem in config_info:
                file.write(str(elem) + '\n')
        

    def launchSimulation(self):
        
        self.running_msg = threading.Thread(target=self.print_waiting_msg, args=()) 
        
        self.running_msg.start()
        subprocess.check_call(['simulation.exe'])
        #os.system("start simulation.exe")
        self.simu_running.set()
        print("Check your folder => results_simu.txt and your variables :) ")
        getFullData()


    def print_waiting_msg(self):
        
        while not self.simu_running.is_set():
            print("Waiting for the generator to finish...")
            time.sleep(1)
            

    
    def getFullData(self):
        
        with open('results_simu.txt') as f:
            data_lines = f.read().splitlines()
            
        for line in data_lines:
            a = line.split()
            self.temperature = np.append(self.temperature, float(a[0]))
            self.hr = np.append(self.hr, float(a[1]))
            self.spo2 = np.append(self.spo2, float(a[2]))
            

    def sendToInfluxDB(self, name, surname, mail, gender, sleep_time):
        self.db.send_personal_info(name, surname, mail, gender, self.age)
        print("Personal information sent")
        self.db.send_data(name, sleep_time, self.temperature, self.hr, self.spo2, 3)
        
        

    

    

    
    
class DataHandler(InfluxDBClient): 
    

    def __init__(self, measurementName):
        # Database settings and connection
        __host="influx.biomed.ulb.ovh"
        __db="biomed1"
        __username='biomed1'
        __password='M5xsTyS4fS0EYsFN'
        __port=80
        super().__init__(host=__host, port=__port, username=__username, password=__password, database=__db)
        self.measurement = measurementName
      
        
    def send_data(self, tag, sleep_time, temperatureArr, hrArr, spo2Arr, time_data):
        for i in range(time_data):
            line = '{},name={} temperature={},hr={},spo2={},resp=2'.format(self.measurement, tag, temperatureArr[i], hrArr[i], spo2Arr[i])
            self.write_points(line, protocol='line')
            print("Data sent", temperatureArr[i], hrArr[i], spo2Arr[i])
            time.sleep(sleep_time)
        
        
    def send_personal_info(self, name, surname, mail, gender, age):
        line = "{},name={},surname={},mail={},gender={},age={} resp=1".format(self.measurement, name, surname, mail, gender, age)
        self.write_points(line, protocol='line')
    


if __name__ == "__main__": 
    
    
    """sims = Simulator(14, 2, "Hoy")
    sims.setAggravator(fever=True, resp_trb=False, severity=2, start_h=2, total_duration_h=4, transition_p_h=1, recovery_p_h=1)
    
    sims.saveConfig()
    

    sims.launchSimulation()
    

    sims.getFullData()
    
    
    sims.sendToInfluxDB("h", "i", "y", 4, "u", 2)"""

    #time.sleep(5)

















    
    """with open('results_simu.txt') as f:
        raw_data = np.array((f.read().splitlines()))  
        raw_data = np.asfarray(raw_data,np.float)
        
    
    time = np.arange(0, len(raw_data), 1) /3600
    
    
    
    plt.xlabel("Temps (h)")
    plt.ylabel("Data")
    
    
    plt.plot(time, raw_data, label="Données brutes")
    plt.legend()
    
    plt.show()"""