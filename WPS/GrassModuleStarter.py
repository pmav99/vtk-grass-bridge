################################################################################
# Author:	Soeren Gebbert
#               Parts of this code are from the great pyWPS from Jachym Cepicky:
#               http://pywps.wald.intevation.org/
#
# Copyright (C) 2009 Soeren Gebbert
#               mail to: soerengebbert <at> googlemail <dot> com
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

import subprocess
import shutil
from optparse import OptionParser
import os
import os.path
import tempfile
from ParameterParser import *
from GrassSettings import *
from ProcessLogging import *
from ErrorHandler import *

GRASS_LOCATION_NAME = "startLocation"
GRASS_WORK_LOCATION = "workLocation"
GRASS_MAPSET_NAME = "PERMANENT"
GRASS_WPS_KEYWORD_LIST = ["grass_resolution_ns", "grass_resolution_ew", "grass_band_number"]

GMS_DEBUG = True


# This module needs an input file for processing. All input and output parameter
# are defined within this file. The file parser expects an input file exactly as
# defined below. All key names must be specified. New-lines between the key names are forbidden.
#
# The format of the input file is defined as:
#
# [System]
#  WorkDir= temporal created locations and mapsets are put in this directory
#  OutputDir= The output of the grass module is put in tis directory
#
# [GRASS]
#  GISBASE= the gisbase directory of grass
#  GRASS_ADDON_PATH= path to addon modules
#  GRASS_VERSION= the version of grass
#  Module= the name of the module which should be executed
#  LOCATION=Name of an existing location with an existing mapset PERMANENT, which should be used for processing, the mapsets are generated temporaly
#  LinkInput=TRUE/FALSE Try to link the input into the generated/existing location, default is TRUE
#  IgnoreProjection=TRUE/FALSE Ignore the projection settings when trying to import the input data (ignored if LinkInput is true), default is FALSE
#  UseXYLocation=TRUE/FALSE create only a XY location/mapset and import all data ignoring the projection information. The resolution will be set based on LiteralData or based on the first input raster resolution, default is FALSE
#
# [ComplexData]
#  Identifier=input
#  MaxOccurs=1
#  PathToFile=/tmp/srtm90.tiff
#  MimeType=text/xml
#  Encoding=UTF-8
#  Schema=http://schemas.opengis.net/gml/3.1.0/polygon.xsd
#
# [LiteralData]
#  Identifier=-p
#  DataType=boolean/integer/double/string
#  Value=true
#
# [ComplexOutput]
#  Identifier=output
#  PathToFile=/tmp/srtm90contour.gml
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
 LOCATION=
 LinkInput=TRUE
 IgnoreProjection=FALSE
 UseXYLocation=FALSE
 
[ComplexData]
 Identifier=input
 MaxOccurs=1
 PathToFile=/tmp/srtm90.tiff
 MimeType=image/tiff
 Encoding=
 Schema=

[LiteralData]
 Identifier=ns_resolution
 DataType=double
 Value=10

[LiteralData]
 Identifier=sw_resolution
 DataType=double
 Value=10

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
 PathToFile=/tmp/srtm90contour.gml
 MimeType=text/xml
 Encoding=UTF-8
 Schema=http://schemas.opengis.net/gml/3.1.0/polygon.xsd
