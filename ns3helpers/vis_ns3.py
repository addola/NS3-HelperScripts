#!/usr/bin/env python

'''
Run latest modified program with PyViz. This required PyBindGen support in ns3.
I create an alias "rv" for "run visualized".
'''

import sys
import os
import sort_dir as sort_dir_obj

def main(argv):
    """Main function"""
    default_program = sort_dir_obj.get_most_recent()
    print ("Using PyViz With NS3 Program : " + default_program)


    pwd = os.getcwd()
    mypath = os.getenv('NS3_ROOT_DIR', '~/workarea')

    if not sort_dir_obj.is_ns3_root_dir():
        print ("Error : Make sure you are in the NS3 root directory")
    #if pwd != mypath:
    #    print ("Error : Make sure you are in the NS3 root directory")
    #    sys.exit(1)
    
    if len(argv) == 0:
        command_string = pwd + "/waf --run " + default_program + " --vis"
        print ("Command String: " + command_string )
        os.system(command_string)
    else:
        command_string = pwd + "/waf --run \"" + default_program

        for i in range(0, len(argv)):
            command_string = command_string + " " + argv[i]
        command_string = command_string + "\"" + " --vis"

        print ("Command String: " + command_string)
        os.system(command_string)
        #os.environ['NS3_PROGRAM'] = default_program

if __name__ == "__main__":
    main(sys.argv[1:])
