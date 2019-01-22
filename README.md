# **NS3-HelperScripts**
These scripts are written to make the processes of running `ns3` scripts and debugging them much easier. I created this because I have multiple `ns3` programs under my `scratch` directory and I always have to remember how to write the name of the program correctly.

The scripts work by finding the latest modified file under all directories under `scratch` to use the name of the directory, which is the name of the `ns3` program.

So, if there's a directory under `scratch` named `MyWirelessUDPEchoExample`, it would be run like this:
``` bash
./waf --run MyWirelessUDPEchoExample
```
However, if a file under that directory was the latest modified file under the `scratch` directory, then I can run it using the `run_ns3.py` program that I included here.


## **Pre-requisite**
----------------
* You need to create an environment variable for `ns3`'s root directory from which you run the `waf` tool.

    For example if `ns3` root directory is `/home/adil/NS3Work`, then you should add to Inline `.bashrc` the following line:
    ```bash
    export $NS3_ROOT_DIR=/home/adil/NS3Work
    ```
* Your ns3 programs are under the `$NS3_ROOT_DIR/scratch`, with each program in its respective directory.

* Place all the `.py` files in one directory. On my computer, I created a `~/scripts` directory for all my helper scripts.

* Additionally, you may create aliases for each of the programs. I created an alias `gons` that navigates me to my `$NS3_ROOT_DIR` in `~/.bashrc`
    ```bash
        alias gons='cd $NS3_ROOT_DIR'
    ```
    This is useful because you need to be at `$NS3_ROOT_DIR` to run these scripts

* You have to have `Python 2.6` installed. I didn't test these with `Python 3`


## **Helper Scripts Tutorial**
----
### **run_ns3.py**
 * when you call this program, it will run the latest modified ns3 program under `$NS3_HOME_DIR/scratch`
 * This program uses `ns3_most_recent.py` to find the most recent program.
 * If you stored this file at a directory called `~/scripts/run_ns3.py`, you will call it by running
    ```bash
    python ~/scripts/run_ns3.py
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
* You can also pass command-line argument to your `ns3` program. Say you have `n=10` and `t=60` then you run it as follows (and I am using my alias here):
    ``` md 
    /home/adil/NS3Work> r n=10 t=60  
    ```
### **debug_ns3.py**
* This will run the latest modified `ns3` program under `scratch` directory with a debugger.
* It will use `gdb` if it was ran on `Unix` and `lldb` if it was ran on `OSX`. It automatically checks for the operating system.
* I suggest creating an alias for this as well, mine is called `dr`, and is created in `~/.bashrc` as follows:
    ```bash
    alias dr='~/scripts/debug_ns3.py'
    ```
* You can also pass command-line arguments to the `ns3` program just like in `run_ns3.py` example.
### **vis_ns3.py**
* If you have set up `ns3` with `PyViz` you may run the latest modified program using this command, which will use the `--vis` option with `ns3`.
* I create an alias for this and call it `rv` as follows:
    ```bash
    alias rv='~/scripts/vis_ns3.py'
    ```
* You can also pass command-line arguments to the `ns3` program just like in `run_ns3.py` example.

## **Questions and Corrections?**
-----
If you have questions, or corrections, or suggestion, contact me at `aalsuha@g.clemson.edu` or `addola@gmail.com` if I am no longer with Clemson University. 