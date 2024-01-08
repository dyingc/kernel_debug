# GDB paging for large mount of output

How to disable Type <RET> for more, q to quit, c to continue without paging

Certain commands to GDB may produce large amounts of information output to the screen. To help you read all of it, GDB pauses and asks you for input at the end of each page of output.

RET -> See one more page of output
q -> Discard the remaining output
c -> Continue without paging for the rest of the current command

(gdb) set pagination off

Default value is on

Show the current pagination mode
(gdb) show pagination




