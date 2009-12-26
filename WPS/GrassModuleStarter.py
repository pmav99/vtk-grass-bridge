# The format of the input file
# [System]
#  WorkDir=/tmp
# [GRASS]
#  GISBASE=
#  GRASS_ADDON_PATH=
#  GRASS_VERSION=
#  Module=v.buffer
#
# [ComplexData]
#  Identifier=input
#  MimeType=text/xml
#  Encoding=UTF-8
#  Schema=http://schemas.opengis.net/gml/3.1.0/polygon.xsd
#
# [LiteralData]
#  Identifier=-p
#  DataType=boolean
#  Value=true
#
# [ComplexOutput]
#  Identifier=input
#  MimeType=text/xml
#  Encoding=UTF-8
#  Schema=http://schemas.opengis.net/gml/3.1.0/polygon.xsd
#
# The goal is to process the input data in the coordinate system of the input data
# We need to use r.in.gdal/v.in.ogr and v/r.external to import the data into a newly created grass location
# we start the grass module within this location
# We are using r.out.gdal and v.out.ogr to export the result data
# We cleanup after 

# This is the execution schema:
# Parse the input parameter and create the parameter map
# Create a temporal directory in the workingdir based on the Name and the PID+TIME of the process
# Create a temporal location to execute the ogr and gdal import modules in the tmp directory
# * Create the environment for grass (MAPSET, LOCATION_NAME and GISBASE)
# * Create the gisrc file in the PERMANENT directory
# * create the DEFAULT_WIND file in the PERMANENT directory of the new location
# Now parse the input options
# The first complex input is used to create a new location based on the
# coordinate system of the input map
# use r.in.gdal or v.in.ogr to create the new location, log stdout and stderr output
# Change the environmentvariables to use the new location
# all other maps are linked via r/v.external into the new location, log stdout and stderr
# execute the grass module, log sterr, only stderr
# export the resulting map(s) with r.out.gdal or v.out.ogr, log stdout and stderr
# remove the temporal directory
# In case an error occured, return an error code and write the error protocoll to stderr
# exit

################################################################################
# Author:	Soeren Gebbert
#               Parts of this code are from the great pyWPS from Jachym Cepicky
# License:
#
# Web Processing Service grass execution script
# Copyright (C) 2009 Soeren Gebbert
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

from subprocess import Popen
import os
import os.path
import tempfile

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__="soeren"
__date__ ="$18.12.2009 17:13:21$"

GRASS_LOCATION_NAME = "startLocation"
GRASS_WORK_LOCATION = "workLocation"
GRASS_MAPSET_NAME = "PERMANENT"

###############################################################################

class ComplexData():
    """This class saves the complex in- and output data
    of a wps execution request"""
    def __init__(self, file):
        self.__file = file
        self.identifer = ""
        self.mimeType = ""
        self.__parseFile()
    def __parseFile(self):
        for i in range(3):
            string = self.__file.readline()
            if string == "":
                return
            splitstring = string.split('=')
            if len(splitstring) > 1:
                if splitstring[0].upper().find("IDENTIFIER") != -1:
                    self.identifier = splitstring[1]
                    print self.identifier
                if splitstring[0].upper().find("MIMETYPE") != -1:
                    self.mimeType = splitstring[1]
                    print self.mimeType

    
class ComplexOutput(ComplexData):
    pass

###############################################################################

class LiteralData():
    """This class saves the literal in- and output data
    of a wps execution request"""
    def __init__(self, file):
        self.__file = file
        self.identifier = ""
        self.value = ""
        self.type = "" #double, integer, string
        self.__parseFile()
    def __parseFile(self):
        for i in range(3):
            string = self.__file.readline()
            if string == "":
                return
            splitstring = string.split('=')
            if len(splitstring) > 1:
                if splitstring[0].upper().find("IDENTIFIER") != -1:
                    self.identifier = splitstring[1]
                    print self.identifier
                if splitstring[0].upper().find("VALUE") != -1:
                    self.value = splitstring[1]
                    print self.value
                if splitstring[0].upper().find("TYPE") != -1:
                    self.type = splitstring[1]
                    print self.type

###############################################################################

