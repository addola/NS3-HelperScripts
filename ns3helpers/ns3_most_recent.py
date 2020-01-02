#!/usr/bin/env python
'''
Simply prints the most recently modified NS3 project
'''
import sort_dir as sort_dir_obj

import sys
import os
import sort_dir as sort_dir_obj
from MyColor import MyColor


#Adds Color to the Terminal. 
try:
    from termcolor import colored
    no_color = False
except ImportError:
    no_color = True
    pass # module doesn't exist, deal with it.


def main(argv):
    """Main function"""
    default_program = sort_dir_obj.get_most_recent()
    print(default_program)

if __name__ == "__main__":
    main(sys.argv[1:])
    
 

