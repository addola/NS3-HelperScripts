Disclaimer
==========
This code is provided as-is, with no guarantee. Try to understand the `python` code, and read these instructions carefully.

Prerequisite
============
1. Create an environment variable named `$NS3_ROOT_DIR` in your Linux operating system. The environment variable should point to your `ns-3` root directory. 
For example, on my computer, I created the environment variable as follows:
```
export NS3_ROOT_DIR="/home/adil/eclipse-workspace/ns-3.30.1-eclipse/ns-3.30.1"
```
2. Your user-created modules should be created in a directory under `$NS3_ROOT_DIR/contrib` 


How it works
============
1. Create a module as a folder under `$NS3_ROOT_DIR/contrib`, let's call it `user`. Do not place C++ header and code files directly under `user` directory, put them in subfolders. For example, create folders named `model`, `helper` and `examples`. This scripts scans those subfolder and generate a `wscript` file written in `python`.
    * If you have a folder named `examples`, it will treated differently.

2. Run this script with `--name=user` to scan the `$NS3_ROOT_DIR/contrib/user` folder. This will create two files named `wscript_user` and `wscript_user_examples`.  
```
./MakeWscript.py --name=user
```

3. Copy the generated `wscript_user` to `$NS3_ROOT_DIR/contrib/user/wscript` and the `wscript_examples` to `$NS3_ROOT_DIR/contrib/user/wscript`.

    * Supplying the `--m` argument make it automatically copy the `wscript` files to their destination

4. You can modify the resulting `wscript` files by modifying the modules they depend on, or to comment out whether you want to build examples. 


Usage in your code
===================
1. In your `ns-3` you need to run `./waf --configure` to scan for changes under `contrib` directory.
    * This will create a `.h` file named `modulename-module.h`. For example, if our module is named `user`, it will create a file named `user-module.h` that includes all the header files in your module. 

2. To include your modules from your `ns-3` simulations, simply perform an include of the `user-module.h` file
    ```
    #include "ns3/user-module.h"
    ```


Questions
=========
For any questions you can contact me `aalsuha@clemson.edu` or `adil.alsuhaim@gmail.com`