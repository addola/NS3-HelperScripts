#!/usr/bin/env python

## 
# Provides facilities to run_ns3.py, debug_ns3.py and vis_ns3.py, so it needs to be in the same directory as those scripts.abs
# valgrind_ns3.py is a work-in-progress.
#
# The main purpose of this script is to get the project name of the latest modified project under $NS3_ROOT_DIR
# Tested on OSX Sierra to Mojave , Ubuntu 16/18
# Update: Now compatible with Python 2 & 3. Tested withPython 2.7 and 3.6
#

import sys
import os
from subprocess import Popen, PIPE, STDOUT
from sys import platform
from MyColor import MyColor

#Adds Color to the Terminal. 
'''
try:
    from termcolor import colored
    no_color = False
except ImportError, e:
    no_color = True
    pass # module doesn't exist, deal with it.
'''

print_details = 0



def print_arr(arr):
    for mystr in arr:
        print (mystr)
    if print_details:
        print ("-------- END OF PRINT ARR --------")



#Removes directories with .git and and ending with ./scratch
def remove_extras(dirs):
    array_size = len(dirs)
    
    if print_details:
        print("array size")
        print(array_size)
    
    ind = 1
    new_list = []
    for filename in dirs:
        if print_details:
            print (str(ind) + " : Examinining : " + filename)
        if not (filename.__contains__('/scratch/.') or filename.endswith('./scratch')):
            new_list.append(filename)
        ind = ind + 1
    return new_list
    if print_details:
        print ("-------- END OF REMOVE EXTRAS --------")
         
def is_ns3_root_dir ():
    '''
    We will perform 'ls' to see if current working directory is for ns3 installation
    '''
    pwd = os.getcwd()
    #Check for the presence of these
    check_list = ['src', 'waf', 'wscript', 'build', 'scratch', 'contrib', 'build']

    cmd = 'ls'
    my_pipe = Popen(cmd, shell=True, stdin=PIPE, stdout=PIPE, stderr=STDOUT, close_fds=True)

    lines = my_pipe.stdout.readlines()
    ls_result = []
    for line in lines:
        if type(line) is str:
            ls_result.append ( line.rstrip('\n') )
        elif type (line) is bytes:
            ls_result.append (line.decode().rstrip('\n'))
    
    my_pipe.kill()
    #For success, the counter has to reach the len (check_list)
    success_counter = 0
    for f in check_list:  #
        for rs in ls_result:
            if f==rs:
                success_counter = success_counter + 1
                break
    return success_counter == len(check_list)
    
def listdir_shell(cmd):
    """lists the most recent changed directory under scratch"""

    if print_details:
        print ("***** PRINTING *****")
        print (cmd)

    my_pipe = Popen(cmd, shell=True, stdin=PIPE, stdout=PIPE, stderr=STDOUT, close_fds=True)
    path = os.getcwd()
    #return p.stdout.read()
    #print ("COMMAND : " + cmd)
    dirlist = []
    lines_read = my_pipe.stdout.readlines() 

    for path in lines_read:
        if type(path) is str: #if using Python2
            pass 
        elif type(path) is bytes: #For Python3
            path = path.decode()
        
        stripped = str(path).rstrip('\n')
        dirlist.append(stripped)


    #dirlist = [path.rstrip('\n') for path in my_pipe.stdout.readlines()]
    if print_details:
        print ("Head of DIRLIST : ")
        print (dirlist[0])

    if dirlist[-1].__contains__('.git'):
        print (MyColor.BOLD + MyColor.YELLOW + "WARNING : ./scratch directory was last modified by git. The last modified project might be incorrect" + MyColor.END +MyColor.END)
        

    dirlist = remove_extras(dirlist)

    if print_details:
        print ("AFTER REMOVAL")
        print_arr(dirlist)

    #print ("DIR LIST IS : ")
    #print (dirlist)

    #remove those with .git

    #if ".git" in dirlist[0].split('/'):
         #most_recent = os.environ['NS3_PROGRAM']
         #return most_recent
    #     print (".git was found.")
    #     print ((dirlist))
    #else:
    if len(dirlist) == 0:
        print (MyColor.RED + "SCRIPT ERROR : Can not determine which project was most recently modified. Has it been moved or deleted?" + MyColor.END)
        sys.exit()

    most_recent = dirlist[-1].split('/')
    
    if print_details:
        print ("MOST RECENTLY MODIFIED PROJECT IS : ")
        print(most_recent[2])

    return most_recent[2]


def get_most_recent():
    """returns the most recent project name under scratch"""
    mypath = os.getenv('NS3_ROOT_DIR', '~/eclipse-workspace/NS3Work')

    ''' Old code
    if os.getcwd() != mypath:
        print ("Error : You need to be at the NS3 directory. Check if $NS3_ROOT_DIR is set")
        #sys.exit(1)
        #os.system("cd " + mypath)
    '''
    if not is_ns3_root_dir(): #if this is NOT ns3 root dir, stop!
        print ("Error : You need to be at the NS3 directory. The directory '" + os.getcwd()+ "' doesn't seem to be an NS3 directory")
        sys.exit(1)
    #This command string works on Mac OSX
    mac_cmd = "find ./scratch -exec stat -f \"%m %N\" {} \\; | sort -n | tail "
   
    #this is the older one that automaticly picks the last element.
    #mac_cmd = "find ./scratch -exec stat -f \"%m %N\" {} \\; | sort -n | tail -1"
    
    #This command string works on Ubuntu.
    #Old Command: find ./scratch/ -type f -exec stat --format '%Y :%y %n' \"{}\" \; | sort -nr | head
    linux_cmd = "find ./scratch/ -type f -exec stat --format '%Y :%y %n' \"{}\" \; | sort -n"

    #Determine if we're running on Mac or Ubuntu
    cmd = ''
    if platform == "linux" or platform == "linux2":
        # linux
        cmd = linux_cmd
    elif platform == "darwin":
        cmd = mac_cmd
    else:
        print ("Can not tell which OS you are using. Going with Linux command")
        cmd = linux_cmd

    #Run the command, and get the more recently modified project. 
    my_list = listdir_shell(cmd)
    
    return my_list



def main(argv):
    """Main method to test things out"""
    print("Tester for scratch directory sorter")

    mac_cmd = "find ./scratch/ -exec stat -f \"%m %N\" {} \\; | sort -n | tail"
    linux_cmd = "find ./scratch/ -type f -exec stat --format '%Y :%y %n' \"{}\" \; | sort -n"

    cmd = ''
    if platform == "linux" or platform == "linux2":
        # linux
        cmd = linux_cmd
    elif platform == "darwin":
        cmd = mac_cmd
    else:
        print ("Can not tell which OS you are using. Going with Linux command")
        cmd = linux_cmd

    #args = "/home/addola/NS3workspace/NS3Work/ns-3.26/scratch -printf '%p\\n' | sort -r | head"
    my_list = listdir_shell(cmd)
    print("CMD : " + cmd)
    print ("List Content is ")
    print (my_list)

    is_ns3_root_dir()

if __name__ == "__main__":
    main(sys.argv[1:])

