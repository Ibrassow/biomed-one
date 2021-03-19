# -*- coding: utf-8 -*-
"""
@author: Ibrahima S. Sow
"""

from influxdb import InfluxDBClient
import numpy as np
import time


"""
# Database settings
host="influx.biomed.ulb.ovh"
db="biomed1"
username='biomed1'
password='M5xsTyS4fS0EYsFN'
client = InfluxDBClient(host=host, port=80, username=username, password=password, database=db)
"""
    


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
     
        
    def get_personal_data(self):
        
        print("Searching for the patient in the database...")
        convert = np.array([])
        
        while len(convert) == 0:
            res = self.query("SELECT * FROM {} WHERE resp=1".format(self.measurement))
            convert = np.array(list(res.get_points()))
            time.sleep(10)
        return convert[0]['name'],convert[0]['surname'],convert[0]['mail'],convert[0]['age'],convert[0]['gender']
    
    
    def process_timestamps(self, timestamp):
        """
        Parameters
        ----------
        timestamp : TYPE
            Timestamp in seconds in RFC339 datetime format in seconds

        Returns
        -------
        The elapsed minutes since the timestamp occured

        """
        return int((time.time()-timestamp)/60) #no need to round the minute
    
    
    def get_last_data(self, time):
        """
        Parameters
        ----------
        time : String
            query time
            
        Returns
        -------
        None or a dictionary containing the requested values (processed_timestamp, temp, hr, spo2, battery) 
        """
        
        data_query = list(self.query("SELECT last(*) FROM {} WHERE resp=2 AND time > now() - {}".format(self.measurement, time), epoch='s').get_points())
        
        if len(data_query) == 0:
            return None
        else:
            last_data = data_query[0] #Dict
            last_data['time'] = self.process_timestamps(last_data['time'])
            return last_data
            

    
    def get_mean_data(self, symptom, time):
        """
        Parameters
        ----------
        symptom : string
            symptom of interest
        time : String
            query time

        Returns
        -------
        mean_data : dict 
            keys are "mean_symptom"
        """
        data_query = list(self.query("SELECT mean({}) FROM {} WHERE resp=2 AND time > now() - {}".format(symptom, self.measurement, time), epoch='s').get_points())
        
        if len(data_query) == 0:
            return None
        else:
            mean_data = data_query[0] #Dict
            mean_data['time'] = self.process_timestamps(mean_data['time'])
            return mean_data

    
    def get_specific_data(self, symptom, time, timestamp=False):
        """
        Parameters
        ----------
        symptom : string
            symptom of interest
        time : String
            query time

        Returns
        -------
        data : list containing the requested data
        """
        data_query = list(self.query("SELECT {} FROM {} WHERE resp=2 AND time > now() - {}".format(symptom, self.measurement, time), epoch='s').get_points())
        #list of dict
        if len(data_query) == 0:
            return None
        else:
            data = []
            if timestamp == False:
                for elem in data_query:
                    data.append(elem[symptom])
                return data
            else:
                for elem in data_query:
                    data.append({ symptom : elem[symptom], 'time' : self.process_timestamps(elem['time'])})
                return data
            
    
        


    
    def send_data():
        pass






















#Some notes



"""
k = list(client.query("Select exponential_moving_average(*,100) from Pourcentage").get_points())
o = []
for i in range(len(k)):
    o.append(k[i]['exponential_moving_average_PourcentageBATTERIE'])

SELECT last(value) FROM response_times WHERE time > now() - 1h
db = DataHandler('Nocturne')


db.query('DROP SERIES from Nocturne')
SELECT last(value) FROM response_times WHERE time > now() - 1h


line="Nocturne,name=Bob,surname=McBob,mail=oyo@rempli.com resp=1"
db.write_points(line, protocol='line')


monit.db.write_points('Nocturne,name=Bob temperature=37,hr=68,spo2=97,resp=2',protocol='line')

line="Nocturne,name=Bob,surname=McBob,mail=oyo@rempli.com resp=1"
client.write_points(line, protocol='line')
client.write_points('Nocturne,name=Bob temperature=37,hr=68,spo2=97,battery=87 resp=2',protocol='line')



https://docs.influxdata.com/influxdb/v1.8/query_language/explore-data/
"""
