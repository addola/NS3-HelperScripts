# **NS3-HelperScripts**
These scripts are written to make the processes of running `ns3` scripts and debugging them much easier. They properly format the `./waf` command-string for passing arguments, using a debugger and `PyViz`. 

I created this because I have multiple `ns3` programs under my `scratch` directory and I always have to remember how to write the name of the program correctly, or how do I run my `ns3` program in a debugger.

The scripts work by finding the latest modified file under all directories under `scratch` to use the name of the directory, which is the name of the `ns3` program.

So, if there's a directory under `scratch` named `MyWirelessUDPEchoExample`, it would be run like this:
``` bash
./waf --run MyWirelessUDPEchoExample
```
I written a script in `run_ns3.py`, in which I can run the latest modified project under `scratch` directory. First, I need to make sure I am in the `ns3` root directory (that's the one from which we run `./waf` commands), then use the full path name tothe `run_ns3.py` script. For example if `run_ns3.py` is stored in `~/UnixScripts/run_ns3.py`, then I run
``` bash
~/UnixScripts/run_ns3.py
```
Notice that I didn't need to specify the project name. My script looks for the latest modified project under `scratch`


## **Prerequisites**
--------------------
* These Python scripts were tested with Python 2.7 & 3.6 on Ubuntu 14 to Ubuntu 18 & Mac OS from High Sierra and later.

* When I first designed this, you needed to to create an environment variable called `$NS3_ROOT_DIR` so that the scripts would only run if your current working directory is `ns3`'s root directory from which you run the `waf` tool. *However*, I updated the scripts so that it checks if your current directory is an `ns3` root directory by checking for the presence of certain files and folders.

    To create environment variables, or add aliases edit the file`./bashrc` on Ubuntu, or `./bash_profile` on Mac OS. I defined an environment variable and aliases for my `ns3` directory as follows: navigate to my `ns3` directory.
    ```bash
    export $NS3_ROOT_DIR=/home/adil/NS3Work
    alias gons='cd $NS3_ROOT_DIR'
    ```
*  Under the directory `$NS3_ROOT_DIR/scratch` you *must* only have directories for every project you're working on. The reason for this is the way I designed the code that checks for the latest modified project. It is a file called `sort_dir.py`, and it checks all files in the directory under `scratch` and returns the directory name containing the latest modified project. 

    Let us say I have `Project1`, `Project2`, and `Project3`, then content of `scratch` would look like this:
    ```bash
    ./scratch/
            Project1/
                project1-main.cc
                SomeClass.cc
                SomeClass.h
            Project2/
                project2-main.cc
            Project3/
                project3-main.cc
    ```

* The helper scripts are written in Python in multiple `.py` files. Place all those `.py` files in one directory. On my computer, I created a directory called `~/UnixScripts` for all my helper scripts.

* Additionally, you may create aliases for each of the scripts. I work mainly with three script files, so I created three aliases
    ```bash
    export UNIX_SCRIPTS_DIR='~/UnixScripts'
    alias r=$UNIX_SCRIPTS_DIR/run_ns3.py
    alias dr=$UNIX_SCRIPTS_DIR/debug_ns3.py
    alias rv=$UNIX_SCRIPTS_DIR/vis_ns3.py
    ```
* Scripts were tested to be compatible with `Python 2` & `Python 3`. Support for `Python 2` ends in 2020.


## **Helper Scripts Tutorial**
----
### **`run_ns3.py`**
 * when you call this program, it will run the latest modified ns3 program under `$NS3_HOME_DIR/scratch`
 * To run this script, you need to be in the `ns3` root directory, and you must use the full-path name to the `run_ns3.py` file.
 * This program uses `sort_dir.py` to find the most recently modified program.
    - The command used to check the latest modified project on Ubuntu is different than the one used on Mac OS. The `sort_dir.py` scripts checks which operating system are you running, and use the proper command.
 * If you stored this file at a directory called `~/UnixScripts`, you will call it by running
    ```bash
    python ~/UnixScripts/run_ns3.py
    ```
* alternatively, you can change the file permission to execute it directly:
    ```bash
    > chmod 777 ~/scripts/run_ns3.py
    > ~/scripts/run_ns3.py
    ```
* I made my life easier by creating an alias to run this program. The alias can be anything you want. I called mine `r`
    ```bash
    alias r='~/scripts/run_ns3'
    ```
    - This allows me to run the latest modified project under `scratch` simply by typing `r` to run the latest modified `ns3` program without parameters. You can choose any alias you want for this.
        ```bash
        r
        ``` 
* You can also pass command-line argument to your `ns3` program. You need to create command-line arguments in your `ns3` program using `ns3::CommandLine` interface method. 

    Say you have two arguments to pass, `n=10` and `t=60` then you run it as follows:
    ```bash
    ~/UnixScripts/run_ns3.py --n=10 --t=60
    ```
    Or you can simply use the alias you created.
    ```bash 
   r --n=10 --t=60  
    ```
    If the latest modified project is named `Project2`, then using the scripts that way actually runs the command: 
    ```bash
    ./waf --run "Project2 --n=10 --t=60"
    ```
    As you can see, the script gives us a more compact way to write the `run` commands.
### **`debug_ns3.py`**
* This will run the latest modified `ns3` program under `scratch` directory with a debugger.

* It will use `gdb` if it was ran on Linux and `lldb` if it was ran on Mac OS. It automatically checks for the operating system. It is done this way because of `gdb` compatibility issues on Mac OS.

* I suggest creating an alias for this as well, mine is called `dr`, and is created in `~/.bashrc` as follows:
    ```bash
    alias dr='~/scripts/debug_ns3.py'
    ```
* You can also pass command-line arguments to the `ns3` program just like in `run_ns3.py` example.

* The command runs differently on Linux & Mac OS.

    - *On Mac OS* if we run the command with parameters `n1=4` and `n2=6` like this:
        ```bash
        dr --n1=4 --n2=6
        ```
        The command translates into the proper way to pass `ns3` program arguments to `lldb` which should match this format:
        ```text
        ./waf --run Project2 --command-template="lldb %s -- --n1=4 --n2=6"
        ```
    - *On Linux* the command translates into the proper format to pass arguments using `gdb`
        ```text
        ./waf --run TutorialLesson --command-template="gdb --args %s"
        ```
* I also have scripts named `gdb_ns3.py` & `llvm_ns3.py` to force the simulation to run using `gdb` & `lldb` debuggers respectively in case you wanted to do that.

### **`vis_ns3.py`**
* If you have set up `ns3` with `PyViz` you may run the latest modified program using this command, which will use the `--vis` option with `ns3`.
* I create an alias for this and call it `rv` as follows:
    ```bash
    alias rv='~/scripts/vis_ns3.py'
    ```
* You can also pass command-line arguments to the `ns3` program just like in `run_ns3.py` example.

    If I want to run the latest modified project with `PyViz` with command-line arguments --n1=4 --n2=6
    ```bash
    rv --n1=4 --n2=6
    ```
    If the latest modified program is under `scratch/Project2` this translates to the command
    ```bash
    ./waf --run "TutorialLesson --n1=4 --n2=6" --vis
    ```

## **Questions and Corrections?**
-----
If you have questions, or corrections, or suggestion, contact me at `aalsuha@g.clemson.edu` or `adil.alsuhaim@gmail.com` if I am no longer with Clemson University. 