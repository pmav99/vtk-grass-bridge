################################################################################
# Author:	Soeren Gebbert
#               Parts of this code are from the great pyWPS from Jachym Cepicky
#
# Copyright (C) 2009 Soeren Gebbert
#
# License:
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

GRASS_LOCATION_NAME = "startLocation"
GRASS_WORK_LOCATION = "workLocation"
GRASS_MAPSET_NAME = "PERMANENT"

# The format of the input file
# [System]
#  WorkDir= temporal created locations and mapsets are put in this directory
#  OutputDir= The output of the grass module is put in tis directory
# [GRASS]
#  GISBASE= the gisbase directory of grass
#  GRASS_ADDON_PATH= path to addon modules
#  GRASS_VERSION= the version of grass
#  Module= the name of the module which should be executed
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
#  Identifier=output.gml
#  MimeType=text/xml
#  Encoding=UTF-8
#  Schema=http://schemas.opengis.net/gml/3.1.0/polygon.xsd
#
#
# Example with multiple LiteralData
"""
[System]
 WorkDir=/tmp
 OutputDir=/tmp
[GRASS]
 GISBASE=/home/soeren/src/grass7.0/grass_trunk/dist.i686-pc-linux-gnu
 GRASS_ADDON_PATH=
 GRASS_VERSION=7.0.svn
 Module=r.contour

[ComplexData]
 Identifier=/tmp/srtm90.tiff
 MimeType=image/tiff
 Encoding=
 Schema=


[LiteralData]
 Identifier=levels
 DataType=double
 Value=50

[LiteralData]
 Identifier=levels
 DataType=double
 Value=100


[LiteralData]
 Identifier=levels
 DataType=double
 Value=200


[LiteralData]
 Identifier=levels
 DataType=double
 Value=300


[ComplexOutput]
 Identifier=output
 MimeType=text/xml
 Encoding=UTF-8
 Schema=http://schemas.opengis.net/gml/3.1.0/polygon.xsd
"""

###############################################################################
###############################################################################
###############################################################################

class ComplexData():
    """This class saves the complex in- and output data
    of a wps execution request"""
    ############################################################################
    def __init__(self, file):
        self.__file = file
        self.identifer = ""
        self.mimeType = ""
        self.__parseFile()

    ############################################################################
    def __parseFile(self):
        for i in range(3):
            string = self.__file.readline()
            if string == "":
                return
            splitstring = string.split('=')
            if len(splitstring) > 1:
                if splitstring[0].upper().find("IDENTIFIER") != -1:
                    self.identifier = splitstring[1].rstrip()
                    print self.identifier
                if splitstring[0].upper().find("MIMETYPE") != -1:
                    self.mimeType = splitstring[1].rstrip()
                    print self.mimeType

###############################################################################
###############################################################################
###############################################################################
    
class ComplexOutput(ComplexData):
    """The same as ComplexData, but used for name convenience"""
    pass

###############################################################################
###############################################################################
###############################################################################

class LiteralData():
    """This class saves the literal in- and output data
    of a wps execution request"""
    ############################################################################
    def __init__(self, file):
        self.__file = file
        self.identifier = ""
        self.value = ""
        self.type = "" #double, integer, string
        self.__parseFile()

    ############################################################################
    def __parseFile(self):
        for i in range(3):
            string = self.__file.readline()
            if string == "":
                return
            splitstring = string.split('=')
            if len(splitstring) > 1:
                if splitstring[0].upper().find("IDENTIFIER") != -1:
                    self.identifier = splitstring[1].rstrip()
                    print self.identifier
                if splitstring[0].upper().find("VALUE") != -1:
                    self.value = splitstring[1].rstrip()
                    print self.value
                if splitstring[0].upper().find("TYPE") != -1:
                    self.type = splitstring[1].rstrip()
                    print self.type

###############################################################################
###############################################################################
###############################################################################

