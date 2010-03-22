################################################################################
# Author:	Soeren Gebbert
#
# Copyright (C) 2010 Soeren Gebbert
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

class ProcessLogging():
    """This class initiates the logging mechanism and is the base class for all
    other classes, which ahve information to be logged.
    """

    ############################################################################
    def __init__(self, logfile):
        self.logfile = logfile

    def LogInfo(self, warning_message):
        print "Not jet implemented"

    def LogWarning(self, warning_message):
        print "Not jet implemented"

    def LogError(self, error_message):
        print "Not jet implemented"


class ModuleLogging(ProcessLogging):
    """This class initiates the logging mechanism for the called grass module.
    """

    ############################################################################
    def __init__(self, logfile, module_output, module_error):
        ProcessLogging.__init__(self, logfile)

        self.module_output = module_output
        self.module_error = module_error

    def LogModuleError(self, error_message):
        print "Not jet implemented"

    def LogModuleOutput(self, output_message):
        print "Not jet implemented"
