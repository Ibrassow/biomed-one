# -*- coding: utf-8 -*-
"""

@author: ibrah
"""

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


#This for the entire aggravation curve
with open('five-day_spo2.txt') as f:
    raw_data = np.array((f.read().splitlines()))  
    raw_data = np.asfarray(raw_data,np.float)
    
    



time = np.arange(0, len(raw_data), 1) /(3600*24)


data = np.array([time, raw_data]).T

#Converting to dataframe

columns = ["Time", "data"]
index = [f'N-{num}' for num in range(len(data))]
df = pd.DataFrame(data, columns=columns, index=index)


nn=300*12

df['roll_data'] = df['data'].rolling(nn).mean()







plt.xlabel("Temps (jours)")
plt.ylabel("Variations ajoutées au différents modèles")


#fig, (ax) = plt.subplots(3, sharex=False)


plt.plot(time, raw_data, label="Données brutes")
plt.plot(time, df['roll_data'], label="Moyenne mouvante (1h)")
plt.legend()

plt.show()