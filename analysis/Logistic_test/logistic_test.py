# -*- coding: utf-8 -*-
"""
Created on Sat Mar 13 23:30:57 2021

@author: ibrah
"""

import numpy as np
import matplotlib.pyplot as plt




"""def logistic(height, total_duration_h):
    
    data = np.array([])
    
    M_shift = total_duration_h * 60 / 2
    
    charac_time = M_shift * 0.75
    
    growth = lambda t : (- np.log(81) * (t - M_shift) / charac_time)
    
    for t in range(total_duration_h*60):
        
        calc = height / (1 + np.exp(growth(t)))
        data = np.append(data, calc)
        
    
    return data"""

def logistic(height, total_duration, gr, t0):
    
    data = np.array([])
    time = np.array([])
    
    
    growth = lambda t : - gr * (t - t0) 
    
    for t in range(-total_duration+t0, total_duration+t0):
        
        calc = height / (1 + np.exp(growth(t)))
        data = np.append(data, calc)
        time = np.append(time, t)
        
    
    return data, time




if __name__ == "__main__": 
    
    #This for the simple logistic curve
    """Graph
    K_height = 2
    duration = 15
    gr = 0.5
    t0 = 5
    
    u, t = logistic(K_height, duration, gr, t0)
    
    fig, ax = plt.subplots(figsize=(12,6))
    
    
    plt.xlabel("Variable temporelle")
    plt.ylabel("f(t)")
    
    ax.set_xticks(np.arange(-duration+t0, duration+t0, 1))
    
    plt.grid(which='both')
    ax.plot(t, u)
    plt.show()"""
    
    
    #This for the entire aggravation curve
    with open('ff9801.txt') as f:
        data = np.array((f.read().splitlines()))  
        data = np.asfarray(data,np.float)
    
    plt.xlabel("Heure (h)")
    plt.ylabel("Variations ajoutées au différents modèles")
    hours = np.arange(0, len(data), 1) / 3600
    plt.plot(hours, data)
    
    
    
    
    
    
    
    #This for the discontinuity
    #with open('logistic_test.txt') as f:
    """with open('ff9801.txt') as f:
        data = np.array((f.read().splitlines()))  
        data = np.asfarray(data,np.float)
    
    plt.xlabel("Temps (s)")
    plt.ylabel("Variations ajoutées au différents modèles")
    plt.plot(data[10000:12000])"""
        
        
        
    