# `lx-symbols`

The `lx-symbols` command is particularly important if you want to debug a kernel module. 

Use the `lx-symbols` command to load symbols for **all** modules currently loaded in the kernel.

If debugging an `out-of-tree` module, use the first argument to provide the path of the root directory to search.


# `lx_current`

Examine fields of the current task struct::

```bash
(gdb) set print pretty                                                                                                               
(gdb) set logging file /tmp/aaa                                                                                                   
(gdb) set logging overwrite on
(gdb) set logging on
Copying output to /tmp/aaa.
(gdb) p $lx_current(9) ## List the #9 CPU info
$10 = {                                                                                                                      
  thread_info = {                                                                                                           
    flags = 0,                                                                                                                           
    status = 0                                                                                                                   
  },                                                                                                                                     
  state = 1,                                                                                                                                
  stack = 0xffffc900005e0000,                                                                                                           
  usage = {                                                                                                                              
    refs = {                                                                                                                            
      counter = 1                                                                                                                 
    }                                                                                                                                  
  },                                                                                                                          
  flags = 4194368,                                                                                                               
  ptrace = 0,                                                                                                                                
  wake_entry = {                                                                                                        
    next = 0x0 <fixed_percpu_data>                                                                                               
  },                                                                                                                      
  on_cpu = 1,                                                                                                                          
  cpu = 9,                                                                                                                          
  wakee_flips = 25,
  wakee_flip_decay_ts = 4297968051,
  last_wakee = 0xffff8887f5e09740,
  recent_used_cpu = 9,
  wake_cpu = 9,
  on_rq = 0,
  prio = 120,
  static_prio = 120,
  normal_prio = 120,
  rt_priority = 0,
  sched_class = 0xffffffff8205e8a0 <fair_sched_class>,
... ... ... ...
```
