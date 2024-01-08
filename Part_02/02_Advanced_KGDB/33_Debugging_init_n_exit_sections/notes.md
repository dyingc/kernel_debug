# Debugging a kernel module init section

To debug the kernel module init section, set a kernel breakpoint in the file kernel/module.c at the place where the kernel (function `do_init_module`) calls the module's init function (`do_one_initcall(mod->init)`)

Note, because we're debugging the kernel source itself, we need to copy the build folder (including all the binaries and source files) from the Target VM to the Dev VM.


## `do_init_module`

This function is defined in `kernel/module.c`:

```C
static noinline int do_init_module(struct module *mod)
{
  int ret = 0;
  struct mod_initfree *freeinit;

  freeinit = kmalloc(sizeof(*freeinit), GFP_KERNEL);
  if (!freeinit) {
    ret = -ENOMEM;
    goto fail;
  }
  freeinit->module_init = mod->init_layout.base;

  /*
   * We want to find out whether @mod uses async during init.  Clear
   * PF_USED_ASYNC.  async_schedule*() will set it.
   */
  current->flags &= ~PF_USED_ASYNC;

  do_mod_ctors(mod);
  /* Start the module */
  if (mod->init != NULL)
    ret = do_one_initcall(mod->init);
  if (ret < 0) {
    goto fail_free_freeinit;
  }
```


## `do_one_initcall`

The `do_one_initcall` is defined in `init/main.c`:

```C
int __init_or_module do_one_initcall(initcall_t fn)
{
  int count = preempt_count();
  char msgbuf[64];
  int ret;

  if (initcall_blacklisted(fn))
    return -EPERM;

  do_trace_initcall_start(fn);
  ret = fn();
  do_trace_initcall_finish(fn, ret);

  msgbuf[0] = 0;

  if (preempt_count() != count) {
    sprintf(msgbuf, "preemption imbalance ");
    preempt_count_set(count);
  }
  if (irqs_disabled()) {
    strlcat(msgbuf, "disabled interrupts ", sizeof(msgbuf));
    local_irq_enable();
  }
  WARN(msgbuf[0], "initcall %pS returned with %s\n", fn, msgbuf);

  add_latent_entropy();
  return ret;
}
```


# Start to debug


## Stop at `do_init_module`

We should add breakpoint right before `do_on_initcall` function, which is line #3582 in our case:

```C
3580   /* Start the module */
3581   if (mod->init != NULL)
3582     ret = do_one_initcall(mod->init);
3583   if (ret < 0) {
3584     goto fail_free_freeinit;
3585   }
```

```bash
(gdb) info function do_init_module
All functions matching regular expression "do_init_module":

File kernel/module.c:
3561:   static int do_init_module(struct module *);
(gdb) b kernel/module.c:3582
Breakpoint 2 at 0xffffffff811526dd: file kernel/module.c, line 3582.
(gdb) c
Continuing.
```

Then, at the Target VM side, try to `insmod` the KM and you'll see the following at the Dev VM's gdb session:

```bash
Thread 999 hit Breakpoint 2, do_init_module (mod=0xffffffffc0495040) at kernel/module.c:3582
3582                    ret = do_one_initcall(mod->init);
(gdb) p mod->init
$1 = (int (*)(void)) 0xffffffffc0293000
```

Note, the `mod->init=0xffffffffc0293000` indicates the address of the module.

Now, we can use the `add-symbol-file` command to add the symbols:

```bash

**Note** if the KM is built by linking more than one object files, it seems we should use `add-symbol-file <mod->core_layout.base>` rather than `add-symbol-file <mod->init>`.


(gdb) remove-symbol-file ~/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/mon_bind.ko
# The above `remove-symbol-file` is to remove the old symbols, if exists
(gdb) add-symbol-file ~/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/mon_bind.ko 0xffffffffc0293000
add symbol table from file "/home/dyingc/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/mon_bind.ko" at
        .text_addr = 0xffffffffc0293000
(y or n) y
Reading symbols from /home/dyingc/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/mon_bind.ko...
```

Note, after the module is fully inserted, you can find this exactly same address from `/sys/module/<KM>/sections.text`.

Thereafter you should then be able to set a breakpoint in the module's init function.

```bash
(gdb) info function kprobe_init
All functions matching regular expression "kprobe_init":

File /home/dyingc/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/kprobe_bind.c:
75:     static int kprobe_init(void);

File kernel/kprobes.c:
2618:   static int debugfs_kprobe_init(void);

File kernel/trace/trace_event_perf.c:
243:    int perf_kprobe_init(struct perf_event *, bool);
(gdb) b /home/dyingc/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/kprobe_bind.c:kprobe_init
warning: (Internal error: pc 0xffffffffffffdffc in read in psymtab, but not in symtab.)

warning: (Internal error: pc 0xffffffffffffdffc in read in psymtab, but not in symtab.)

Breakpoint 5 at 0x3c: /home/dyingc/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/kprobe_bind.c:kprobe_init. (3 locations)
```


## Troubleshooting


### Breakpoint doesn't work - `0x3c` breakpoint


#### Symptom

- The breakpoint is somehow set to `0x3c`:

```bash
(gdb) p mod->core_layout->base
$4 = (int (*)(void)) 0xffffffffc0462000
(gdb) add-symbol-file /home/dyingc/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/kb.ko 0xffffffffc0462000
add symbol table from file "/home/dyingc/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/kb.ko" at
        .text_addr = 0xffffffffc0462000
(y or n) y
Reading symbols from /home/dyingc/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/kb.ko...
(gdb) b /home/dyingc/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/kb.c:kb_init
Breakpoint 5 at 0x3c: file /home/dyingc/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/kb.c, line 7.
```


#### Analysis

- After the module is fully inserted, there's no `.text` section under `/sys/module/<km>/sections` but a .text.unlikely`

```bash
dyingc@ubuntu19:~/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe$ sudo cat /sys/module/kb/sections/.text.unlikely
0xffffffffc0462000
```

This is because, sometimes the Linux kernel tries to `optimize` the code and put some or all your module's code into the `.text.unlikely` section, which is used for code that is unlikely to be executed.

- It's noticed that, for some reason, if we use `__init` for the init function (eg: `static int __init kprobe_init(void)`), we'll hit this issue as well


#### Solution

Change the `Makefile` to indicate `ccflags-y += -Og` or `ccflags-y += -g -O0`

- `ccflags-y += -Og` - not to optimize the code unless it's debug-non-related, to make debug easier
- `ccflags-y += -O0` - not to optimize the code at all
- Remove the **__init** from the signature of the initial function. eg: from `static int __init kprobe_init(void)` to `static int kprobe_init(void)`
- Rebuild the module

```bash
dyingc@ubuntu19:~/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe$ sudo cat /sys/module/kb/sections/.text
0xffffffffc04b4000
```


### Breakpoint doesn't work - KM across multi-C files


#### Symptom

- The breakpoint address is indeed the **mod->init** however, if you check the value inside `/sys/module/<km>/sections/.text`, after the module is loaded, you'll this **mod->init** is different with the value inside `/sys/module/<km>/sections/.text`.


#### Solution

Use the output of `p mod->core_layout.base` for the `add-symbol-file` command.

