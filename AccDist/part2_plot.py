import matplotlib.pyplot as plt
import matplotlib as matplotlib
import csv
import sys
import numpy as np

x1 = []
y1 = []
x_ticks =  ["2*10$^4$","32768","10$^5$","2*10$^5$","10$^6$"]
CDFFile = sys.argv[1];
if len(sys.argv) > 2 and int(sys.argv[2]) == True:
    CDFFile += "_CacheSimAccDist.cdf"
else:
    CDFFile += "_AccDist.cdf"

with open(CDFFile,'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x1.append(int(row[0]))
        y1.append(float(row[1]))

fig, ax = plt.subplots()

ax.semilogx(x1, y1, color = 'black', linewidth = 3, linestyle='solid')
ax.set_xlabel("Access Distance",fontsize=24,labelpad=20)
#ax.set_xticks([20000,32768,100000,200000])
#ax.set_xticklabels(x_ticks)
#ax.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
ax.set_ylabel("CDF",fontsize=24)
ax.set_ylim([0.3,1.0])
plt.xticks(fontsize = 24)
plt.yticks(fontsize=24)
plt.grid(True,color="black")
plt.show()

