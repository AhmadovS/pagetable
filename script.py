#! /usr/bin/env python

marker = open("/u/csc369h/fall/pub/a2-traces/simpleloop", "r")
m = open("/u/csc369h/fall/pub/a2-traces/simpleloop.marker", "r")
ma = m.readline()
start = ma.split()[0]
end = ma.split()[1]
st = marker.readline()
while(st[0]=="="):
	st = marker.readline()
code = []
data = []
temp = []
i = 0
j = 0
k = 0
while (st[0] != "=" and st!="\n"):
	i+=1
	temp = st.strip(",").split()
	if(temp[1].split(",")[0]==start[2:] and j==0):
		j = i
	if(temp[1].split(",")[0]==end[2:] and k==0):
		k = i
	if(temp[0] == "I"):
		if(temp[1].split(",")[0][:5] not in code):
			code.append(temp[1].split(",")[0][:5])
	elif(temp[0] in ['L','S','M']):
		if(temp[1].split(",")[0][:5] not in data):
			data.append(temp[1].split(",")[0][:5])
	st = marker.readline()
	temp = []


print("CODE: " + str(len(code)), "DATA: " + str(len(data)), "WITHIN: " + str((k-j)-1), "OUTSIDE: " + str((i - ((k-j)-1))))