"""

###############################################################################
###############################################################################
###############################################################################

class GrassModuleStarter(ModuleLogging):
    """This class does the following:

     The goal is to process the input data within grass in its own coordinate system without import
       and export

     Main steps are:

     1.) Parse the input file (may be KVP from a WPS execution request)
     2.) Create the new grass location with r.in.gdal or v.in.ogr with the input coordinate system
     3.) Use r.in.gdal/v.in.ogr and v/r.external to import the data into a newly created grass location
     4.) Start the grass module within this location
     5.) Use r.external.out/r.out.gdal and v.out.ogr to export the result data
     6.) Cleanup

     The steps in more detail:
     
     1.) Parse the input parameter and create the parameter map (GISBASE; work dir, ...)
     2.) Create a temporal directory in the work-dir based on temporal directoy creation of python
     3.) Create a temporal location and PERMANENT mapset to execute the ogr and gdal import modules
         * Create the environment for grass (GIS_LOCK, GISRC, GISBASE ...)
         * Write the gisrc file in the PERMANENT directory
         * create the WIND and DEFAULT_WIND file in the PERMANENT directory of the new location
     4.) Now create a new location/mapset with the coordinate system of the first complex input
         * Use r.in.gdal or v.in.ogr to create the new location without actually importing the map,
           log stdout and stderr of the import modules
         * Rewrite the gisrc with new location name (we work in PERMANENT mapset)
     5.) Link all other maps via r/v.external(TODO) into the new location, log stdout and stderr
         or import with r.in.gdal or v.in.org. This can be specified in the input file
     6.) Start the grass module, log stdout and stderr, provide the stdout as file
     7.) In case raster output should be created use r.out.gdal or use r.external.out(TODO) to force the direct creation
         of images output files, otherwise export the output with v.out.ogr, log stdout and stderr
     8.) Remove the temporal directory and exit properly

     In case an error occured, return an error code and write the error protocoll to stderr
     Create meaningful logfiles, so the user will be informed properly what was going wrong
       in case of an error (TODO)

    This python script is based on the latest grass7 svn version
    """
    ############################################################################
    def __init__(self, inputfile, logfile, module_output, module_error):

        self.inputFile = inputfile

        self.inputCounter = 0
        self.outputCounter = 0
        # Handling of multi band raster input files
        self.multipleRasterImport = False
        self.multipleRasterProcessing = False
        self.bandNumber = 0

        # These maps are used to create the parameter for the grass command
        self.inputMap = {}
        self.outputMap = {}

        # the pid of the process which is currently executed, to be used for suspending
        self.runPID = -1

        # Initiate the logfiles
        ModuleLogging.__init__(self, logfile, module_output, module_error)

        # Do all the work
        try:
            self.__run()
        except:
            raise 
        finally:
            self.__closeLogfiles()

    def __run(self):

        # Parse the input parameter
        self.inputParameter = InputParameter(self.logfile)
        try:
            self.inputParameter.parseFile(self.inputFile)
        except:
            log = "Error parsing the input file"
            self.LogError(log)
            raise

        # Create the input parameter of literal data
        self.__createLiteralInputMap()
        # Create the output map for data export
        self.__createOutputMap()
        # Check if multiple import is defined or a single band
        self.__checkBandNumber()

        # In case no band number was provided, we check if the module supports multiple
        # files for each input parameter. If this is not the case, a switch will be set
        # to call the module for each band which was imported
        if self.multipleRasterImport == True:
            try:
                self.__checkModuleForMultipleRasterInputs()
            except:
                log = "Unable to check for multiple raster inputs"
                self.LogError(log)
                raise
            
        # Create a temporal directory for the location and mapset creation
        if self.inputParameter.workDir != "":
            try:
                self.gisdbase = tempfile.mkdtemp(dir=self.inputParameter.workDir)
            except:
                log = "Unable create a temp-directory"
                self.LogError(log)
                raise
        else:
            try:
                self.gisdbase = tempfile.mkdtemp()
            except:
                log = "Unable create a temp-directory"
                self.LogError(log)
                raise

        # Add Error messages
        try:
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
                self.gisrc = GrassGisRC(self.gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME, self.logfile)
                self.gisrc.writeFile(tempdir = self.gisdbase)
                self.gisrcfile = self.gisrc.getFileName()
                self.LogInfo("Created gisrc file " + str(self.gisrcfile))
            except:
                raise

            try:
                self.wind = GrassWindFile(self.gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME, self.logfile)
                self.windfile = self.wind.getFileName()
                self.LogInfo("Created WIND file " + str(self.windfile))
            except:
                raise

            # Create the new location based on the first input and import all maps
            try:
                self.__importData()
            except:
                raise

            # start the grass module one or multiple times, depending on the multiple import parameter
            try:
                self.__startGrassModule()
            except:
                raise

            # now export the results
            try:
                self.__exportOutput()
            except:
                raise
        except:
            raise
        finally:
            self.__removeTempData()

    ############################################################################
    def __checkModuleForMultipleRasterInputs(self):
        """ Check if the grass module supports multiple inputs """
        SingleInputCount = 0
        MultipleInputCount = 0
        self.LogInfo("Check for multiple import")
        for input in self.inputParameter.complexDataList:
            if self.__isRaster(input):
                if(input.maxOccurs > 1):
                    MultipleInputCount += 1
                else:
                    SingleInputCount += 1

        if MultipleInputCount > 0 and SingleInputCount > 0:
            self.multipleRasterProcessing = True
        elif MultipleInputCount == 0 and SingleInputCount > 0:
            self.multipleRasterProcessing = True
        elif MultipleInputCount > 0 and SingleInputCount == 0:
            self.multipleRasterProcessing = False
        else:
            self.multipleRasterProcessing = False
            self.multipleRasterImport = False
            self.bandNumber = 0


    ############################################################################
    def __checkBandNumber(self):
        """ Check if a band number is provided as literal data, if not multiple
        import is enabled even the input file contains only one band """
        self.LogInfo("Check if a band number is present")
        self.multipleRasterImport = True
        self.bandNumber = 0
        for i in self.inputMap:
            if i == "grass_band_number" and self.inputMap[i] != "":
                self.bandNumber = int(self.inputMap[i])
                self.multipleRasterImport = False
                self.LogInfo("Noticed grass_band_number: " + str(self.bandNumber))

    ############################################################################
    def __closeLogfiles(self):
        """ Close all logfiles """
        self.logfile.close()
        self.module_output.close()
        self.module_error.close()

    ############################################################################
    def __removeTempData(self):
        # remove the created directory
        if os.path.isdir(str(self.gisdbase)):
            try:
                self.LogInfo("Remove " + str(self.gisdbase))
                shutil.rmtree(self.gisdbase)
            except:
                log = "Unable to remove temporary grass location " + str(self.gisdbase)
                self.LogError(log)
                raise

    ############################################################################
    def __setEnvironment(self):
        # set the grass environment
        self.genv = GrassEnvironment(self.logfile)
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
    def __runProcess(self, inputlist):
        """This function runs a process and logs the stdout and stderr"""

        try:
            self.LogInfo("Run process: " + str(inputlist))
            proc = subprocess.Popen(args=inputlist, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            self.runPID = proc.pid
            self.LogInfo("Process pid: " + str(self.runPID))
            stdout_buff, stderr_buff = proc.communicate()
            self.LogInfo(stdout_buff)
            self.LogInfo(stderr_buff)
        except:
            self.LogError("Unable to execute process: " + str(inputlist))
            raise

        return proc.returncode, stdout_buff, stderr_buff

    ############################################################################
    def __importData(self):
        """Import all ComplexData inputs which are raster or vector files. Take care of multiple inputs and group generation"""
        list = self.inputParameter.complexDataList

        # The list may be empty in case no input is provided
        if len(list) == 0:
            return

        if GMS_DEBUG:
            parameter = ["g.region", '-p']
            errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

        # Create a new location based on the first input map
        self.__createInputLocation(list[0])

        # Rewrite the gisrc file to enable the new created location
        self.gisrc.locationName = GRASS_WORK_LOCATION
        self.gisrc.rewriteFile()

        # Set the region resolution in case resolution values are provided as literal data
        self.__setRegionResolution()
        
        if GMS_DEBUG:
            parameter = ["g.region", '-p']
            errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

        # In case no band number was provided but the input has only one band, the data will be imported not linked
        if self.inputParameter.linkInput == "FALSE" or self.multipleRasterImport == True or self.multipleRasterProcessing == True:
            for i in list:
                self.__importInput(i)
        else:
            # Link the inputs into the location
            # Linking is only available if a band number was provided
            for i in list:
                self.__linkInput(i)

    ############################################################################
    def __isRaster(self, input):
        """Check for raster input"""
        if input.mimeType.upper() == "IMAGE/TIFF":
            self.LogInfo("Raster map to import is of type TIFF")
            return "GTiff"
        elif input.mimeType.upper() == "IMAGE/PNG":
            self.LogInfo("Raster map to import is of type PNG")
            return "PNG"
        else:
            return ""

    ############################################################################
    def __isVector(self, input):
        """Check for vector input"""
        if input.mimeType.upper() == "TEXT/XML" and input.schema.upper().find("GML") != -1:
            self.LogInfo("Vector map to import is of type GML")
            return "GML"
        else:
            return ""

    ############################################################################
    def __isTextFile(self, input):
        """Check for text file input"""
        if input.mimeType.upper() == "TEXT/PLAIN":
            self.LogInfo("Text file recognized")
            return "TXT"
        else:
            return ""

    ############################################################################
    def __createInputLocation(self, input):
        """Creat a new work location based on an input dataset"""
        
        if self.__isRaster(input) != "":
            parameter = ["r.in.gdal", "input=" + input.pathToFile, "location=" + GRASS_WORK_LOCATION , "-ce", "output=undefined"]
            errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

            if errorid != 0:
                log = "Unable to create input location from input " + str(input.pathToFile)
                self.LogError(log)
                raise GMSError(log)

        elif self.__isVector(input) != "":
            parameter = ["v.in.ogr", "dsn=" + input.pathToFile, "location=" + GRASS_WORK_LOCATION , "-ce", "output=undefined"]
            errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

            if errorid != 0:
                log = "Unable to create input location from input " + str(input.pathToFile)
                self.LogError(log)
                raise GMSError(log)
            
        elif self.__isTextFile(input) != "":
            return
        else:
            log = "Unable to create input location from input " + str(input.pathToFile)
            self.LogError(log)
            raise GMSError(log)


    ############################################################################
    def __setRegionResolution(self):
        # Set the region resolution accordingly to the literal input parameters
        values = 0
        ns = 10.0
        ew = 10.0
        for i in self.inputMap:
            if i == "grass_resolution_ns" and self.inputMap[i] != "":
                self.LogInfo("Noticed grass_resolution_ns")
                values += 1
            if i == "grass_resolution_ew" and self.inputMap[i] != "":
                self.LogInfo("Notices grass_resolution_ew")
                values += 1

            if values == 2:
                proc = subprocess.Popen(["g.region", "ewres=" + str(ew), "nsres=" + str(ns)])
                proc.communicate()
                if proc.returncode != 0:
                    log = "Unable to set the region resolution"
                    self.LogError(log)
                    raise GMSError(log)

    ############################################################################
    def __linkInput(self, input):
        """Link the input data into a grass work location"""
        self.LogInfo("Link input")

        inputName = "input_" + str(self.inputCounter)

        if self.__isRaster(input) != "":
            parameter = ["r.external", "input=" + input.pathToFile, "output=" + inputName]
            if self.inputParameter.ignoreProjection == "TRUE":
                parameter.append("-o")
            if self.bandNumber > 0:
                parameter.append("band=" + str(self.bandNumber))

            errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

            # If the linking fails, import the data with r.in.gdal
            if errorid != 0:
                self.LogInfo("Unable to link the raster map" + input.pathToFile + " try to import.")
                try:
                    self.__importInput(input)
                except:
                    log = "Unable to link or import the raster map " + input.pathToFile + " into the grass mapset"
                    self.LogError(log)
                    raise GMSError(log)
                return

            if GMS_DEBUG:
                parameter = ["r.info", inputName ]
                errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

        elif self.__isVector(input) != "":
            # Linking does not work properly right now for GML -> no random access, so we import the vector data
            self.__importInput(input)
            return
        elif self.__isTextFile(input) != "":
            self.__updateInputMap(input, input.pathToFile)
            return
        else:
            log = "An unexpected error occured while input linking into the grass mapsets, input " + str(input.pathToFile)
            self.LogError(log)
            raise GMSError(log)

        self.__updateInputMap(input, inputName)

    ############################################################################
    def __importInput(self, input):
        # TODO: implement correct and meaningful error handling and error messages

        inputName = "input_" + str(self.inputCounter)

        # import the raster data via gdal
        if self.__isRaster(input) != "":
            parameter = ["r.in.gdal", "input=" + input.pathToFile, "output=" + inputName]
            if self.inputParameter.ignoreProjection == "TRUE":
                parameter.append("-o")
            if self.bandNumber > 0:
                parameter.append("band=" + str(self.bandNumber))
            else:
                # Keep band numbers and create a group
                parameter.append("-k")

            errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

            if errorid != 0:
                log = "Unable to import " + inputName
                self.LogError(log)
                raise GMSError(log)

            # Check if r.in.gdal created a group and put these file names into inputName
            if self.bandNumber == 0:
                inputName = self.__checkForRasterGroup(inputName)

            if GMS_DEBUG:
                for i in inputName.split(','):
                    parameter = ["r.info", i ]
                    errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)
            
        # import the vector data via ogr
        elif self.__isVector(input) != "":
            parameter = ["v.in.ogr", "dsn=" + input.pathToFile, "output=" + inputName]
            errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

            if errorid != 0:
                log = "Unable to import " + inputName
                self.LogError(log)
                raise GMSError(log)

            if GMS_DEBUG:
                parameter = ["v.info", inputName ]
                errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)


        # Text input file, no need to create a new name or for import, use the path as input
        elif self.__isTextFile(input) != "":
            self.__updateInputMap(input, input.pathToFile)
            return
        else:
            log = "Unable to register textfile " + str(input.pathToFile)
            self.LogError(log)
            raise GMSError(log)

        self.__updateInputMap(input, inputName)

    ############################################################################
    def __checkForRasterGroup(self, inputName):
        """This function checks if inputName is a grass group and returns the
        raster map names of the group as single string connected via ,"""
        name = ""

        parameter = ["i.group", "group=" + inputName, "-g"]

        errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

        if errorid != 0:
            # no group was found, maybe stderr_buff should be parsed for keywords?
            # We return the group name, which is the raster map name
            return inputName
        else:
            count = 0
            for i in stdout_buff.split():

                if count > 0:
                    names += "," + str(i)
                else:
                    names = i
                count += 1

        return names

    ############################################################################
    def __updateInputMap(self, input, inputName):
        """ Update the input map and connect the inputs if multiple defined """
        if self.inputMap.has_key(input.identifier):
            self.inputMap[input.identifier] += "," + inputName
        else:
            self.inputMap[input.identifier] = inputName
        self.inputCounter += 1

    ############################################################################
    def __createLiteralInputMap(self):
        """Create the entries of the input map for literal data"""
        list = self.inputParameter.literalDataList

        # The list may be empty
        if len(list) == 0:
            return

        for i in list:
            # Boolean values are unique and have no values eg -p or --o
            if i.type.upper() == "BOOLEAN":
                self.inputMap[i.identifier] = ""
            # Connect the values if multiple defined
            elif self.inputMap.has_key(i.identifier):
                self.inputMap[i.identifier] += "," + i.value
            else:
                self.inputMap[i.identifier] = i.value

    ############################################################################
    def __createOutputMap(self):
        """Create the entries of the output map"""
        list = self.inputParameter.complexOutputList

        # The list may be empty
        if len(list) == 0:
            return

        for i in list:
            outputName = "output_" + str(self.outputCounter)
            # Ignore if multiple defined
            if self.outputMap.has_key(i.identifier):
                pass
            else:
                self.outputMap[i.identifier] = outputName

            self.outputCounter += 1

    ############################################################################
    def __exportOutput(self):
        """Export the output"""
        # TODO: implement correct and meaningful error handling and error messages

        for output in self.inputParameter.complexOutputList:
            outputName = self.outputMap[output.identifier]

            # export the data via gdal
            if self.__isRaster(output) != "":
                parameter = ["r.out.gdal", "-c", "input=" + outputName, "format=" + self.__isRaster(output), "output=" + output.pathToFile]
                errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

                if errorid != 0:
                    log = "Unable to export " + inputName
                    self.LogError(log)
                    raise GMSError(log)

            # export the data via ogr
            elif self.__isVector(output) != "":
                parameter = ["v.out.ogr", "input=" + outputName, "format=" + self.__isVector(output),"dsn=" + output.pathToFile]
                errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

                if errorid != 0:
                    log = "Unable to export " + inputName
                    self.LogError(log)
                    raise GMSError(log)
            else:
                log = "Unable to export " + inputName
                self.LogError(log)
                raise GMSError(log)

    ############################################################################
    def __startGrassModule(self):
        """Create the parameter list and start the grass module"""
        self.LogInfo("Start GRASS module " + str(self.inputParameter.grassModule))
        parameter = []

        parameter.append(self.inputParameter.grassModule)

        for i in self.inputMap:
            # filter the resolution adjustment and the stdout output from the parameter list!
            if i not in GRASS_WPS_KEYWORD_LIST:
                if self.inputMap[i] != "":
                    parameter.append(i + "=" + self.inputMap[i])
                else:
                    parameter.append(i)

        for i in self.outputMap:
            parameter.append(i + "=" + self.outputMap[i])

        errorid, stdout_buff, stderr_buff = self.__runProcess(parameter)

        self.LogModuleStdout(stdout_buff)
        self.LogModuleStderr(stderr_buff)

        if errorid != 0:
            log = "Error while executing the grass module"
            self.LogError(log)
            raise GMSError(log)

###############################################################################
###############################################################################
###############################################################################

def main():
    """The main function which will be called if the script is executed directly"""

    usage = "usage: %prog [-help,--help] --file inputfile.txt [--logfile log.txt] [--module_output mout.txt] [--module_error merror.txt]"
    description = "Use %prog to process geo-data with grass without the need to explicitely " +\
                  "generate a grass location and the import/export of the input and output geo-data. " +\
                  "This may helpful for WPS server or other web services providing grass geo-processing."
    parser = OptionParser(usage=usage, description=description)
    parser.add_option("-f", "--file", dest="filename",
                      help="The path to the input file", metavar="FILE")
    parser.add_option("-l", "--logfile", dest="logfile", default="logfile.txt", \
                      help="The name to the logfile. This file logs everything "\
                      "which happens in this module (import, export, location creation ...).", metavar="FILE")
    parser.add_option("-m", "--module_output", dest="module_output", default="logfile_module_stdout.txt",
                      help="The name to the file logging the messages to stdout "\
                      "of the called grass processing module (textual module output).", metavar="FILE")
    parser.add_option("-e", "--module_error", dest="module_error", default="logfile_module_stderr.txt",\
                      help="The name to the file logging the messages to stderr"\
                      " of the called grass processing module (warnings and errors).", metavar="FILE")

    (options, args) = parser.parse_args()


    if options.filename == None:
        parser.print_help()
        parser.error("A file name must be provided")

    starter = GrassModuleStarter(options.filename, options.logfile, options.module_output, options.module_error)
    exit(0)

###############################################################################
if __name__ == "__main__":
    main()
