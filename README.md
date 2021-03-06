# Compiler Design
__Name:__ Matthew Lesko-Krleza
__Student ID:__ 260692352<br>
## Assignment 1: Scanner+Parser
References used: https://github.com/meyerd/flex-bison-example (for more lexing and parsing examples), https://github.com/comp520/Examples/tree/master/flex%2Bbison/scanner%2Bparser, and the bison and flex man pages.<br>
## Assignment 2: AST, Symbol Table, Typechecking, and Codegen
References used: I used the JOOS flex+bison implementation https://github.com/comp520/JOOS/tree/master/flex%2Bbison as a reference to see how to create the AST, create the Symbol Table, and to generate code. However, instead of weaving in AST references for each respective symbol in the symbol table, I just stored the type directly into an AST node.<br>

## Notes
* `programs`: Test programs are organized by compilation phase and by expected result. Valid programs should output `OK` and status code `0`, while invalid programs should output `Error: <description>` and status code `1`.
  * `Scan+parse`: Runs both the scanner and parser phases
  * `Typecheck`: Runs until the end of the typechecker phase
  * `Codegen`: Runs until your compiler outputs the target code
* `src`: Source code for your assignment
* `build.sh`: Builds your compiler using `Make` or similar. You should replace the commands here if necessary to build your compiler
* `run.sh`: Runs your compiler using two arguments (mode - $1 and input file - $2). You should replace the commands here if necessary to invoke your compiler
* `test.sh`: Automatically runs your compiler against test programs in the programs directory and checks the output

Comments found in both files provide more details as to the requirements. Since a large portion of grading is performed automatically, please ensure to follow the input/output specifications **exactly**. You must be able to run the provided scripts on the SOCS Trottier machines.
