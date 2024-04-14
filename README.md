# Rolang Virtual Machine

`RVM` is a VM that runs `Rolang ByteCode`

# Build

```
cmake -B build/
make -C build/
```

The `RVM` executable will be placed in `bin/`.

# Usage

```
RVM -f <file>			# run ByteCode written as text
RVM -b <binary file>		# run ByteCode written as binary (.rbc file extension)
RVM -f <file> -o <ON/OFF>	# convert text ByteCode to binary ByteCode
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

To run this example just type:

```
RVM -f tests/data/simpleTest.txt
```

`RVM` can process and generate `Rolang ByteCode` in the `rbc` binary format. In order to convert the example above to the `rbc` format, use this command:

```
RVM -f tests/data/simpleTest.txt -o ON
```

After executing this line, you will find a new file called `simpleTest.rbc` in the `tests/data/` directory with the following content:

```
0001011100000000000101110000010100000101000010010000000000000100000110010001100100011101
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

The VM have a set of 31 instructions.

```
add
sub
mul
div
inc
dec
ld     <num>
sv     <num>
jmp    <word>
jne    <word>
je     <word>
jz     <word>
jnz    <word>
eq
gr
ls
and
or
nan
xor
not
dup    <num>
call   <word>
ret
pushn  <num>
pushs  <word>
popn   <num>
pops   <word>
new    <num>
del    <word>
hlt
```

Also, labels can be used to indicate where to jump.

# Demo

![GIF](docs/RVM.gif)

# License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
