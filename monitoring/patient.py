# -*- coding: utf-8 -*-
"""

@author: Ibrahima S. Sow
"""

class Patient:
    
    #state => const class variable ?
    
    def __init__(self, name, surname, mail, age, gender, patientid, risky = False):
        """
        
        Parameters
        ----------
        name : name of the specific patient
            Need a correspondance to its ID
        hr : int
        temperature : float
        battery_life : int
        spo2 : float, set to 0 if no value  ==> change that

        """
        
        self.name = name
        self.surname = surname
        self.mail = mail
        self.age = age
        self.gender = gender
        self.patientid = patientid
        print("Patient found : {}, {}, {} years, {}, {}".format(name, surname, age, gender, mail))
        
        self.hr = 0.
        self.temperature = 0.
        self.battery_life = 0.
        self.spo2 = 0.
        self.state = 0
        self.wifi_strength = 0.
        self.time_since_last = 0.
        
        self.t_last_hr = 0.
        self.t_last_spo2 = 0.
        
        
    
        
    def update_state(self, current_data):
        
        
        self.time_since_last = current_data['time']
        
        try:
            self.hr = current_data['last_hr']
            self.t_last_hr = self.time_since_last
        except KeyError:
            print("didn't get hr data")
            
        
        try:
            self.spo2 = current_data['last_spo2']
            self.t_last_spo2 = self.time_since_last
        except KeyError:
            print("didn't get spo2 data")
        
        
        self.temperature = current_data['last_temperature']
        self.battery_life = current_data['last_battery']
        self.wifi_strength = current_data['last_rssi']
        
        print("Patient current : T° : {} - HR : {} - SPO2 : {} - Battery : {} - WiFi signal : {}".format(self.temperature, self.hr, self.spo2, self.battery_life, self.wifi_strength))



