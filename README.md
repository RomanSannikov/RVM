# Rolang Virtual Machine

`RVM` is a VM that runs Rolang ByteCode

# Build

```
cmake -B build/
make -C build/
```

The `RVM` executable will be placed in `bin/`.

# Usage

```
RVM -f <file>				# run ByteCode written as text
RVM -b <binary file>		# run ByteCode written as binary (.rbc file extension)
RVM -f <file> -o <ON/OFF>	# convert text ByteCode to binary ByteCode
```

You can find test examples in the `tests/data/` directory.

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
```
add
sub
mul
div
inc
dec
ld     <word>
sv     <word>
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
call   <word>
ret
pushn  <num>
pushs  <word>
popn   <num>
pops   <word>
new    <num>, <word>
del    <word>
hlt
<label>
```

# License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
