# Day 24

I solved today's problem by hand on a sheet of paper after examining the input.

## Input Analysis

From the given input we can see that the instructions can be split into 14
chunks, one for each digit.  Each chunk is essentially the same except with
different constants for some of the instructions.  The z variable is essentially
used as a stack where the input digit plus some constant are stored as a base 26
integer.  So in order for z to be zero by the end, said stack must be empty.

Seven of the chunks contain a pop operation on the stack.  Every chunk also
contains a conditional statement checking whether the top of the stack plus some
contstant is equal to the input value of the chunk which is always stored in w.
Should the input equal this value nothing gets pushed to the stack within that
chunk.  As such in each chunk with a pop operation this condition must be
satisfied so that the stack is empty by the end of execution.

By examining the code I was able to determine which input values were related to
each other.  This resulted in me obtaining the following equations for my input:

```
w4 = w3 + 8
w5 = w2 - 2
w7 = w6 - 8
w8 = w1 + 2
w11 = w10 - 6
w13 = w12 - 1
w14 = w9 - 4
```

From these equations I extracted the following contraints on the input values:

```
w1 = [1..7]
w2 = [3..9]
w3 = 1
w4 = 9
w5 = [1..7]
w6 = 9
w7 = 1
w8 = [3..9]
w9 = [5..9]
w10 = [7..9]
w11 = [1..3]
w12 = [2..9]
w13 = [1..8]
w14 = [1..5]
```

## Solution

From the above contraints I could easily obtain my solutions for both part 1 and
2.  These being 79197919993985 and 13191913571211 respectively.