class InputParameter():
    """This class parses and stores the key-value input parameter"""
    def __init__(self):
        self.workDir = ""
        self.grassGisBase = ""
        self.grassAddonPath = ""
        self.grassVersion = ""
        self.grassModule = ""
        self.complexDataList = []
        self.complexOutputList = []
        self.literalDataList = []
        self.__fileName = ""

    def parseFile(self, filename):
        self.__filename = filename

        if os.path.isfile(filename) == False:
            raise IOError

        self.__file = open(filename, 'r')

        while True:
            string = self.__file.readline()
            if string == "":
                break

            if string.upper().find("[SYSTEM]") != -1:
                print string.upper()
                self.__parseSystem()

            if string.upper().find("[GRASS]") != -1:
                print string.upper()
                self.__parseGrass()

            if string.upper().find("[COMPLEXDATA]") != -1:
                print string.upper()
                self.complexDataList.append(ComplexData(self.__file))

            if string.upper().find("[COMPLEXOUTPUT]") != -1:
                print string.upper()
                self.complexOutputList.append(ComplexOutput(self.__file))

            if string.upper().find("[LITERALDATA]") != -1:
                print string.upper()
                self.literalDataList.append(LiteralData(self.__file))

    def __parseSystem(self):
        string = self.__file.readline()
        if string == "":
            return
        splitstring = string.split('=')
        if len(splitstring) > 1:
            self.workDir = splitstring[1]
            print self.workDir

    def __parseGrass(self):
        for i in range(3):
            string = self.__file.readline()
            if string == "":
                return
            splitstring = string.split('=')
            if len(splitstring) > 1:
                if splitstring[0].upper().find("GISBASE") != -1:
                    self.grassGisBase = splitstring[1]
                    print self.grassGisBase
                if splitstring[0].upper().find("GRASS_ADDON_PATH") != -1:
                    self.grassAddonPath = splitstring[1]
                    print self.grassAddonPath
                if splitstring[0].upper().find("GRASS_VERSION") != -1:
                    self.grassVersion = splitstring[1]
                    print self.grassVersion
                if splitstring[0].upper().find("MODULE") != -1:
                    self.grassModule = splitstring[1]
                    print self.grassModule


###############################################################################

class GrassEnvironment():
    """This class saves and sets the grass environment variables"""
    def __init__(self):
        self.env = {"GISBASE":"", "GISRC":"", "LD_LIBRARY_PATH":"",\
        "GRASS_ADDON_PATH":"", "GRASS_VERSION":""}
    def getEnvVariables(self):
        for key in self.env:
            try:
                self.env[key] = os.getenv(key, self.env[key])
            except:
                raise
    def setEnvVariables(self):
        for key in self.env:
            try:
                value = self.env[key]
                origValue = os.getenv(key)
                if origValue:
                    value  += ":"+origValue
                os.putenv(key,value)
                os.environ[key] = value
            except:
                raise

###############################################################################

class GrassGisRC():
    """This class takes care of the correct creation of the gisrc file"""
    def __init__(self):
        self.locationName = GRASS_LOCATION_NAME
        self.mapset = GRASS_MAPSET_NAME
        self.gisdbase = ""
        self.__gisrcFile = ""
    def __init__(self, gisdbase, locationName, mapset):
        self.locationName = locationName
        self.mapset = mapset
        self.gisdbase = gisdbase
        self.__gisrcFile = ""
    def writeFile(self, tempdir):
        if os.path.isdir(tempdir):
            try:
                self.__gisrcFile = os.path.join(tempdir, "gisrc")
                gisrc = open(self.__gisrcFile, 'w')
                gisrc.write("LOCATION_NAME: %s\n" % self.locationName)
                gisrc.write("MAPSET: %s\n" % self.mapset)
                gisrc.write("DIGITIZER: none\n")
                gisrc.write("GISDBASE: %s\n" % self.gisdbase)
                gisrc.write("OVERWRITE: 1\n")
                gisrc.write("GRASS_GUI: text\n")
                gisrc.close()
            except:
                raise
    def getFileName(self):
        return self.__gisrcFile

###############################################################################

