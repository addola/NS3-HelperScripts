#!/usr/bin/env python

import sys
import os
import sort_dir as sort_dir_obj

def main(argv):
    """Main function"""
    default_program = sort_dir_obj.get_most_recent()
    print "Using PyViz With NS3 Program : " + default_program


    pwd = os.getcwd()
    mypath = os.getenv('NS3_ROOT_DIR', '~/workarea')

    if pwd != mypath:
        print "Error : Make sure you are in the NS3 root directory"
        sys.exit(1)

    if len(argv) == 0:
        os.system(pwd + "/waf --run " + default_program + " --vis")
    else:
        command_string = pwd + "/waf --run \"" + default_program

        for i in range(0, len(argv)):
            command_string = command_string + " " + argv[i]
        command_string = command_string + "\"" + " --vis"

        print "Processing Command: " + command_string
        os.system(command_string)
        os.environ['NS3_PROGRAM'] = default_program

if __name__ == "__main__":
    main(sys.argv[1:])
