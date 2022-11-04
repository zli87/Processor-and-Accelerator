#****************************************************
#
#	04/15/2015 Sharath Pawan P (spendya@ncsu.edu)
#
#****************************************************

import re, sys, collections

filename=sys.argv[1]
f1=open(filename,'r')
#f1=open('dc_shell.log', 'r')

f2 = open('output_warnings.txt', 'w')
f3 = open('output_errors.txt', 'w')
dict1 = {}
error_count = 0

for line in f1:
	m1=re.search(r"Error|error|Errors|errors",line)
	m2=re.search(r"Warning:.*\((.*)\)",line)


	if m1:
		error_count = int(error_count) + 1
	if m2:
		a1 = str(m2.group(1))
		a2 = dict1.get(a1)
		if not a2:
			dict1[a1] = 1
		else:
			dict1[a1] = int(dict1.get(a1)) + 1

dict2 = collections.OrderedDict(sorted(dict1.items()))
for z,x in dict2.items():
	f2.write(str(z))
	f2.write("\t\toccurred\t\t")
	f2.write(str(x))
	f2.write("\t\ttimes\n")

if (int(error_count) > 0):
	f3.write("Error(s) Occurred ")
	f3.write(str(error_count))
	f3.write(" times")
else:
	f3.write("No Errors Detected\n")
