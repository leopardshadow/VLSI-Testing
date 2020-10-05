import re

file_obj = open("allPathCount.txt",'r')

lines = file_obj.readlines()

maxCnt = 100000

for line in lines:
	m = re.findall(" (\d*)$", line)
	if len(m) > 0:
		cnt = int(m[0])
		if cnt > 10000:
			maxCnt = min(maxCnt, cnt-10000)

print(maxCnt)
