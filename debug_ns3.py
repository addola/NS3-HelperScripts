#!/usr/bin/env python
#pylint: disable=C0301

import sys
import os
from sys import platform
import sort_dir as sort_dir_obj

def main(argv):
    """ Main Function """
    default_program = sort_dir_obj.get_most_recent()
    print "Debugging NS3 Program : " + default_program

    # Determine which OS we're running.
    # This uses gdb for Ubuntu, and llvm for Mac OSX
    gdb_args = "gdb --args %s"
    lldb_args = "lldb %s"
    if platform == "linux" or platform == "linux2":
        # linux
        debug_args = gdb_args
        print "You are working on Linux"

    elif platform == "darwin":
        debug_args = lldb_args
        print "You are working on OSX"
    else:
        print "Can't tell what OS are you running. Using GDB"
        debug_args = gdb_args


    pwd = os.getcwd()
    mypath = os.getenv('NS3_ROOT_DIR', '~/workarea')

    if pwd != mypath:
        print "Error : Make sure you are in the NS3 root directory"
        sys.exit(1)

    #./waf --run WaveSandbox --command-template="gdb --args %s --x=20 --y=99"
    #./waf --run WaveSandbox --command-template="lldb  %s -- --x=20 --y=99"

    #Running debugger with no argument
    if len(argv) == 0:
        print pwd + "/waf --run " + default_program + " --command-template=\"" + debug_args+ "\""
        os.system(pwd + "/waf --run " + default_program + " --command-template=\"" + debug_args+ "\"")
    else: #Running with gdb and arguments (GDB only for now)
        command_string = pwd + "/waf --run " + default_program + " --command-template=\"" + debug_args

        if debug_args == gdb_args:
            for i in range(0, len(argv)):
                command_string = command_string + " " + argv[i]
        elif debug_args == lldb_args:
            command_string = command_string + " --"
            for i in range(0, len(argv)):
                command_string = command_string + " " + argv[i]
        command_string = command_string + "\""


        #command_string = pwd + "/build/scratch/" + default_program + "/" + default_program + " "
        #for i in range(0, len(argv)):
        #    command_string = command_string + " " + argv[i]
        #command_string = "gdb --args " + command_string

        print "Processing Command: " + command_string
        os.system(command_string)
        os.environ['NS3_PROGRAM'] = default_program



if __name__ == "__main__":
    main(sys.argv[1:])
