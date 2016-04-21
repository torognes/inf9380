
#!/usr/bin/env python
import numpy as np
import pylab

N = 5
menMeans = (20, 35, 30, 35, 27)
menStd =   (2, 3, 4, 1, 2)

ind = np.arange(N)  # the x locations for the groups
width = 0.35       # the width of the bars


pylab.subplot(111)
rects1 = pylab.bar(ind, menMeans, width, color='r', yerr=menStd)

womenMeans = (25, 32, 34, 20, 25)
womenStd =   (3, 5, 2, 3, 3)
rects2 = pylab.bar(ind+width, womenMeans, width, color='y', yerr=womenStd)

# add some
pylab.ylabel('Scores')
pylab.title('Scores by group and gender')
pylab.xticks(ind+width, ('G1', 'G2', 'G3', 'G4', 'G5') )

pylab.legend( (rects1[0], rects2[0]), ('Men', 'Women') )

def autolabel(rects):
    # attach some text labels
    for rect in rects:
        height = rect.get_height()
        pylab.text(rect.get_x()+rect.get_width()/2., 1.05*height, '%d'%int(height),
                ha='center', va='bottom')

autolabel(rects1)
autolabel(rects2)

pylab.show()
