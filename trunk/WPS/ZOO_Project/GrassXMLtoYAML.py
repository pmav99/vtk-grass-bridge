# -*- coding: utf-8 -*-
################################################################################
# Author:	Soeren Gebbert
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
import WPS_1_0_0.OGC_WPS_1_0_0
import yaml

class GrassXMLtoZcfg():
    """ Convert a Grass WPS XML file into a ZOO-WPS config file (zcfg)"""
    def __init__(self):
        pass
        
    def setGrassXMLFileName(self,  filename):
        self.__grassXMLFileName = filename
        if not os.path.isfile(self.__grassXMLFileName):
            raise IOError("Unable to open xml file")
            
    def setZcfgFileName(self,  filename):
        self.__zcfgFileName = filename
        self.__output = open(self.__zcfgFileName,  'w')
    
    def __closeOutput(self):
        self.__output.close()
        
    def convert(self):
    
        self.__yam = {}
        """Start the conversion from WPS XML to ZOO-WPS config file zcfg"""
        try:
            doc = WPS_1_0_0.OGC_WPS_1_0_0.CreateFromDocument(file(self.__grassXMLFileName).read())
            
            zcfg = {}

	    zcfg['serviceProvider'] = "test_service"
	    zcfg['serviceType'] = "Python"
	    self.__yam["zcfg"] = zcfg
	    
            if len(doc.ProcessDescription) > 1:
                raise IOError("Only one Process is supported")
            
            for i in doc.ProcessDescription:
	        proc = {}	    
	        proc['processVersion'] = str(i.processVersion)
	        proc['storeSupported'] = bool(i.storeSupported)
	        proc['statusSupported'] = bool(i.statusSupported)
		proc["Identifier"] = str(i.Identifier.value())
	        ita = self.__getTitleAbstract(i)
	        for key in ita.keys():
		    proc[key] = ita[key] 
	        proc["DataInputs"] = self.__getDataInputs(i)
	        proc["ProcessOutputs"] = self.__getProcessOutputs(i)
	        self.__yam["ProcessDescription"] = proc
	        
	    yaml.dump(self.__yam, self.__output, default_flow_style=False)
        except:
            raise
        finally:
            self.__closeOutput()
            
    def __getTitleAbstract(self, element):
        ita = {}
        if element.Title.value() != None:
            ita["Title"] = str(element.Title.value())
        if element.Abstract != None:
            ita["Abstract"] = str(element.Abstract.value())
            
        return ita
        
    def __getDataInputs(self,  process):
        """Write all data inputs into the zcfg file"""
	dataInputs = {}
        for i in process.DataInputs.Input:
            input = {}
            ita = self.__getTitleAbstract(i)        
            for key in ita.keys():
                input[key] = ita[key] 
                
            input["minOccurs"] = int(i.minOccurs)
            input["maxOccurs"] = int(i.maxOccurs)
            
            if i.ComplexData != None:
                input["ComplexData"] = self.__getComplexData(i.ComplexData)
                #self.__writeComplexData(i.ComplexData)
            if i.LiteralData != None:
                input["LiteralData"] = self.__getLiteralData(i.LiteralData)
                #self.__writeLiteralData(i.LiteralData)
                
            dataInputs[str(i.Identifier.value())] = input
            
        return dataInputs
  
    def __getProcessOutputs(self,  process):
        """Write all process outputs into the zcfg file"""
        processOutputs = {}
        for i in process.ProcessOutputs.Output:
            output = {}
            ita = self.__getTitleAbstract(i)        
            for key in ita.keys():
                output[key] = ita[key] 
                
            if i.ComplexOutput != None:
                output["ComplexOutput"] = self.__getComplexData(i.ComplexOutput)
                #self.__writeComplexData(i.ComplexOutput)
            if i.LiteralOutput != None:
                output["ComplexOutput"] = self.__getLiteralData(i.LiteralOutput)
                #self.__writeLiteralData(i.LiteralOutput)
                
            processOutputs[str(i.Identifier.value())] = output
            
        return processOutputs
        
    def __getComplexData(self,  element):
        """Write the complex data into the zcfg file"""
        complexData = {}
        default = {}
        supported = {}

        if element.Default.Format.MimeType != None:
            default["MimeType"] = str(element.Default.Format.MimeType)
        if element.Default.Format.Encoding != None:
            default["Encoding"] = str(element.Default.Format.Encoding)
        if element.Default.Format.Schema != None:
            default["Schema"] = str(element.Default.Format.Schema)
        complexData["Default"] = default
        
        for i in element.Supported.Format:
	  if element.Default.Format.MimeType != None:
	      supported["MimeType"] = str(element.Default.Format.MimeType)
	  if element.Default.Format.Encoding != None:
	      supported["Encoding"] = str(element.Default.Format.Encoding)
	  if element.Default.Format.Schema != None:
	      supported["Schema"] = str(element.Default.Format.Schema)
        complexData["Supported"] = supported
        
        return complexData
        
    def __getLiteralData(self,  element):
        """Write the literal data into the zcfg file"""
        literalData = {}
        allowedValues = []
        if element.DataType != None:
            literalData["DataType"] = str(element.DataType.value())
        if element.AllowedValues != None:
            for i in element.AllowedValues.Value:
                if literalData["DataType"] == "boolean":
		    allowedValues.append(bool(i.value()))
                elif literalData["DataType"] == "inreger":
		    allowedValues.append(int(i.value()))
                elif literalData["DataType"] == "float":
		    allowedValues.append(float(i.value()))
		else:
		    allowedValues.append(str(i.value()))
            literalData["AllowdValues"] = allowedValues
        if element.DefaultValue != None:
	    if literalData["DataType"] == "boolean":
		literalData["DefaultValue"] = bool(element.DefaultValue)
	    elif literalData["DataType"] == "inreger":
		literalData["DefaultValue"] = int(element.DefaultValue)
	    elif literalData["DataType"] == "float":
		literalData["DefaultValue"] = float(element.DefaultValue)
	    else:
		literalData["DefaultValue"] = str(element.DefaultValue)
        else:
            literalData["AnyValue"] = True
            
        return literalData
        # Units are missing

    def __writeLiteralData(self,  element):
        """Write the literal data into the zcfg file"""
        self.__output.write("  <LiteralData>\n")
        if element.DataType != None:
            self.__output.write("    DataType   = " + str(element.DataType.value()) + "\n")
        if element.AllowedValues != None:
            for i in element.AllowedValues.Value:
                self.__output.write("    AllowedValue   = " + str(i.value()) + "\n")
        self.__output.write("    <Default>\n")
        if element.DefaultValue != None:
            self.__output.write("      value = " + str(element.DefaultValue) + "\n")
        self.__output.write("    </Default>\n")
        self.__output.write("  </LiteralData>\n")

def main():
    """The main function which will be called if the script is executed directly"""

    usage = "usage: %prog [-help,--help] --xmlfile inputfile.xml --zcfgfile output.txt]"
    description = "Use %prog to convert Grass 7.0 WPS XML process description files into ZOO-WPS server YAML config files."
    parser = OptionParser(usage=usage, description=description)
    parser.add_option("-x", "--xmlfile", dest="xmlfile", help="The path to the grass WPS input xml file", metavar="FILE")
    parser.add_option("-z", "--zcfgfile", dest="zcfgfile", help="Path to the new created zcfg file", metavar="FILE")

    (options, args) = parser.parse_args()

    if options.xmlfile == None or options.zcfgfile == None:
        parser.print_help()
        parser.error("Booth file names must be provided")

    converter = GrassXMLtoZcfg()
    converter.setGrassXMLFileName(options.xmlfile)
    converter.setZcfgFileName(options.zcfgfile)
    converter.convert()
    
    exit(0)

###############################################################################
if __name__ == "__main__":
    main()
