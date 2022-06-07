# Python script for setting parameters
import pprint
import sys
import os
sys.path.insert(0, "./utils")
from setInitParams import *
from math import asin, sqrt,sin

DirName = "Res_example"

BoundType = enum("NONE","PERIODIC","OPEN","NEIGHBOUR")

BoundTypeX = [BoundType.OPEN, BoundType.OPEN]
BoundTypeY = BoundType.OPEN

DEBUG = 1

#####

NumCPUs = 1 # number of processors

Dx = 0.05 # step on X
Dy = 0.05 # step on Y

CellsX = 20
CellsY = 20
CellsZ = 10

NumPartPerCell = 2

MaxTime = 1800 # in 1/w_p

DiagDelay2D = 4 # in 1 / w_p
DiagDelay1D = 1 # in 1 / w_p

BUniform = [0., 0.0, 0.0] # in w_c / w_p

n0 = 5.e15 # particles / cm^3


#######################################


Dt =  0.5*min(Dx,Dy)  # time step

MaxTimeStep = int(round(MaxTime/Dt+1))
TimeStepDelayDiag2D = int(round(DiagDelay2D/Dt))
TimeStepDelayDiag1D = int(round(DiagDelay1D/Dt))



PartParams = {} # 

PName="Electrons"

Exist = True
PartDict = {}
PartDict["Charge"] = -1.0
PartDict["Density"] = 1.0
PartDict["Mass"] = 1.0


if Exist:
    setDictParams(PartParams, PName, PartDict)

#####//////////////////////////////
try:
    WorkDir = DirName+"_"
    os.makedirs(WorkDir, exist_ok=True)
    os.system("cp -r ./src " + WorkDir)
    os.system("cp -r ./utils " + WorkDir)
    os.system("cp -r *.py " + WorkDir)
    os.system("cp -r *.sh " + WorkDir)
    os.chdir("./"+WorkDir)
    os.system("pwd")
except BaseException:
    print("Error in work with files!\n")
    exit()


SysParams = []

setParams(SysParams,'NumCPUs',NumCPUs)

setParams(SysParams,'Dx',Dx)
setParams(SysParams,'Dt',Dt)

setParams(SysParams,'CellsX',CellsX)
setParams(SysParams,'CellsY',CellsY)
setParams(SysParams,'CellsZ',CellsZ)
setParams(SysParams,'MaxTime',MaxTime)
setParams(SysParams,'NumPartPerCell',NumPartPerCell)

writeDictParams("Particles","PartParams.cfg",PartParams)

writeParams('SysParams.cfg', SysParams)