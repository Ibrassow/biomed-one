# -*- coding: utf-8 -*-
"""
Created on Tue Mar 16 00:53:19 2021

@author: ibrah
"""


import numpy as np
import matplotlib.pyplot as plt


    

with open('interp1.txt') as f:
    
    rr1 = np.array(f.read().splitlines())
    
with open('interp2.txt') as f2:
    
    rr2 = np.array(f2.read().splitlines())
        
    
rr1 = np.float_(rr1)
rr2 = np.float_(rr2)



beat_occur1 = np.array([0.])


for i in range(1, len(rr1)):
    beat_occur1 = np.append(beat_occur1, beat_occur1[i-1] + rr1[i-1])




s=225
e=s+15

beat_occur2 = np.arange(0, len(rr2))

"""fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 5), sharey=True)
#fig.suptitle("")

ax1.scatter(beat_occur1, rr1, color='b')
ax1.plot(beat_occur1, rr1)
ax1.set_xlabel('Temps (s)')
#ax1.set_ylabel('RR (s)')
ax1.grid(axis='both')
ax1.set_xlim((s, e))


ax2.scatter(beat_occur2, rr2, color='r')
ax2.plot(beat_occur2, rr2)
ax2.set_xlabel('Temps (s)')
#ax2.set_ylabel('RR (s)')
ax2.grid(axis='both')
ax2.set_xlim((s, e))"""



fig, (ax3) = plt.subplots(1, 1, figsize=(5, 5), sharey=True)

sc1 = ax3.scatter(beat_occur1, rr1, color='b')
pl1 = ax3.plot(beat_occur1, rr1)
sc2 = ax3.scatter(beat_occur2, rr2, color='r')
pl2 = ax3.plot(beat_occur2, rr2)
ax3.set_xlabel('Temps (s)')
ax3.grid(axis='both')
ax3.set_xlim((s, e))
ax3.legend([sc1, sc2], ["Points d'origine", "Points interpolés"])










#plt.xlim((s, e))
plt.show()