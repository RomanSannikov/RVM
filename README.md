# Rolang Virtual Machine


#### RVM is a vm that runs Rolang ByteCode


### Instructions:<br>
add<br>
sub    <br>
mul    <br>
div    <br>
ld    var<br>
sv    var<br>
eq<br>
jmp    loc <br>
jne    loc    <br>
je    loc    <br>
gr<br>
ls<br>
and    
or
nan<br>
xor    <br> 
not<br>
call    loc<br>   
ret <br>
push    val    <br>
pop    val    <br>
new    len, name<br>
del    name<br>
hlt<br>
`<label>`<br>


### Command Line Arguments:<br>
none         - error, at least one argument should be written<br>
one arg    - filename. By default, RVM try to read the file like it has RBC in there<br>
-f        - then a filename goes<br>
-b        - run a binary representation of Rolang ByteCode<br>
-c        - run Rolang ByteCode<br>


### How it works:<br>
You start the RVM with the given args. Basically, it scans the first line, tokenizes it, makes an instruction out of that (parsing part), and save the instruction in the array. In the parsing part, the parser decides what to do using the first token. Decisions are made by using the template (decision tree). The decision tree is just a linked list. If the parser finds a pointer to NULL, it throws an error. While the parser breaking the tree up, it makes instruction in a binary representation. There are 3 types of instruction: opcode, opcode + opDef + operand + operand, opcode + loc. opcode = 1 byte, opDef = 1 byte, operand = 2 bytes, loc = 4 bytes. To make the jump instruction, the parser just repeats looking at opcode of the next instruction and skipping the right amount of bytes. The parser always tracks the line number. When an instruction has been parsed, the parser gives control to the running part of the VM. The RVM throw an error if a program ends without the halt instruction.


### How it works (briefly):<br>
<ol>
    <li>scan a line</li><br>
    <li>tokenize the line</li><br>
    <li>parse the line (it makes an instruction in bin. representation)</li><br>
    <li>save the instruction in the instruction array</li><br>
    <li>if the next line exists, go to the first stage </li><br>
    <li>execute the instructions from the instruction array (the parsing part is likely the same)</li><br>
</ol>

### How the jump instructions work:<br>
The RVM has The Symbole Table and The Label Location Table.<br>
The Symbol Table stores label names and entry points of jmp values (the jmp value goes after the opcode)<br>
The Label Location Tabel stores label names and their location (a label points to the next instruction)<br>
<ol>
    <li>if label</li>
    <li>write label to The Label Location Table</li>
    <li>else if jmp instruction</li>
    <li>write label to The Symbol Table</li>
    <li>when the scanning part is done, go throw The ST and fill up jmp values using The LLT</li>
</ol>

### Logging System:<br>
The RVM uses a logging system that helps to handle exceptions. The LS works in a different thread. In an unusual situation, a function throws an exception, that the LS handles. The LS has two states of handling. The first one is the yellow state. In that state, the LS just writes info in the console. The second one is the red state. In that state, the LS writes info in the console and terminates the program.

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
    End Without HALT - there are no instructions left and the last instruction isn't HALT.
