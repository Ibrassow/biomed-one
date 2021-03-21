# -*- coding: utf-8 -*-
"""
@author: Ibrahima S. Sow
"""

"""
- Add daily report to patient state

"""


#import numpy as np
import threading

from db_handler import DataHandler
from patient import Patient
from alert import Alert

#from influxdb import InfluxDBClient
import time




class Monitor:
    """Manager of the monitoring system for a single patient"""
    
    def __init__(self, ID_patient):
        
        
        self.__patient_found = False
        self.db = DataHandler(ID_patient)
        #Query the information of the patient, does not stop until received (don't forget to add commorbidities and other)
        name, surname, mail, age, gender = self.db.get_personal_data()
        """All fields required  ==> Handle this""" 
        
        #if age <= 60 or commorbidities:
         #   risk_factor = True
         
        self.patient = Patient(name=name, surname=surname, mail=mail, age=age, gender=gender, patientid=ID_patient) 
        self.alert_sys = Alert(self.patient)
        
        self.__running = False


        #Thread settings for the 5-minute sub-montoring process
        self.stop_short_monitoring = threading.Event()
        self.thread_short_monitoring = None
        
        #Thread settings for the danger sub-monitoring process
        self.stop_danger_monitoring = threading.Event()
        self.thread_danger_monitoring = None
        
        
        self.time_resume_danger = 0.
        
        self.exit_event = threading.Event()
        
    
    def __get_personal_details(self):
        pass
    
  
    
    def start_monitoring(self):
        
        
        self.exit_event.clear()
        self.__running = True
        
        
        self.thread_short_monitoring = threading.Thread(target=self.__short_term_analysis, args=()) 
        self.thread_danger_monitoring = threading.Thread(target=self.__danger_analysis, args=()) 
        
        
        
        self.thread_short_monitoring.start()
        self.thread_danger_monitoring.start()
        print("Threads started\n")

        
    def stop_monitoring(self):
        self.__running = False
        
        self.exit_event.set()
        
        self.thread_danger_monitoring.join()
        self.thread_short_monitoring.join()
        
        print("Every thread stopped")  
    
  
    
    
    def __short_term_analysis(self):
        
        #manage the battery level, update patient state, check signal and stop other monitoring process if necessary
        
        print("Short analysis module started")

        danger_start_condition = False
        
        no_signal_start = 0.
        no_signal_time = 0.
        signal = True

        while self.__running == True:
            
            current_data = self.db.get_last_data('5m')
            
            if current_data == None:
                #HANDLE LOSS OF SIGNAL 
                if signal == True:
                    no_signal_start = time.time()
                    signal = False

                no_signal_time = ((time.time() - no_signal_start))

            
                if no_signal_time/60 > 20:
                    
                    if self.db.get_specific_data("rssi", "25m") == None:
                        if not self.stop_danger_monitoring.is_set():
                            self.stop_danger_monitoring.set()
                            print("NO SIGNAL : Pausing the danger monitoring...\n")
                        danger_start_condition = False
                    
                hh = int(no_signal_time/3600)%60
                mm = int(no_signal_time/60)%60
                ss = int(no_signal_time % 60)
                
                print("NO SIGNAL - since {}:{}:{}".format(hh,mm,ss))
                
                
            
            else: #if there are data
                signal = True
                no_signal_time = 0.
                
                self.patient.update_state(current_data)
                
                if danger_start_condition == False:
                        danger_start_condition == True 
                        self.stop_danger_monitoring.clear() #allow the danger monitoring to run
                        self.time_resume_danger = time.time() #gets the time of the execution authorization
                
                self.alert_sys.short_analysis()
             
                
            if self.exit_event.is_set():
                break
            
            time.sleep(250)
            
          
            
    def __danger_analysis(self):
        
        time.sleep(60)
        
        print("Danger analysis module started")
        
        while self.__running == True:
            
            if not self.stop_danger_monitoring.is_set() and ((time.time() - self.time_resume_danger)/60) >= 10: 
                #execute when the flag has been cleared since at least 10min, to cope with the possible loss of signal
                
                ls_spo2 = self.db.get_specific_data("spo2", "15m")
                ls_temp = self.db.get_specific_data("temperature", "20m")
                ls_hr = self.db.get_specific_data("hr", "20m")
                self.alert_sys.danger_analysis(ls_spo2, ls_temp, ls_hr) 
            
            
            if self.exit_event.is_set():
                break
            
            time.sleep(300)
            
        
        
    def __mild_analysis(self):
        pass

    
    





    


if __name__ == "__main__": 
    
    
    #t1 = threading.Thread(target=simu_test, args=())
    #t1.start()
    
    Monit = Monitor("")
    Monit.start_monitoring()
    
    
    
    
        
        
    






    
    
    
   
    
   
    
   

    


    
    
    
