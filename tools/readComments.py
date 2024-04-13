import os

def printComment(line, file, lineNum):
	print(file + ": " + str(lineNum) + "\t" + line.lstrip(), end = '')

commandLineParameter = ["Todo:", "Fix:"]

# Desc: Find all .hpp and .cpp

print("")

list = os.listdir("./RVM")
for i in range(0, len(list) - 1):
	if(i >= len(list)):
		break
	elif (list[i].find(".cpp") == -1) and (list[i].find(".hpp") == -1):
		list.pop(i)

for i in range(0, len(commandLineParameter)):
	print(commandLineParameter[i])
	for j in list:
		file = open("./RVM/" + j, "r")

		found = False
		readData = file.readline()
		line = 1
		while readData != '':
			if found == True:
				if readData.find("// ") != -1:
					printComment(readData, j, line)
				else:
					found = False
			elif readData.find("// " + commandLineParameter[i]) != -1:
				printComment(readData, j, line)
				found = True
			readData = file.readline()
			line = line + 1

		file.close()
	print("")
