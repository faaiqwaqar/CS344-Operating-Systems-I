# !/usr/bin/python
# Author: Faaiq Gohar Waqar
# Course: CS344 - Operating Systems I
# Instructor: Braxton Cuneo/ Benjamin Brewster
# Project Goal: When Executed,, Creates 3 Files
# Each file contains 10 random charectors and endline
# Output sent to stdout, will compute a product
import random
###############################################################################
#Write random set of chars into the Files
#Opening of the files
fileOne = open("myfileOne.txt", "w+")
fileTwo = open("myfileTwo.txt", "w+")
fileThree = open("myfileThree.txt", "w+")

#USE str(chr(numberInsertion)) for conversion to ASCII
for x in range(10):
    stringBuffer = random.randint(97,122)
    asciNum = str(chr(stringBuffer))
    fileOne.write(asciNum)

for x in range(10):
    stringBuffer = random.randint(97,122)
    asciNum = str(chr(stringBuffer))
    fileTwo.write(asciNum)

for x in range(10):
    stringBuffer = random.randint(97,122)
    asciNum = str(chr(stringBuffer))
    fileThree.write(asciNum)

#CLose files on intiial
fileOne.close()
fileTwo.close()
fileThree.close()

###############################################################################
#Open files specifically for reading
fileOne = open("myfileOne.txt", "r")
fileTwo = open("myfileTwo.txt", "r")
fileThree = open("myfileThree.txt", "r")

#Assign variabels to store file contents
textOne = fileOne.read()
textTwo = fileTwo.read()
textThree = fileThree.read()

#print file contents to stdout
print textOne
print textTwo
print textThree

#close files
fileOne.close()
fileTwo.close()
fileThree.close()

###############################################################################
#open files to append charectors
fileOne = open("myfileOne.txt", "a")
fileTwo = open("myfileTwo.txt", "a")
fileThree = open("myfileThree.txt", "a")

#append an endline char to the end of each file
fileOne.write("\n")
fileTwo.write("\n")
fileThree.write("\n")

#close the files for manipulation
fileOne.close()
fileTwo.close()
fileThree.close()

###############################################################################
#Create and print the first random number
variableOne = random.randint(1,42)
print variableOne
#Create and print the second random number
variableTwo = random.randint(1,42)
print variableTwo
#Calculate and print the product
variableThree = variableOne * variableTwo
print variableThree
