#!/usr/bin/env python
#pylint: disable=C0301
'''
@package ns3_lldb
This scrpits runs ns3 program with lldb, allowing for command line arguments. This is designed to work on Mac OSX

This is to use directly to force run the ns3 program using lldb specifically.
'''


import sys
import os
from sys import platform
import sort_dir as sort_dir_obj

def main(argv):
    """ Main Function """
    default_program = sort_dir_obj.get_most_recent()
    print ("Debugging NS3 Program with LLDB : " + default_program)

    # Determine which OS we're running.
    # This uses gdb for Ubuntu, and llvm for Mac OSX
    debug_args = "lldb %s"
    
    pwd = os.getcwd()
    mypath = os.getenv('NS3_ROOT_DIR', '~/workarea')

    if not sort_dir_obj.is_ns3_root_dir():
        print ("Error : Make sure you are in the NS3 root directory")
        sys.exit(1)

    if len(argv) == 0:  
        print (pwd + "/waf --run " + default_program + " --command-template=\"" + debug_args+ "\"")
        os.system(pwd + "/waf --run " + default_program + " --command-template=\"" + debug_args+ "\"")
    else: #Running with gdb and arguments (GDB only for now)
        command_string = pwd + "/waf --run " + default_program + " --command-template=\"" + debug_args + " --"

        for i in range(0, len(argv)):
                command_string = command_string + " " + argv[i]
        command_string = command_string + "\""

        print ("Processing Command: " + command_string)
        os.system(command_string)

if __name__ == "__main__":
    main(sys.argv[1:])
