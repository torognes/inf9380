import pylab as pl

f = open('csc_usage.dat')
f.readline() # read the header
# read the data into a dictionary
usage = {}
for line in f:
    line = line.split()
    usage[line[0]] = float(line[1])
f.close()

labels = usage.keys()
fracs = usage.values()
# find out the largest fraction and explode that wedge
explode = [0.0 for f in fracs]
maxindex = fracs.index(max(fracs))
explode[maxindex] = 0.05
# make the actual plot
pl.pie(fracs, explode=explode, labels=labels, autopct='%3.1f')
pl.show()
