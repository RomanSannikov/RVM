# Rolang Virtual Machine


#### RVM is a vm that runs Rolang ByteCode


### Instructions:<br>
add<br>
sub	<br>
mul	<br>
div	<br>
ld	var<br>
sv	var<br>
eq<br>
jmp	loc<br>	
jne	loc	<br>
je	loc	<br>
gr<br>
ls<br>
and	<br>	
or	<br>
nan<br>
xor	<br>	
call	loc<br>	
push	val	<br>
pop	val	<br>
new	len, name<br>
del	name<br>
hlt<br>


### Command Line Arguments:<br>
none 		- error, at least one argument should be written<br>
one arg	- filename. By default, RVM try to read the file like it has RBC in there<br>
-f		- then a filename goes<br>
-b		- run a binary representation of Rolang ByteCode<br>
-c		- run Rolang ByteCode<br>


### How it works:<br>
You start the RVM with the given args and it starts scanning the file. First, the RVM scans the whole file and makes tokens out of them. Second, the recognizer part goes. The recognizer tries to recognize tokens and give them a type. Then, the parsing part goes. In that part, the parser decides what to do using the first token. Decisions are made by using the template (decision tree). The decision tree is just a linked list. If the parser finds a pointer to NULL, it throws an error. While the parser breaking the tree up, it makes instruction in a binary representation. There are 3 types of instruction: opcode, opcode + opDef + operand + operand, opcode + loc. opcode = 1 byte, opDef = 1 byte, operand = 2 bytes, loc = 4 bytes. To make the jump instruction, the parser just repeats looking at opcode of the next instruction and skipping right amount of bytes. The parser always tracks the line number. When instruction has been parsed, the parser give the control to the running part of the VM. The RVM throw an error if a program ends without the halt instruction.


### Errors:<br>
Scanner:<br>
Bad File - cannot open the given file.<br>
Tokenizer:<br>
Recognizer:<br>
Bad Token - cannot recognize a token.<br>
Parser:<br>
	Not Instruction - the instruction is not correct.<br>
Executor:<br>
	Bad Instruction - the binary representation of the instruction is not correct. The parser has made a mistake.<br>
