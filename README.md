# Rolang Virtual Machine

`RVM` is a virtual machine that runs `Rolang ByteCode`

# Build

```
cmake -B build/
make -C build/
```

The `RVM` executable will be placed in `bin/`.

# Usage

```
USAGE:
  RVM [submodule] [options] <file>

SUBMODULES:
  run            run the code from the file (this is a default submodule)
  convert        convert the provided file from one format to another

OPTIONS:
  -b, --binary   hint that the provided file is in the rbc format
  -h, --help     prints help information
```

You can find test examples in the `tests/data/` directory.

The demo is shown down below.

# Example

The provided example demonstrates the `Rolang ByteCode` extracted from `tests/data/simpleTest.txt`
```
pushn 0
pushn 5
loop:
dec
jne loop
popn
popn

hlt
```

This little program is the equivalent of the following code snippet in `C`:

```
for (int i = 5; i != 0; i--) {}
```

To run this example just type the following line (you may want to type `./bin/RVM` instead of just `RVM` since the binary is in the `bin/` directory):

```
RVM -f tests/data/simpleTest.txt
```

`RVM` can process and generate `Rolang ByteCode` in the `rbc` binary format. In order to convert the example above to the `rbc` format, use this command:

```
RVM -f tests/data/simpleTest.txt -o ON
```

After executing this line, you will find a new file called `simpleTest.rbc` in the `tests/data/` directory with the following content:

```
0001100000000000000110000000010100000101000010010000000000000100000110100001101000011101
```

From a technical standpoint, it should be noted that this is not genuine binary code, but rather plain text. This conversion is carried out specifically for the purpose of debugging.

Also, as you can see, the Rolang ByteCode must end with the `hlt` instruction.

# Overview of implementation
**There are 3 types of instructions:**
* `opcode`
* `opcode + opDef + operand + operand`
* `opcode + loc`

**Sizes of each part:**
* `opcode` = 1 byte
* `opDef` = 1 byte
* `operand` = 2 bytes
* `loc` = 4 bytes

**`RVM` steps:**
1. scan a line
2. tokenize the line
3. parse the line (it converts the instruction into binary representation)
4. save the instruction in the instruction array
5. if the next line exists, go to the stage 1.
6. execute the instructions from the instruction array

`RVM` has a `Jump Table` that stores label names, their locations, and the corresponding jump instruction locations. When the parser encounters a label or jump instruction, it writes their locations and names to the `Jump Table`. Once all instructions have been processed, the parsing phase ends by assigning the jump locations to the respective jump calls.

### Rolang ByteCode instructions:

The VM has a set of 30 instructions.

```
add
  sums two top stack numbers

sub
  subtracts two top stack numbers

mul
  multiplies two top stack numbers

div
  divides two top stack numbers

inc
  increments top stack element

dec
  decrements top stack element

ld     <num>
  loads <num>'s variable onto the stack

sv     <num>
  saves <num>'s variable onto the stack

jmp    <word>
  jumps to the label <word>

jne    <word>
  jumps to the label <word> if two top stack elements are not equal

je     <word>
  jumps to the label <word> if two top stack elements are equal

jz     <word>
  jumps to the label <word> if top stack element is zero

jnz    <word>
  jumps to the label <word> if top stack element is not zero

eq
  pushes 1 onto the stack if two top stack elements are equal, otherwise - 0

gr
  pushes 1 onto the stack if top stack element is greater than the second top stack element, otherwise - 0

ls
  pushes 1 onto the stack if top stack element is less than the second top stack element, otherwise - 0

and
  pushes the result of anding two top stack elements onto the stack

or
  pushes the result of oring two top stack elements onto the stack

nand
  pushes the result of nanding two top stack elements onto the stack

xor
  pushes the result of xoring two top stack elements onto the stack

not
  pushes the result of notting two top stack elements onto the stack

dup    <num>
  pushes a duplicate of the top stack element onto the stack

call   <word>
  executes a jump to the ladel <word> with saving the current stack frame to be able to return to the same place

ret
  returns to the last stack frame position (from the last call)

pushn  <num>
  pushes the given number <num>

pushs  <word>
  pushes the given string <word> ending with '\0'

popn
  pops the top number

pops
  pops the top string terminating with '\0' (pops everything until '\0')

new    <num>
  allocates a heap object of the type <num>

hlt
  halts the execution of the program
```

Also, labels can be used to indicate where to jump.

Since the VM is stack based, it has the following approach for taking operands for two-operand operations:

`<operand1> <operation> <operand2>`,
  where `<operand1>` is the top stack element and `<operand2>` is the top stack element after `<operand1>` is popped.

# Demo

![GIF](docs/RVM.gif)

# License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
