# jdshell

[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)

Shell for operating systems class at UNM (cs-481) by Jayson Grace (jayson.e.grace@gmail.com) and Dominic Salas (dominic.salas@gmail.com).


## Supported Commands
* exit
* last10 - gives you the last ten commands run

## Instructions

1. Clone the repo:
```bash
git clone https://github.com/l50/jdshell.git <name of folder you choose> && cd <name of folder you chose>
```
2. Compile to get the executable:
```bash
cd src && gcc main.c -o jdshell
```

3. Run the shell:
```bash
./jdshell
```

### Todo
- [x] Create shell prompt
- [x] Implement exit
- [] Implement last10
- [] Add Error handling  
- [] Implement "command information line"
- [] Implement Background job handling
- [] Implement signal handler
- [] Ensure ls, ps, sleep and date work with jdshell
- [] Create report
