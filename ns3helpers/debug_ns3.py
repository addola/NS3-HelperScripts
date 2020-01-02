#!/usr/bin/env python
#pylint: disable=C0301

## @package ns3_debug
#
# The script <i>debug_ns3.py</i> runs the latest modified ns3 script under $NS3_ROOT_DIR/scratch in debug mode. It will automatically detect whether you're working on Ubuntu or Mac OSX and run either <i>gdb</i> or <i>lldb</i>.
#
#    You have to be at $NS3_ROOT_DIR to run the script.
#
#    You can also pass command line argument. Let's say this script is stored at  $SCRIPT_DIR, then you first go to $NS3_ROOT_DIR and run:
#
#    $NS3_ROOT_DIR> $SCRIPT_DIR/debug_ns3.py -t=30 -n=5 -i=1
#
#    to run the latest modified program under 'scratch' directory. 

import sys
import os
from sys import platform
import sort_dir as sort_dir_obj

def main(argv):
    """ main function
    This function requires <strong>sort_dir.py</strong> to find the latest modified ns3 program under scratch    
    """
    default_program = sort_dir_obj.get_most_recent()
    print ("Debugging NS3 Program : " + default_program)

    # Determine which OS we're running.
    # This uses gdb for Ubuntu, and llvm (lldb) for Mac OSX
    gdb_args = "gdb --args %s"
    lldb_args = "lldb %s"
    if platform == "linux" or platform == "linux2":
        # linux
        debug_args = gdb_args
        print ("You are working on Linux")

    elif platform == "darwin":
        debug_args = lldb_args
        print ("You are working on OSX")
    else:
        print ("Can't tell what OS are you running. Using GDB")
        debug_args = gdb_args


    pwd = os.getcwd()
    mypath = os.getenv('NS3_ROOT_DIR', '~/workarea')

    if not sort_dir_obj.is_ns3_root_dir() :
        print ("Error : Make sure you are in the NS3 root directory")
        sys.exit(1)

    #./waf --run WaveSandbox --command-template="gdb --args %s --x=20 --y=99"
    #./waf --run WaveSandbox --command-template="lldb  %s -- --x=20 --y=99"

    #Running debugger with no argument
    if len(argv) == 0:
        print (pwd + "/waf --run " + default_program + " --command-template=\"" + debug_args+ "\"")
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

        print ("Processing Command: " + command_string)
        os.system(command_string)
        os.environ['NS3_PROGRAM'] = default_program



if __name__ == "__main__":
    main(sys.argv[1:])
