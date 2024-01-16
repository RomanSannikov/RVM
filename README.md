
# Rolang Virtual Machine
 
 
RVM is a VM that runs Rolang ByteCode
 
 
### How it works:<br>
You start the RVM with the given args. Basically, it scans the first line, tokenizes it, makes an instruction out of that (parsing part), and save the instruction in the array. There are 3 types of instruction: opcode, opcode + opDef + operand + operand, opcode + loc. opcode = 1 byte, opDef = 1 byte, operand = 2 bytes, loc = 4 bytes. To make the jump instruction, the parser’s gotten the Jump Tabel. When everything is parsed, the parser fills up the values of jump instructions (pointers to labels). The parser always tracks the line number. When all instructions have been parsed, the next step is the running part of the VM.

<ol>
	<li>scan a line</li>
	<li>tokenize the line</li>
	<li>parse the line (it makes an instruction in bin. representation)</li>
	<li>save the instruction in the instruction array</li>
	<li>if the next line exists, go to the first stage </li>
	<li>execute the instructions from the instruction array</li>
</ol>
 
 
### How the jump instructions work: <br>
The RVM has the Jump Tabel. The JT stores label names, their locations, and jump instruction locations. If a label appears, its name and location are written. If a jump appears, its location and the label name are written.<br>


### Tokenizer
Note, the tokenizer ignores punctuation symbols

### Command Line Arguments:<br>
none - error, at least one argument should be written<br>
one arg - filename. By default, RVM try to read the file like it has RBC in there<br>
-f - then a filename goes<br>
-b - run a binary representation of Rolang ByteCode<br>
-o - output Rolang ByteCode<br>

### Supported instructions:<br>
add<br>
sub    <br>
mul    <br>
div    <br>
inc<br>
dec<br>
ld    word<br>
sv    word<br>
jmp    word<br>
jne    word<br>
je     word<br>
jz     word<br>
jnz     word<br>
eq<br>
gr<br>
ls<br>
and<br>
or<br>
nan<br>
xor<br>
not<br>
call   word<br>
ret <br>
pushn  num    <br>
pushs  word    <br>
popn   num    <br>
pops   word    <br>
new    num, word<br>
del    word<br>
hlt<br>
`<label>`<br>