class InputParameter():
    """This class parses and stores the key-value input parameter of
    a wps execusion request"""
    ############################################################################
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

    ############################################################################
    def parseFile(self, filename):
        """Parse the key-value pairs and call the appropriate subroutines and
        classes"""
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

    ############################################################################
    def __parseSystem(self):
        """Parse and store the sytem relevant variables"""
        for i in range(2):
            string = self.__file.readline()
            if string == "":
                return
            splitstring = string.split('=')
            if len(splitstring) > 1:
                if splitstring[0].upper().find("WORKDIR") != -1:
                    self.workDir = splitstring[1].rstrip()
                    print self.workDir
                if splitstring[0].upper().find("OUTPUTDIR") != -1:
                    self.outputDir = splitstring[1].rstrip()
                    print self.outputDir

    ############################################################################
    def __parseGrass(self):
        """Parse and store the grass relevant variables"""
        for i in range(4):
            string = self.__file.readline()
            if string == "":
                return
            splitstring = string.split('=')
            if len(splitstring) > 1:
                if splitstring[0].upper().find("GISBASE") != -1:
                    self.grassGisBase = splitstring[1].rstrip()
                    print self.grassGisBase
                if splitstring[0].upper().find("GRASS_ADDON_PATH") != -1:
                    self.grassAddonPath = splitstring[1].rstrip()
                    print self.grassAddonPath
                if splitstring[0].upper().find("GRASS_VERSION") != -1:
                    self.grassVersion = splitstring[1].rstrip()
                    print self.grassVersion
                if splitstring[0].upper().find("MODULE") != -1:
                    self.grassModule = splitstring[1].rstrip()
                    print self.grassModule


###############################################################################
###############################################################################
###############################################################################

class GrassEnvironment():
    """This class saves and sets grass environment variables"""

    ############################################################################
    def __init__(self):
        self.env = {"GISBASE":"", "GISRC":"", "LD_LIBRARY_PATH":"",\
        "GRASS_ADDON_PATH":"", "GRASS_VERSION":""}

    ############################################################################
    def getEnvVariables(self):
        for key in self.env:
            try:
                self.env[key] = os.getenv(key, self.env[key])
            except:
                raise

    ############################################################################
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
###############################################################################
###############################################################################

class GrassGisRC():
    """This class takes care of the correct creation of the gisrc file"""
    ############################################################################
    def __init__(self):
        self.locationName = GRASS_LOCATION_NAME
        self.mapset = GRASS_MAPSET_NAME
        self.gisdbase = ""
        self.__gisrcFile = ""

    ############################################################################
    def __init__(self, gisdbase, locationName, mapset):
        self.locationName = locationName
        self.mapset = mapset
        self.gisdbase = gisdbase
        self.__gisrcFile = ""
        self.__tmpDir = ""

    ############################################################################
    def rewriteFile(self):
        if self.__gisrcFile != "":
            self.__writeFile()
        else:
            raise IOError

    ############################################################################
    def writeFile(self, tempdir):
        if os.path.isdir(tempdir):
            try:
               self.__gisrcFile = os.path.join(tempdir, "gisrc")
               self.__writeFile()
            except:
                raise

    ############################################################################
    def __writeFile(self):
        try:
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

    ############################################################################
    def getFileName(self):
        return self.__gisrcFile

###############################################################################
###############################################################################
###############################################################################

class GrassWindFile():
    """This class takes care of thr correct creation of grass WIND and
    DEFAULT_WIND files in the start location"""
    ############################################################################
    def __init__(self, gisdbase, location, mapset):
        """ Create the WIND and if needed the DEFAULT_WIND file """
        self.__windFile = ""
        self.__windname = "WIND"

        if mapset == "PERMANENT":
            """If PERMANENT is used as mapset, the DEFAULT_WIND file will
            be created too"""
            self.__windFile = os.path.join(gisdbase, location, mapset, "DEFAULT_WIND" )
            self.__write()

        self.__windFile = os.path.join(gisdbase, location, mapset, "WIND")
        self.__write()

    ############################################################################
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

    ############################################################################
    def getFileName(self):
        return self.__windFile

