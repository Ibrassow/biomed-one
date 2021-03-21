# -*- coding: utf-8 -*-
"""
@author: Ibrahima S. Sow
"""

from alertmsg import ALERT_MSG, MAIL
from patient import Patient

import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

import time


"""Depending on the age and the commorbidities AS WELL AS the location of measurement... 
These are indicative for the moment
"""

HR_MAX = 100.
HR_HIGH = 90.
HR_MIN = 60.
MILD_FEVER = 37.5
MID_FEVER = 38.
HIGH_FEVER = 39.
LOW_TEMP = 35.
SPO2_MID = 94
SPO2_MIN = 92


LEVEL1 = 1
LEVEL2 = 2
LEVEL3 = 3
OUTLIER = 4
OK = 5

MEDICAL_TEAM_MAIL = ""

class Alert:    
    
    
    def __init__(self, _Patient):
        self.patient = _Patient 
        self.mail_sender = AlertSender(self.patient.mail)
        self.main_sender_med_team = AlertSender(MEDICAL_TEAM_MAIL)
        self.battery_alert_status = 0
        
      
    def __get_alert_msg(self, category, flag):
        if category == "battery":
            return ALERT_MSG[category][flag]
        else:
            return [ALERT_MSG[category], ALERT_MSG["FLAG"][flag]]

        



    def short_analysis(self):
        
        #Battery analysis
        
        battery_flag = self.check_battery()
        
        if battery_flag == "LOW":
            
            if self.battery_alert_status == 0:
                
                self.battery_alert_status = 1
                alrt = self.__get_alert_msg('battery', battery_flag)
                self.mail_sender.send(alrt[0], alrt[1].format(self.patient.battery_life))
                print("Battery is low... Alert sent")
                

        elif battery_flag == "VERY LOW":
            if self.battery_alert_status == 1:
                
                self.battery_alert_status = 2
                alrt = self.__get_alert_msg('battery', battery_flag)
                self.mail_sender.send(alrt[0], alrt[1].format(self.patient.battery_life))
                print("Battery is very low... Alert sent")
            
        elif battery_flag == "OK":
            self.battery_alert_status = 0
          
            
          #OUTLIER DATA ?
          
            
    def danger_analysis(self, list_spo2, list_temp, list_hr):
        
        """if last < 94:
        if last <= 92:
            Urgence médical (LEVEL1)
        elif last > 92 #93 
            if last-1 < 94:
                Alerte anormalement bas! (LEVEL1)
            else:
                Wait for next measure
                """
        alrt_msg = []
        print("Danger module is analyzing the current state...")
        
        spo2_flag = self.check_spo2()
        temp_flag = self.check_temp()
        hr_flag = self.check_hr()
        
        if self.patient.t_last_spo2 == self.patient.time_since_last:
            if spo2_flag != OK:
                if spo2_flag == LEVEL1:
                    alrt_spo2 = self.__get_alert_msg('spo2', spo2_flag)
                    alrt_msg.append(alrt_spo2)
                    print("DANGEROUS SPO2 ! \n")
                    #SEND MAIL
                elif spo2_flag == LEVEL2:
                    try:
                        if self.check_spo2(list_spo2[1]) != OK:
                            alrt_spo2 = self.__get_alert_msg('spo2', spo2_flag)
                            alrt_msg.append(alrt_spo2)
                            #SEND MAIL
                        else:
                            pass
                    except IndexError:
                        print("Not enough data to make conclusions on SPO2...")
        
        
        if temp_flag != OK:
            if temp_flag == LEVEL1:
                alrt_temp = self.__get_alert_msg('temperature', temp_flag) #may the temperature rise suddenly ?
                alrt_msg.append(alrt_temp)
                print("DANGEROUS TEMPERATURE !")
            elif temp_flag == LEVEL2:
                if self.check_temp(list_temp[1]) == LEVEL1 or self.check_temp(list_temp[1]) == LEVEL2:
                    alrt_temp = self.__get_alert_msg('temperature', temp_flag)
                    alrt_msg.append(alrt_temp)
                else:
                    pass


        if self.patient.t_last_hr == self.patient.time_since_last:       
            if hr_flag != OK:
                if hr_flag == LEVEL1:  #condition on second high or low
                    alrt_hr = self.__get_alert_msg('hr', temp_flag)
                    alrt_msg.append(alrt_hr)#may the temperature rise suddenly ?
                    print("DANGEROUS HEART RATE !")

        
        if len(alrt_msg) != 0:
            mm = self.craft_msg_med(alrt_msg)
            self.mail_sender_med_team.send(mm[0], mm[1])
            print("... DANGER Alert sent!")

        





        

    def craft_msg_med(self, list_alrt_msg):
        #Reunite the message part 
        
        text = MAIL['INTRO'].format(self.patient.patientid, self.patient.name)
        subj_l = []
        tt = []
        
        for i in list_alrt_msg:
            subj_l.append(i[0])
            tt.append(i[1].format(i[0]))
        
        
        subjectline = "Alerte : " + ' - '.join(subj_l)
        
        text += '\n'.join(tt)
        text += MAIL['MEDICAL_DETAILS'].format(self.patient.temperature,self.patient.hr,self.patient.spo2)
        text += MAIL['CLOSING']
        
        print(subjectline)
        print(text)
        return subjectline, text
    
    
    
    
    
    def craft_msg_patient(self, list_alrt_msg):
        pass
                



    def check_temp(self, temp_eval = None):
        
        if temp_eval == None: #allow to test other spo2 values
            temp_eval = self.patient.temperature
        
        if self.patient.temperature <= 35 or self.patient.temperature >= 41:
            return OUTLIER
        elif self.patient.temperature >= HIGH_FEVER:
            return LEVEL1
        elif self.patient.temperature < HIGH_FEVER and self.patient.temperature >= MID_FEVER:
            return LEVEL2
        elif self.patient.temperature < LOW_TEMP:
            return LEVEL3
        else:
            return OK



    def check_spo2(self, spo2_eval = None):

        if spo2_eval == None: #allow to test other spo2 values
            spo2_eval = self.patient.spo2
            
        if spo2_eval <= 85. or spo2_eval >= 101:
                return OUTLIER
        elif spo2_eval <= SPO2_MIN: #emergency, alert medical staff <=92
            return LEVEL1
        elif spo2_eval <= SPO2_MID: #<=94 but >92
            return LEVEL2         
        else:
            return OK  
            
            
            

    def check_hr(self, hr_eval = None):
        
        if hr_eval == None: #allow to test other spo2 values
            hr_eval = self.patient.hr
            
        if self.patient.hr <= 40 or self.patient.hr >= 160:
            return OUTLIER
        elif self.patient.hr > HR_MAX: #Abnormal heart rate -- This does not take into account exercising
            return LEVEL1
        elif self.patient.hr > HR_HIGH: #Elevated heart rate
            return LEVEL2
        elif self.patient.hr < HR_MIN: #Low heart rate - This excludes trained people
            return LEVEL3
        else: #Hr is within range
            return OK      
      
    def check_battery(self):
        
        if self.patient.battery_life <= 30 and self.patient.battery_life >= 15:
            return "LOW"
        elif self.patient.battery_life < 15:
            return "VERY LOW"
        elif self.patient.battery_life > 100 or self.patient.battery_life < 0:
            return OUTLIER
        else:
            return "OK" 
    
 
    



class AlertSender:
    
    def __init__(self, recipient):
        
        self.sender = ""
        self.password = ""
        self.recipient = recipient


    
    def send(self, subjectline, text):
        
        server = smtplib.SMTP('smtp.gmail.com', 587)
        server.starttls()
        server.login(self.sender, self.password)
        msgg = MIMEMultipart()
        msgg['Subject'] = subjectline
        msgg.attach(MIMEText(text, 'plain'))
        server.sendmail(self.sender, self.recipient, msgg.as_string())
        server.quit()






        
