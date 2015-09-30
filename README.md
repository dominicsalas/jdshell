# jdshell

[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)

Shell for operating systems class at UNM (CS-481) by Jayson Grace (jayson.e.grace@gmail.com) and Dominic Salas (dominic.salas@gmail.com).


## Supported Commands
* exit - Exit the shell.
* last10 - Gives you the last ten commands run.

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

## Contributing
Please fork and do a pull request when you've created a feature that you want to be added.

### Todo
- [x] Create shell prompt
- [x] Implement executing commands
- [x] Implement exit
- [] Implement last10
- [] Ensure shell won't exit on error  
- [x] Implement "command information lines"
- [] Implement Background job handling
- [] Implement signal handler
- [x] Ensure ls, ps, sleep and date work with jdshell
- [x] Test with vim
- [] Take Snapshots
- [] Create report

### Extra
- [] Implement cd
