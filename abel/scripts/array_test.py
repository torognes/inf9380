import re
import sys

#Read 3 input arguments
inputfile  = sys.argv[1]
outputfile = sys.argv[2]
taskid = sys.argv[3]

#The third argument should be an integer
#To illustrate how to do different things
#depending on the task

modifier = int(taskid)

 
with open(inputfile, 'r') as f:
 read=""
 for line in f:
   new_v = int(line) + modifier
   read=read+str(new_v)+"\n"
 f.close
 with open(outputfile, 'w') as f2:
  f2.write("Writing from task " + taskid+ "\n")
  f2.write(read)
  f2.close
	
