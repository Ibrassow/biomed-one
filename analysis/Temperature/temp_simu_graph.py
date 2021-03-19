# -*- coding: utf-8 -*-
"""

@author: ibrah
"""

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd





with open('temp_curve_2h.txt') as f:
    raw_data = np.array((f.read().splitlines()))  
    raw_data = np.asfarray(raw_data,np.float)
    
    
time = np.arange(0, len(raw_data), 1)/60





data = np.array([time, raw_data]).T

#Converting to dataframe

columns = ["Time", "data"]
index = [f'N-{num}' for num in range(len(data))]
df = pd.DataFrame(data, columns=columns, index=index)


nn=60

df['roll_data'] = df['data'].rolling(nn).mean()





#fig, (ax) = plt.subplots(3, sharex=False)
plt.xlabel("Temps (min)")
plt.ylabel("Température (°C)")
#plt.plot(time, raw_data)

plt.plot(time, raw_data, label="Données brutes")
plt.plot(time, df['roll_data'], label="Moyenne mouvante (1 min)")
plt.legend()

plt.show()