# Rolang Virtual Machine


#### RVM is a vm that runs Rolang ByteCode


### Instructions:
add	
sub	
mul	
div	
ld	var
sv	var
eq
jmp	loc	
jne	loc	
je	loc	
gr
ls
and		
or	
nan
xor		
call	loc	
push	val	
pop	val	
new	len, name
del	name
hlt


### Command Line Arguments:
none 		- error, at least one argument should be written
one arg	- filename. By default RVM try to read the file like it has RBC in there
-f		- then a filename goes
-b		- run a binary representation of Rolang ByteCode
-c		- run Rolang ByteCode


### How it works:
You start the RVM with the given args and it starts scanning the file. First, the RVM scans the whole file and makes tokens out of them. Second, the recognizer part goes. The recognizer tries to recognize tokens and give them a type. Then, the parsing part goes. In that part, the parser decides what to do using the first token. Decision are made by using the template (decision tree). The decision tree is just a linked list. If the parser finds a pointer to NULL, it throws an error. While it breaking the tree out, it makes an instruction in a binary representation. There are 3 types of instruction: opcode, opcode + opDef + operand + operand, opcode + loc. opcode = 1 byte, opDef = 1 byte, operand = 2 bytes, loc = 4 bytes. To make jump instruction, the parser just repeats looking at opcode of the next instruction and skipping right amount of bytes. The parser always tracks the line number. When an instruction has been parsed, the parser give the control to the running part of the VM. The RVM throw an error if a program ends without the halt instruction.


### Errors:
Scanner:
Bad File - cannot open the given file.
Tokenizer:
Recognizer:
Bad Token - cannot recognize a token.
Parser:
	Not Instruction - the instruction is not correct.
Executor:
	Bad Instruction - the binary representation of the instruction is not correct. The parser has made a mistake.
