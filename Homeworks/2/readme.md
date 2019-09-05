CSCI-243 Homework 2 \-- Histogram
=================================

Due Date
--------

Due: Thursday, September 12, 2019 at 11:59pm

Overview
--------

This assignment provides practice writing C programs using:

-   a header file for specifying function prototypes and defined
    constants;
-   character input from `stdin` (`getchar`);
-   looping (`for` and `while`), and conditionals (`if`);
-   processing arrays;
-   calling functions and parameter passing;
-   output of characters and integers to `stdout` (`printf`);
-   output to `stderr` (`fprintf`).
-   checking for end of file (`getchar` return value); and

Problem summary
---------------

The problem is to read a text document from *standard input* and to
display a (vertical) histogram to *standard output*. where the column
heights represent the the number of alphabetic characters \'A\' through
\'Z\' in the document.

The histogram will have one column of \'X\'s for each of the alphabetic
characters \'A\' through \'Z\', where the height of the column will be
the number of characters in the document corresponding to the given
character. The columns will be labeled on the bottom of the histogram by
the letters \'A\' through \'Z\', as shown in the examples below.

Lowercase alphabetic characters are to be converted to uppercase for the
purposes of counting characters. This means that the histogram for an
input of \"abc\" will be exactly the same as the histogram for an input
of \"ABC\".

The input text document should be read until it reaches end-of-file.

If there are no alphabetic characters in the document, the output will
be *only* the line \"no alphabetic characters read\", sent to *standard
error*; otherwise, the output to *standard output* will be a line of the
form \"nn alphabetic characters read\" \-- where \"nn\" is the (decimal)
number of alphabetic characters in the document \-- followed by the
histogram as described above. See the examples below for the precise
output expected.

The following are the step-by-step activities you should carry out to
complete this homework:

Write a header file, `hist.h`, which specifies the prototypes of the
utility functions `read_text` and `display_histogram` as well as an
integer constant `HIST_SIZE`:

-   The `read_text` function takes a single parameter, an array of
    integers, and returns an integer result.
-   The `display_histogram` function takes a single parameter, an array
    of integers, and returns nothing (`void`).
-   The `HIST_SIZE` constant (use the `#define` directive) is 26, the
    number of uppercase letters \'A\' through \'Z\'. This constant will
    be used to create the array of letter counts for the purpose of
    building the histogram.
