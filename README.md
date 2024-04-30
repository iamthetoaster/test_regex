# A Simple Regex Test Program

This program can be used to test regexes. It takes the regex as an argument, and then tries to match it against all other arguments. It will underline the match, and colorize groups within the match. 

## Screenshots

A simple example of how to use this program:

[A screenshot of the following command, and its output: ./test_regex 'A(B+)C' '...AAABBBCCC...'](./.screenshots/simple_example.png)

A more complex example, using xargs and searching for variable declarations in this program's source code:

[A screenshot of the following command, and its output: tr '\n' '\0' < test_regex.c | xargs -0 ./test_regex '^ *(\[A-Za-z_\]+) +(const +)?(\*)?(const +)?(\[A-Za-z_\]+) *(\\[\[0-9\]*\\])? *= *(.*)'](./.screenshots/complex_example.png)
