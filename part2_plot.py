import matplotlib.pyplot as plt
import csv
import sys
import numpy as np

x1 = []
y1 = []

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
ax.set_ylabel("CDF",fontsize=24)
plt.xticks(fontsize = 24)
plt.yticks(fontsize=24)
plt.grid(True,color="black")
plt.show()

