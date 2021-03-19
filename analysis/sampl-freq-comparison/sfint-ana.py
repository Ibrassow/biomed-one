

# -*- coding: utf-8 -*-
"""
@author: ibrah
"""

import numpy as np
import matplotlib.pyplot as plt









if __name__ == "__main__": 
    
    
    
    with open('fint-1.txt') as f:
        
        rr1 = np.array(f.read().splitlines())
        
    with open('fint-4.txt') as f2:
        
        rr2 = np.array(f2.read().splitlines())
        
    with open('fint-4.txt') as f3:
        
        rr3 = np.array(f3.read().splitlines())
                      
    """with open('fint-1000.txt') as f4:
        
        rr4 = np.array(f4.read().splitlines())"""
    
    
    
    
    
    
    
    
    
    
    
    
    
    rr1 = np.float_(rr1)
    rr2 = np.float_(rr2)
    rr4 = np.float_(rr3)
    rr8 = np.float_(rr4)
    

    beat_occur1 = np.array([0.])
    beat_occur2 = np.array([0.])
    beat_occur4 = np.array([0.])
    beat_occur8 = np.array([0.])


    for i in range(1, len(rr1)):
        beat_occur1 = np.append(beat_occur1, beat_occur1[i-1] + rr1[i-1])
        
    for i in range(1, 4096):
        beat_occur2 = np.append(beat_occur2, beat_occur2[i-1] + rr2[i-1])
    
    for i in range(1, 4096):
        beat_occur4 = np.append(beat_occur4, beat_occur4[i-1] + rr4[i-1])
    
    for i in range(1, 4096):
        beat_occur8 = np.append(beat_occur8, beat_occur8[i-1] + rr8[i-1])
    
    """for i in range(1, 4096):
        beat_occur4 = np.append(beat_occur4, beat_occur4[i-1] + rr4[i-1])"""
        
        
        
    
        
    s = 240
    e = 250
        

    
    
    fig, (ax1, ax2, ax4, ax8) = plt.subplots(1, 4, figsize=(15, 5), sharey=True)
    #fig.suptitle("400 points de données générés avec une moyenne de 0.86 s (70 bpm) et un écart-type de 12 s (5 bpm)")

    ax1.scatter(beat_occur1[s:e], rr1[s:e], color='r')
    ax1.plot(beat_occur1[s:e], rr1[s:e])
    ax1.set_xlabel('Temps (s)')
    #ax1.set_ylabel('RR (s)')
    ax1.grid(axis='both')
    ax1.set_title('1 Hz') 
    
    
    ax2.scatter(beat_occur2[s*2:e*2]/2, rr2[s*2:e*2], color='r')
    ax2.plot(beat_occur2[s*2:e*2]/2, rr2[s*2:e*2])
    ax2.set_xlabel('Temps (s)')
    #ax2.set_ylabel('RR (s)')
    ax2.grid(axis='both')
    ax2.set_title('2 Hz') 
    
    ax4.scatter(beat_occur4[s*4:e*4]/4, rr4[s*4:e*4], color='r')
    ax4.plot(beat_occur4[s*4:e*4]/4, rr4[s*4:e*4])
    ax4.set_xlabel('Temps (s)')
    #ax4.set_ylabel('RR (s)')
    ax4.grid(axis='both')
    ax4.set_title('4 Hz') 
    
    
    ax8.scatter(beat_occur8[s*8:e*8]/8, rr8[s*8:e*8], color='r')
    ax8.plot(beat_occur8[s*8:e*8]/8, rr8[s*8:e*8])
    ax8.set_xlabel('Temps (s)')
    #ax8.set_ylabel('RR (s)')
    ax8.grid(axis='both')
    ax8.set_title('8 Hz') 
    
    
    """ax4.plot(beat_occur4[s:e]/1000, rr4[s:e])
    ax4.set_xlabel('Temps (s)')
    ax4.set_ylabel('RR (s)')
    ax4.grid(axis='y')"""
    
    




    plt.show()

    
    

    


    