###############################################################################
###############################################################################
###############################################################################

class GrassModuleStarter():
    """This class does the following:

     The goal is to process the input data in the coordinate system of the input data
     We need to use r.in.gdal/v.in.ogr and v/r.external to import the data into a newly created grass location
     we start the grass module within this location
     We are using r.external.out/r.out.gdal and v.out.ogr to export the result data
     We cleanup after

     This is the execution schema:
     Parse the input parameter and create the parameter map (GISBASE; work dir, ...)
     Create a temporal directory in the work-dir based on temporal directoy creation of python
     Create a temporal location and PERMANENT mapset to execute the ogr and gdal import modules
     * Create the environment for grass (GIS_LOCK, GISRC, GISBASE ...)
     * Write the gisrc file in the PERMANENT directory
     * create the WIND and DEFAULT_WIND file in the PERMANENT directory of the new location
     Now create a new location/mapset with the coordinate system of the first complex input 
     * Use r.in.gdal or v.in.ogr to create the new location without actually importing the map,
       log stdout and stderr of the import modules
     * Rewrite the gisrc with new location name (we work in PERMANENT mapset)
     Link all other maps via r/v.external into the new location, log stdout and stderr
     execute the grass module, log only stderr
     In case raster output should be created, use r.external.out to force the direct creation
       of images output files
     otherwise export the output with v.out.ogr, log stdout and stderr
     remove the temporal directory
     In case an error occured, return an error code and write the error protocoll to stderr
     exit

    """
    ############################################################################
    def __init__(self, inputfile):

        self.inputCounter = 0
        self.outputCounter = 0

        # These maos are used to create the parameter for the grass command
        self.inputMap = {}
        self.outputMap = {}

        self.inputParameter = InputParameter()
        try:
            self.inputParameter.parseFile(inputfile)
        except:
            raise

        # Create a temporal directory for the location and mapset creation
        if self.inputParameter.workDir != "":
            try:
                self.gisdbase = tempfile.mkdtemp(dir=self.inputParameter.workDir)
            except:
                raise
        else:
            try:
                self.gisdbase = tempfile.mkdtemp()
            except:
                raise

        try:
            os.mkdir(os.path.join(self.gisdbase, GRASS_LOCATION_NAME))
            os.mkdir(os.path.join(self.gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME))
        except:
            raise

        self.fullmapsetpath = os.path.join(self.gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME)

        # set the evironment variables for grass (Unix system only)
        try:
            self.__setEnvironment()
        except:
            raise
        
        # gisrc and wind file creation
        try:
            self.gisrc = GrassGisRC(self.gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME)
            self.gisrc.writeFile(tempdir = self.gisdbase)
            self.gisrcfile = self.gisrc.getFileName()
            print self.gisrcfile
        except:
            raise

        try:
            self.wind = GrassWindFile(self.gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME)
            self.windfile = self.wind.getFileName()
            print self.windfile
        except:
            raise

        # Create the new location based on the first input  and import all maps
        try:
            self.__importData()
        except:
            raise

        # start the grass module
        try:
            self.__startGrassModule()
        except:
            raise
        
        # remove the created directory
        try:
            print "Remove ", self.gisdbase
            os.rmdir(self.gisdbase)
        except:
            pass

    ############################################################################
    def __setEnvironment(self):
        # set the grass environment
        self.genv = GrassEnvironment()
        self.genv.env["GIS_LOCK"] = str(os.getpid())
        self.genv.env["GISBASE"] = self.inputParameter.grassGisBase
        self.genv.env["GISRC"] = os.path.join(self.gisdbase, "gisrc")
        self.genv.env["LD_LIBRARY_PATH"] = str(os.path.join(self.genv.env["GISBASE"], "lib"))
        self.genv.env["GRASS_VERSION"] = "7.0.svn"
        self.genv.env["GRASS_ADDON_PATH"] = self.inputParameter.grassAddonPath
        self.genv.env["PATH"] = str(os.path.join(self.genv.env["GISBASE"], "bin") + ":" + os.path.join(self.genv.env["GISBASE"], "scripts"))
        self.genv.setEnvVariables()
        self.genv.getEnvVariables()

    ############################################################################
    def __importData(self):
        print "Import data"

        
        list = self.inputParameter.complexDataList

        # The list must be not empty
        if len(list) == 0:
            raise IOError

        #proc = Popen(["g.region", '-p'])
        #proc.communicate()

        # Create a new location based on the first input map
        self.__createInputLocation(list[0])

        # Rewrite the gisrc file to enable the new created location
        self.gisrc.locationName = GRASS_WORK_LOCATION
        self.gisrc.rewriteFile()

        #proc = Popen(["g.region", '-p'])
        #proc.communicate()
        
        # Link the inputs into the location
        for i in list:
            print i.identifier
            print i.mimeType
            self.__linkInput(i)

            proc = Popen(["r.info", str(self.inputMap[i.identifier])])
            proc.communicate()

    ############################################################################
    def __isRaster(self, input):
        if input.mimeType.upper() == "IMAGE/TIFF":
            print "Raster is TIFF"
            return "TIFF"
        elif input.mimeType.upper() == "IMAGE/PNG":
            print "Raster is PNG"
            return "PNG"
        else:
            return ""

    ############################################################################
    def __isVector(self, input):
        if input.mimeType.upper() == "TEXT/XML" and input.mimeType.upper().find("gml") != -1:
            print "Vector is gml"
            return "GML"
        else:
            return ""

    ############################################################################
    def __createInputLocation(self, input):
        """Creat a new work location based on the input dataset"""
        print "Create new location"
        # TODO: implement correct and meaningful error handling and error messages
        if self.__isRaster(input) != "":
            proc = Popen(["r.in.gdal", "input=" + input.identifier, "location=" + GRASS_WORK_LOCATION , "-ec", "output=undefined"])
            proc.communicate()

            if proc.returncode != 0:
                raise IOError

        # create a new location based on the first input
        elif self.__isVector(input) != "":
            proc = Popen(["v.in.ogr", "input=" + input.identifier, "location=" + GRASS_WORK_LOCATION , "output=undefined"])
            proc.communicate()

            if proc.returncode != 0:
                raise IOError
        else:
            raise IOError

    ############################################################################
    def __linkInput(self, input):
        """Link the input data into a grass work location"""
        print "Link input"
        # TODO: implement correct and meaningful error handling and error messages

        inputName = "input_" + str(self.inputCounter)

        if self.__isRaster(input) != "":
            proc = Popen(["r.external", "input=" + input.identifier, "output=" + inputName])
            proc.communicate()

            if proc.returncode != 0:
                raise IOError

        elif self.__isVector(input) != "":
            proc = Popen(["v.external", "dsn=" + input.identifier, "output=" + inputName])
            proc.communicate()

            if proc.returncode != 0:
                raise IOError
        else:
            raise IOError

        self.inputMap[input.identifier] = inputName
        self.inputCounter += 1

    ############################################################################
    def __importInput(self, input):
        # TODO: implement correct and meaningful error handling and error messages

        inputName = "input_" + str(self.inputCounter)

        # import the data via gdal
        if self.__isRaster(input) != "":
            proc = Popen(["r.in.gdal", "input=" + input.identifier, "output=" + inputName])
            proc.communicate()

            if proc.returncode != 0:
                raise IOError

        # import the data via ogr
        elif self.__isVector(input) != "":
            proc = Popen(["v.in.ogr", "input=" + input.identifier, "output=" + inputName])
            proc.communicate()
            
            if proc.returncode != 0:
                raise IOError
        else:
            raise IOError

        self.inputMap[input.identifier] = inputName
        self.inputCounter += 1

    ############################################################################
    def __startGrassModule(self):
        print "Start GRASS module ", self.inputParameter.grassModule

###############################################################################
###############################################################################
###############################################################################

if __name__ == "__main__":

    starter = GrassModuleStarter("input.txt")
    exit(0)
