#
#  Program: vtkGRASSBridge
#  COPYRIGHT: (C) 2009 by Soeren Gebbert, soerengebbert@googlemail.com
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; version 2 of the License.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.



import unittest
import os
from libvtkCommonPython import *
from libvtkGRASSBridgeCommonPython import *


class GRASSModuleTest(unittest.TestCase):
    def testSmoke(self):

        init = vtkGRASSInit()
        init.Init("GRASSRegionTest")
        init.ExitOnErrorOn()

	module = vtkGRASSModule()
	module.SetDescription("Test my module description")
	module.SetLabel("label")
	module.AddKeyword("Test")
	module.AddKeyword("raster")

	flag = vtkGRASSFlag()
	flag.SetDescription("This is a flag")
	flag.SetKey('f')
	flag.SetGuiSection("Flags")

	option = vtkGRASSOptions()
	option.SetKey("option")
	option.SetDescription("This is an option")
	option.SetGuiSection("Options")
	option.SetGisprompt("old,raster,cell")
	option.RequiredOff()
	option.MultipleOff()
	option.SetTypeToInteger()

	init.Parser("test")

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSModuleTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

