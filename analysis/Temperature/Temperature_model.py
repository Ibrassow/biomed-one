# -*- coding: utf-8 -*-
"""
Created on Tue Feb 16 20:28:24 2021

@author: ibrah
"""

import numpy as np
from scipy.optimize import leastsq
import pylab as plt







t = np.arange(0,24, 1) #nombre de points qu'on prends sur la courbe
yData = [36.73, 36.39, 36.35,36.26, 36.33, 36.48, 36.44, 36.52, 36.71, 36.84, 37.3, 37.48, 37.38, 37.35, 37.44, 37.47, 37.55, 37.48, 37.51,37.41, 37.44, 37.26, 37.06, 37.02] #données du problème
mean = np.mean(yData) #donne la moyenne
chercher_std = 3*np.std(yData)/2 #donne la deviation selon un axe
phase = 0 #donne le dephasage initial
frequence = 1/24 #donne la frequence initiale
amplitude = 1 #donne l'amplitude initial
#utilisation de la methode des moindres carrees
best_function = lambda x: x[0]*np.sin(x[1]*t+x[2]) + x[3] - yData
[amp_res, freq_res, dephasage_res, moyenne_res], xcov = leastsq(best_function, [amplitude,frequence, phase, mean])
#on limite ici les erreurs et on creer une fonction avec les donnees opti
x = np.arange(0,max(t),0.05) #donne l'echelle de l'axe x
y=amp_res*np.sin(freq_res*x+dephasage_res)+moyenne_res #donne les valeurs de la temp




plt.title('Variation de la température au cours de 24H')
plt.xlabel('Heures')
plt.ylabel('Température')
plt.plot(t, yData, '.')
plt.plot(x,y, label='Best approximation')
plt.legend()
plt.show()




modelPredictions = amp_res * np.sin(freq_res * t + dephasage_res) + moyenne_res

absError = modelPredictions - yData

SE = np.square(absError) # squared errors
MSE = np.mean(SE) # mean squared errors
RMSE = np.sqrt(MSE) # Root Mean Squared Error, RMSE