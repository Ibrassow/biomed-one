# -*- coding: utf-8 -*-
"""
@author: Ibrahima S. Sow
"""

LEVEL1 = 1
LEVEL2 = 2
LEVEL3 = 3
OUTLIER = 4
OK = 5



ALERT_MSG = {
    
    "battery" : {
        
        "LOW" : ["Batterie faible", "La batterie de votre dispositif est de {} %. Veuillez retirer le boitier du brassard et le recharger via le câble USB."],
        "VERY LOW" : ["Batterie TRES FAIBLE", "La batterie de votre dispositif n'est que de {} % !!! Veuillez donc retirer le boîtier du brassard et le recharger via le câble USB au plus vite."]
        },
    
    "FLAG" : {      
        LEVEL1 : """{} se situant dans une zone CRITIQUE et nécessite une assistance médicale immédiate. Ce symptôme s’est déclaré à l'instant!. \
Le patient se trouve actuellement à son domicile. Veuillez prendre contact AU PLUS VITE avec le patient pour la suite de la procédure. """,  
        LEVEL2 : """{} se situant dans une zone ALARMANTE. Le patient se trouve actuellement à son domicile. Veuillez prendre contact au plus vite avec celui-ci pour la suite de la procédure.  \
Dans le cas où vous envisagez une hospitalisation, veuillez au préalable en informer l'équipe technique. Pour rappel, le patient ne peut sortir du domicile. """,
        LEVEL3 : """""""",
        },
        
    "spo2" : "TAUX DE SPO2",
    "hr" : "RYTHME CARDIAQUE",
    "temperature" : "TEMPERATURE"
    
        }






"""ALERT_MSG = {
        "battery" : {
        
        "LOW" : ["Batterie faible", "La batterie de votre dispositif est de {} %. Veuillez retirer le boitier du brassard et le recharger via le câble USB."],
        "VERY LOW" : ["Batterie TRES FAIBLE", "La batterie de votre dispositif n'est que de {} % !!! Veuillez donc retirer le boîtier du brassard et le recharger via le câble USB au plus vite."]
        },
    
        "spo2" : {
    
    LEVEL1: ["TAUX DE SPO2", " "],
    LEVEL2: ["Taux de SPO2", " "],
    LEVEL3: ["Taux de SPO2", " "]

    },
    

    "TEMPERATURE" : {
        
        LEVEL1: ["TEMPERATURE"," "],
        LEVEL2: ["TEMPERATURE"," "],
        LEVEL3: ["temperature"," "]
        
        },
    
    
    "hr" : {
        
        LEVEL1: ["RYTHME CARDIAQUE"," "],
        LEVEL2: ["RYTHME CARDIAQUE"," "],
        LEVEL3: ["Rythme cardiaque"," "]

    }


    
    }"""






MAIL = {
        'INTRO' : """ATTENTION: à l'intention du médecin responsable seulement.
        
Ce mail concerne le patient de numéro de référence {}. Le patient {} semble éprouver des difficultés au niveau de/du:\n""",
        
        
        'CLOSING' : """

Ceci est un mail automatique, veuillez ne pas y répondre. \
Pour contacter le patient, il est important de se référer au numéro de contact fourni dans la fiche d'identité. \
Pour avoir accès au rapport complet du patient, vous \
pouvez envoyer un mail sur biomed1.monitoring@gmail.com., nous vous enverrons les informations dans les plus brefs délais. 
----------------------------------------------------------------------------------------------
Une question ou un problème technique  ? 
Veuillez contacter l'équipe technique au 02 777 44 56 ou envoyer un mail sur biomed1.monitoring@gmail.com. Nous ferons notre possible pour répondre à vos besoins au plus vite. """,

        
        'MEDICAL_DETAILS' : """Veuillez trouver les détails ci-dessous. 

Taux de SPO2 : {}
Rythme cardiaque : {}
Temperature : {}"""}

        
        










def craft_msg_med(list_alrt_msg):
    #Reunite the message part 
    
    text = MAIL['INTRO'].format("IDTEMP", "KKKK")
    subj_l = []
    tt = []
    
    for i in list_alrt_msg:
        subj_l.append(i[0])
        tt.append(i[1])
    
    
    subjectline = "Alerte : " + ' - '.join(subj_l)
    
    text += MAIL['MEDICAL_DETAILS'].format(37,72,97)
    text += MAIL['CLOSING']
    
    return subjectline, text



 








