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

import os
import os.path

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

