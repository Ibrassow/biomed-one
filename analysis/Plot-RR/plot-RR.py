# -*- coding: utf-8 -*-
"""
@author: ibrah
"""

import numpy as np
import matplotlib.pyplot as plt









if __name__ == "__main__": 
    
    
    
    with open('RR-70-5.txt') as f:
        
        rr = np.array(f.read().splitlines())
    
    
    rr = np.float_(rr)
    

    beat_occur = np.array([0.])


    for i in range(1, len(rr)):
        beat_occur = np.append(beat_occur, beat_occur[i-1] + rr[i-1])
        
        
    s = 0
    e = 100
        

    plt.legend(["moyenne : 0.86 s \nécart-type : 12 s"])
    
    
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 5))
    #fig.suptitle("400 points de données générés avec une moyenne de 0.86 s (70 bpm) et un écart-type de 12 s (5 bpm)")

    ax1.plot(beat_occur[s:e], rr[s:e])
    ax1.set_xlabel('Temps (s)')
    ax1.set_ylabel('RR (s)')
    ax1.grid(axis='y')
    #ax1.set_title('Tachogramme RR')
    
    
    
    ax2.plot(beat_occur[s:e], 60/rr[s:e])
    ax2.set_xlabel('Temps (s)')
    ax2.set_ylabel('Fréquence cardiaque instantanée (bpm)')
    ax2.grid(axis='y')
    #ax2.set_title('Rythme cardiaque')



    plt.show()
    #fig.savefig('RR-70-5.png', dpi=fig.dpi)
    
    

    


    
