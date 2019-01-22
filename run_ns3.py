#!/usr/bin/env python
'''
Pre-requisites:
===============
1. This was tested on Mac OSX (Sierra) & Ubuntu with Python 2.7 installed.

2. You need to defined $NS3_ROOT_DIR as an environment variable so that it points to the ns3 root directory from whhich you run ./waf
commands. For example:
    ~/Documents/NS3Download/ns-3.26
ns-3.26 has the directories: src, build, etc..
    
3. Your NS3 program(s) should be in a directory under $NS3_ROOT_DIR/scratch. The name of the directory is the name of your program.

Make sure that your program have at least one file that has the main function.
    Clarification:
        ns-3.26 > 
            scratch >
                > NS3Program1
                    > Program.cc                (this can have any name, but must have a main function)
                    > SomeUserDefinedFile.cc    (Optional: if you want some separation of code when you design some C++ classes)
                    > SomeUserDefinedFile.h     (All these will be compiled when you run ./waf )
                > NS3Program2
                    > main.cc 

4. All scripts use sort_dir.py to find the last modified project under scratch directory

5. Make sure the python file permission allow "execution". You can do that by running
    chmod 700 run_ns3.py
otherwise, you will have to run is by typing: "python run_ns3.py" instead of ./run_ns3.py

Usage:
======
1. To run the last program you edited & saved under scratch directory simply run:
    path_to_script/run_ns3.py
    
    - You need to be in the NS3 root directory for this to run.
    - You may place these python scripts in any directory. For example, under $NS3_ROOT_DIR/scripts.BaseException
        + To run the last modified program, we type:
            ./scripts/run_ns3.py
        This execuates the following:
            $NS3_ROOT_DIR/ns-3.26/waf --run LastModifiedProgram

2. You can run it with arguments like:
    ./scripts/run_ns3.py --nCsma=10 --nWifi=10
which runs
    $NS3_ROOT_DIR/ns-3.26/waf --run LastModifiedProgram "--nCsma=10 --nWifi=10"
                

'''
import sys
import os
import sort_dir as sort_dir_obj
from MyColor import MyColor


#Adds Color to the Terminal. 
try:
    from termcolor import colored
    no_color = False
except ImportError, e:
    no_color = True
    pass # module doesn't exist, deal with it.

print_details = 1

def main(argv):
    """Main function"""
    #Get the present working directory
    pwd = os.getcwd()
    #check if NS3_ROOT_DIR is set, otherwise choose ~/workarea as a default directory (will probably be wrong)
    mypath = os.getenv('NS3_ROOT_DIR', '/home/addola/eclipse-workspace/NS3Work')
    
    #If you're not at the NS3 root directory, this program stops. 
    if pwd != mypath:
        print MyColor.RED + "Error : Make sure you are in the NS3 root directory" + MyColor.END
        '''
        if no_color:
            print "Error : Make sure you are in the NS3 root directory"
        else:
            err_str = 'Error : You need to be in the NS3 Root directory' 
            sys.exit(colored(err_str, 'red'))
        '''

    #Get the most recent modified program from $NS3_ROOT_DIR/scratch
    default_program = sort_dir_obj.get_most_recent()

    
    '''
    if no_color:
        print  "Script is running NS3 program : " + default_program   
    else:
        print colored("Script is running NS3 program : " + default_program, 'green') 
    '''
    print MyColor.GREEN + "Scripts is running NS3 Program : " + default_program + MyColor.END

    #If running without arguments...
    if len(argv) == 0:
        #Printing the command string so that users can see if they're not running the intended program.

        if print_details:
            #print "Program is not supplied with parameters"
            print "Command : " + pwd + "/waf --run " + default_program
        #Run the program!
        os.system(pwd + "/waf --run " + default_program)
        #Set NS3_PROGRAM to the last executed program (not needed. This should be removed later.)
        #os.environ['NS3_PROGRAM'] = default_program
    else:
        #Program have more than zero arguments, so we go through them in a loop, and use build a command string.
        #The default command string with a quotation mark added..
        command_string = pwd + "/waf --run \"" + default_program

        #Looping through Command line arguments
        arguments = ""

        for i in range(0, len(argv)):
            #add commandline arguments one by one.
            arguments = arguments + argv[i] + "\n"
            command_string = command_string + " " + argv[i]

        #close the string literal
        command_string = command_string + "\""

        
        #Showing command string before executaion.
        print "Command: " + command_string
        #Run it with parameters!
        os.system(command_string)
        
        #TODO : Delete this
        #Set NS3_PROGRAM to the last executed program (not needed. This should be removed later.)
        #os.environ['NS3_PROGRAM'] = default_program

if __name__ == "__main__":
    main(sys.argv[1:])


