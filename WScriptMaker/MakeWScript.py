#!/usr/bin/env python

## @package wscript_builder
# Builds a wscript python file from a $NS3_ROOT_DIR/contrib/name, where name is the name of the module specified 
# ./MakeWScript.py --name=user
#
# This will create wscript_name and wscript_name_examples
#

import os
from os import walk
import argparse
import sys 

def PrintList(l):
    for item in l:
        print (item) 

def main(argv):

    parser = argparse.ArgumentParser(description='Makes wscript file for a given ns3 library. It requires that you define an environment variable $NS3_ROOT_DIR that points to the root ns3 directory.')
    parser.add_argument('--name', help="Name of ns3 module.", default='user')
    parser.add_argument('-v','--verbose', help="Verbatim output", action='store_true')
    parser.add_argument('-m', '--move', help='Move the created files over to overwrite the current wscript files',action='store_true')

    args = parser.parse_args()

    move_files = args.move

    

    log_details = args.verbose

    files = []
    d = []
    s = []
    h = []

    module_name = args.name

    ns3_root_dir = os.environ['NS3_ROOT_DIR']
    ns3_source_path = ns3_root_dir+'/contrib/'

    module_path =  ns3_source_path + module_name


    examples_source = []
    #Read all Dirs
    is_example = True

    print("PATH : " + module_path)

    for (dirpath, dirnames, filenames) in walk(module_path):
        d.extend(dirnames)
        break
    #Process all subdirs
    for sub_dir in d:
        #Ignore certain directories such as .git, binding and docs
        if sub_dir == '.git' or sub_dir == 'trash' or sub_dir[0] == '.'  or sub_dir == '.trash' or sub_dir == 'bindings' or sub_dir == 'doc':
            if log_details:
                print ("\tIgnored sub-dir: " + sub_dir)
            continue

        #if sub_dir == 'examples':
            
        #Get files for current sub_dir
        for (dirpath, dirnames, filenames) in walk(module_path + '/' + sub_dir):
            files.extend(filenames)
            break

        #Process all files
        for file_name in files:
            if log_details:
                print ("\tFile : " + sub_dir + '/' +  file_name)
            #The 'examples' directory is treated diffrently, as we need to create its own wscript.
            if sub_dir == 'examples':
                if file_name.endswith('.cc'):
                    examples_source.append(file_name)
                
            elif file_name.endswith('.h'):
                header_path = sub_dir + '/' + file_name
                h.append(header_path)
            elif file_name.endswith('.cc'):
                source_path = sub_dir + '/' + file_name
                s.append(source_path)
            else:
                pass
                #print file_name + " is neither header nor source!"

        files = [] #Reset the files

    dependency = ['core', 'network', 'wave', 'wifi', 'applications', 'internet', 'netanim']

    if module_name != 'user':
        dependency.append ('user')

    #'buildings','spectrum', 'stats','wimax','point-to-point','csma'

    #file for wscript
    f = open('wscript_' + module_name, 'w')
    #file for examples if any
    fe = open('wscript_' + module_name + '_examples', 'w')


    if (log_details):
        print ("Created file : " + 'wscript_' + module_name)
        print ("Created file : " + 'wscript_' + module_name + '_examples')
        

    ######### Writing to the module's wscript ###########
    f.write("## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-\n")
    f.write("## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-\n")
    f.write("\n")
    f.write("def build(bld):\n")

    list_of_deps = '\tmodule_dependencies = ' + str(dependency) + '\n'
    #f.write("\tmodule_dependencies = ['wave', 'netanim']\n")
    f.write (list_of_deps)
    #'buildings', 'virtual-net-device','point-to-point' ,'spectrum','csma','core', 'network', 'netanim',  'stats','wifi', 'lte','applications','internet'
    f.write("\tmodule = bld.create_ns3_module('"+ module_name + "', module_dependencies)\n")

    ### Source files

    print ("# of Source files (.cc) : " + str(len(s)) + "\n# of Header files (.h) : " + str(len(h)) )

    sanity_counter = 0
    f.write("\n#Source files\n\n")
    f.write("\tmodule.source = [\n")
    for i in range(len(s)):
        sanity_counter = sanity_counter + 1
        if(i == len(s)-1): #if last element...
            f.write("\t\t'" + s[i] + "'\n")
        else:
            f.write("\t\t'" + s[i] + "',\n")
    f.write("\t\t]\n")
    
    ### Header files
    f.write("\n#Header files\n\n")

    f.write("\theaders = bld(features='ns3header')\n")
    f.write("\theaders.module = '"+module_name+"'\n")
    f.write("\theaders.source = [\n")
    for i in range(len(h)):
        if(i == len(h)-1): #if last element...
            f.write("\t\t'" + h[i] + "'\n")
        else:
            f.write("\t\t'" + h[i] + "',\n")
    f.write("\t\t]\n")

    f.write("\n\n\tif (bld.env['ENABLE_EXAMPLES']):\n")
    f.write("\t\tbld.recurse('examples')\n")
    f.write("\n\t#bld.ns3_python_bindings()")
    f.close()

    ########### create wscript for Examples ###########

    fe.write("# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-\n")
    fe.write('def build(bld):\n')

    for ex in examples_source:
        fe.write('\t')
        fe.write("obj = bld.create_ns3_program('" + ex[0:len(ex)-3] +"',\n")
        fe.write("\t\t['core', 'wave', 'applications', 'mobility', 'network', 'wifi',"+ "'" + module_name + "'])")
        fe.write('\n')
        fe.write("\tobj.source = '" + ex + "'\n\n")
    if len (examples_source) == 0:
        fe.write("\tpass")
    
    if move_files:  #if user chose to move & overwrite the wscript files
        wscript_module = f.name
        wscript_example = fe.name
        #Actually move the generated wscript files to the proper directories. 
        os.system ('mv ' + wscript_module + ' ' + module_path + '/wscript')
        os.system ('mv ' + wscript_example + ' ' + module_path + '/examples/wscript')
        

if __name__ == "__main__":
    main(sys.argv[1:])