-   Use an [\#include](https://en.wikipedia.org/wiki/Include_guard)
    guard to bracket your header file contents.

Create a source file `hist.c` that defines a `main` function and two
function *stubs*: one for `read_text` and another for
`display_histogram`. These functions may appear in any order in your
source file, but typically your `main` function will appear *last*.

1.  Implement your `read_text` function stub. This function should have
    the same signature as the `read_text` function prototype in your
    `hist.h` header file. The body of this function stub will do nothing
    other than return the value zero.
2.  Implement your `display_histogram` function stub. This function
    should have the same signature as the `display_histogram` function
    prototype in your `hist.h` header file. The body of this function
    stub should output (to *standard output*)the `HIST_SIZE` values in
    the argument array, one value per line.
3.  Implement your `main` function to take no parameters (its parameter
    list is `void`) and return an integer result. In the body of your
    `main` function, first create an integer array of size `HIST_SIZE`
    that will hold the letter counts. This array should be initialized
    to all zeros: it will be passed as a parameter to both the
    `read_text` and `display_histogram` functions. Next, in your `main`
    function body, pass this array to the `read_text` function, and
    deposit the return value into an integer variable. Then check this
    return value: if the return value is zero, output the line
    `"no alphabetic characters read"` to *standard error*; if it is
    nonzero, output the line `"nn alphabetic characters read"` to
    *standard output* \-- where `nn` is the return value \-- and then
    pass the integer array to the `display_histogram` function. The last
    thing your `main` function body should do is to return the value
    `EXIT_SUCCESS`, which is defined in the header file `stdlib.h`.

Compile your `hist.c` source file as follows:

        gcc -ggdb -std=c99 -Wall -Wextra -pedantic hist.c -o hist

Test your resulting program `hist` using input from your `hist.c`
program as follows (using standard input redirection):

        ./hist < hist.c

This should produce the `"no alphabetic characters read"` message,
because your `read_text` stub just returns zero.

Of course, you can send other text to your `hist` program, including
input from your keyboard, like this:

        ./hist

To end input from the keyboard, enter a `ctrl-D` at the beginning of a
line to signal the end of input from *standard input*

Change your `read_text` stub to return the value 1 instead of zero.
Recompile (as above) and carry out the same test as above. This should
produce the message `"1 alphabetic characters read"`, and then should
print 26 lines of zeros as a result of calling `display_histogram`
(why?).

Now complete the correct implementation of the `read_text` function:
First, create an integer variable to hold the letter count of the number
of alphabetic characters read, initialized to zero. Then enter a loop
that attempts to read a character from *standard input*, stopping when
no more characters are available (see the documentation for `getchar`
for how to do this). For each character successfully read, if it is a
lowercase letter, convert it to uppercase. You may use `toupper` to do
so, if you wish. If the (possibly converted) character is an uppercase
letter in the range \'A\' through \'Z\', increment the corresponding
slot in the integer array that is passed as a parameter to this function
\-- where \'A\' corresponds to slot zero, \'B\' corresponds to slot 1,
and so forth \-- and increment the letter count variable. You should be
able to increment the appropriate slot in the integer array using just
one instruction! Finally, once the loop exits, return the letter count.

Compile and test your resulting implementation. Since you have changed
your `read_text` function but not your `display_histogram` function, you
should be able to see output that looks like this (your mileage may
vary):

        15 alphabetic characters read
        4
        3
        5
        1
        ... (22 more lines of integer values)

Be sure that the integer values printed by `display_histogram` are
correct, and verify that the sum of the letter frequencies equals the
letter count. Use short text inputs, even one-character ones (from the
keyboard is best), to check your results.

Finally, complete your implementation of the `display_histogram`
function, so that your output will conform to the examples shown below.
The first step in this implementation will be to determine the maximum
value of the integers in the letter count array. Once you know this, you
can proceed to display the rows of the histogram, which will consist of
spaces or \'X\'s as shown in the examples. Finally, display a line of 26
dashes, followed by a line consisting of the letters \'A\' through
\'Z\'.

Compile and run your program with several tests, including a test of
input having no alphabetic letters at all. Here are the results of some
sample runs. Your results should look *exactly* like these:

1.  file name: fox\
    file contents: \"The quick brown fox jumps over the lazy dog.\"\
    program execution:

        ./hist < fox

    program output:

        35 alphabetic characters read
                      X           
            X         X           
            X  X      X  X XX     
        XXXXXXXXXXXXXXXXXXXXXXXXXX
        --------------------------
        ABCDEFGHIJKLMNOPQRSTUVWXYZ

2.  file name: empty\
    file contents: (empty file)\
    program execution:

        ./hist < empty 

    program output (to standard error):

        no alphabetic characters read

3.  file name: box\
    file contents: \"Pack my box with five dozen liquor jugs.\"\
    program execution:

        ./hist < box

    program output:

        32 alphabetic characters read
                X     X           
            X   X     X     X     
        XXXXXXXXXXXXXXXXXXXXXXXXXX
        --------------------------
        ABCDEFGHIJKLMNOPQRSTUVWXYZ

*Do not limit your tests to just these.* Compile, test, and edit your
program until it works correctly.

Resources
---------

There are a number of online sources and tools that will be useful.

-   To ensure a header file is only included once, see
    <https://en.wikipedia.org/wiki/Include_guard> for how to write your
    own header files with *include guards*.
-   See <http://en.cppreference.com/w/> for online references to the C
    language, its libraries and other information. *Note that you should
    look for the sublinks that are specific to C because this site is
    for both C and C++.*
-   Use the `man` pages while logged onto the CS systems. Useful
    functions of interest for this assignment include:
    -   `getchar`
    -   `printf`
    -   `fprintf`
    -   among others\...
-   Use the following *format tag* to structure your integer output:
    -   `%d` for integral data;

Tests
-----

While you get no source code, there are some sample input files for
testing.

         get csci243 hw2

Here is what get fetches:

-   The files `box` and `fox` are files you can *redirect* into your
    program execution. The file `empty` is an empty file. They should
    produce the same output as shown above. You should use additional
    test files of your own design for testing purposes.
-   Before you submit, make sure that your source code (both .c and .h
    files) follows the [C Style
    Standards](https://cs.rit.edu/~csci243/resources/C-Style-Standard.pdf)
    for this class. In particular, consult the [.c style
    example](https://cs.rit.edu/~csci243/resources/style_c) and the [.h
    style example](https://cs.rit.edu/~csci243/resources/style_h) as
    models of how your code should appear.

#### Submission

Submit your `hist.c` and `hist.h` code with the *try command*:

        try grd-243 hw2-1 hist.c hist.h

You should consult the [Student guide to
try](https://cs.rit.edu/~csci243/resources/try.html) for comprehensive
information about how to use `try`, and the [Try
Summary](https://cs.rit.edu/~csci243/resources/try-summary.html) for a
quick guide for what to expect from `try`.

**Notes:**

-   You can resubmit your `hist.c` and `hist.h` files more than once.
    This is in case you want or need to make revisions and corrections
    after your earlier submissions.

Grading
-------

The distribution for grading is shown below.

-   5% `hist.h` prototypes and constant definition
-   5% `hist.h` `#include` guard
-   70% `hist.c` implementation
-   10% code style
-   10% code documentation, including docstrings with your name and
    section

------------------------------------------------------------------------

Updated: Mon Sep 2 13:07:36 EDT 2019

[normal]{.cleanslate .TridactylStatusIndicator .TridactylModenormal}
