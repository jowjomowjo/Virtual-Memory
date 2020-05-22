# Virtual-Memory

To compile this assignment with the Makefile, just run: 
make 
 
To recompile, you can just run that command again.  
However, if things get weird (for example, if you get strange liner errors), you can try forcing a complete rebuild of all the source code with: 
make clean all 
 
The program file generated will be named vm-sim.  It takes the name of a memory trace file as an argument, and a few such files are provided for you in the workloads/ directory.  
For example, to run the simulator with the basic trace file, run: 
./vm-sim workloads/basic 
 
The provided trace files are: 
• basic – A basic workload that causes no page faults due to eviction (but pages still fault the first time they are accessed, of course!) 
• eviction – A workload that causes a lot of evictions and should have many page faults 
• tlb – A workload that would cause a lot of TLB hits (except the TLB is not currently implemented in the simulator, so it’s just another generic case for now) 
• everything – A workload that provides a bit of everything for a more exhaustive test 
 
There are also several optional command line options that you can play around with, such as changing the memory size and the page size.  
The defaults are memory size = 16 and page size = 2. 
To view these options, type: 
./vm-sim -h
