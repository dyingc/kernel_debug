# Debugging out of tree modules

You need to do to add the debug symbols of the new modules into the `gdb` session:

1. Copy the kernel modules from Target VM to dev VM, at EXACTLY the same folder - better not to use symbolic link
2. Copy the source C files from Target VM to dev VM, at EXACTLY the same folder - better not to use symbolic link
3. Inside the `gdb` remote session, you need to load the new modules that are out of kernel source tree:
  3.1 Option one: `lx-symbols` <FOLDER_of_the_new_kernel_modules> # Note, it's folder, not individual file. All the `.ko` modules in this folder will be loaded
  3.2 Option two: `add-symbol-file <module_FILE_path> 0x<address> # Note, it's individual ABS file path of the `.ko` module. The address can be fetched from the Target VM: `/sys/module/<my_module>/sections/.text` after you do: `sudo insmod <my_module>`