class GrassWindFile():
    def __init__(self):
        self.__windFile = ""
        self.__windname = "WIND"
    def writeFile(self,gisdbase, location, mapset):
        """ Create default WIND file """

        if mapset == "PERMANENT":
            self.__windFile = os.path.join(gisdbase, location, mapset, "DEFAULT_WIND" )
            self.__write()

        """Create the normal wind file too"""
        self.__windFile = os.path.join(gisdbase, location, mapset, "WIND")
        self.__write()

    def __write(self):
        try:
            wind =open(self.__windFile,'w')
            wind.write("""proj:       0\n""")
            wind.write("""zone:       0\n""")
            wind.write("""north:      100\n""")
            wind.write("""south:      0\n""")
            wind.write("""east:       100\n""")
            wind.write("""west:       0\n""")
            wind.write("""cols:       100\n""")
            wind.write("""rows:       100\n""")
            wind.write("""e-w resol:  1\n""")
            wind.write("""n-s resol:  1\n""")
            wind.close()
        except:
            raise
    def getFileName(self):
        return self.__windFile


if __name__ == "__main__":

    input = InputParameter()
    input.parseFile("input.txt")

    exit()

    workDir = input.workDir

    # create a temporal directory for the location and mapset creation
    gisdbase = tempfile.mkdtemp(workDir)

    os.mkdir(os.path.join(gisdbase, GRASS_LOCATION_NAME))
    os.mkdir(os.path.join(gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME))

    fullmapsetpath = os.path.join(gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME)

    # set the grass environment
    genv = GrassEnvironment()
    genv.env["GIS_LOCK"] = str(os.getpid())
    genv.env["GISBASE"] = "/home/soeren/src/grass7.0/grass_trunk/dist.i686-pc-linux-gnu"
    genv.env["GISRC"] = os.path.join(gisdbase, "gisrc")
    genv.env["LD_LIBRARY_PATH"] = str(os.path.join(genv.env["GISBASE"], "lib"))
    genv.env["GRASS_VERSION"] = "7.0.svn"
    genv.env["GRASS_ADDON_PATH"] = ""
    genv.env["PATH"] = str(os.path.join(genv.env["GISBASE"], "bin") + ":" + os.path.join(genv.env["GISBASE"], "scripts"))
    genv.setEnvVariables()
    genv.getEnvVariables()

    gisrc = GrassGisRC(gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME)
    gisrc.writeFile(gisdbase)
    gisrcfile = gisrc.getFileName()
    print gisrcfile

    wind = GrassWindFile()
    wind.writeFile(gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME)
    windfile = wind.getFileName()
    print windfile

    #proc = Popen(["g.version", "-c"])
    proc = Popen(["r.random.surface", 'output=test'])
    proc.communicate()
    proc = Popen(["r.info", 'map=test', '-m'])
    proc.communicate()

    # create a new location based on a input tiff map
    proc = Popen(["r.in.gdal", "input=/tmp/srtm90.tiff", "location=" + GRASS_WORK_LOCATION , "-ec", "output=test"])
    proc.communicate()

    # Rewrite the gisrc file to enable the new created location
    gisrc = GrassGisRC(gisdbase, GRASS_WORK_LOCATION, GRASS_MAPSET_NAME)
    gisrc.writeFile(gisdbase)
    gisrcfile = gisrc.getFileName()
    print gisrcfile

    # Link the import file
    proc = Popen(["r.external", 'title=test', 'input=/tmp/srtm90.tiff', 'output=test'])
    proc.communicate()

    proc = Popen(["r.info", 'map=test', '-m'])
    proc.communicate()
    proc = Popen(["g.region", '-p'])
    proc.communicate()

    proc = Popen(["r.external", 'title=test2', 'input=/tmp/srtm90.tiff', 'output=test2'])
    proc.communicate()

    proc = Popen(["r.info", 'map=test2', '-m'])
    proc.communicate()

    proc = Popen(["r.slope.aspect", 'elevation=test2', 'slope=slope', 'aspect=aspect'])
    proc.communicate()

    proc = Popen(["r.info", 'map=slope', '-m'])
    proc.communicate()
    proc = Popen(["r.info", 'map=aspect', '-m'])
    proc.communicate()
    # remove the created directory
    try:
        os.rmdir(gisdbase)
    except:
        pass