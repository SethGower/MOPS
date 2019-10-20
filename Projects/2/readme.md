Mechanics of Programming Project 2 Interpreter 
===============================================

-   Revision 1.0 - published Thu Oct 12 7:17:35 EDT 2019
-   Revision 1.1 - changed due date Thu Oct 17 09:02:51 EDT 2019

------------------------------------------------------------------------

Due Date: Nov 5, 2019 at 11:59 PM
---------------------------------

Overview
--------

In this project you will implement an
[interpreter](http://en.wikipedia.org/wiki/Interpreter_(computing)) for
a very simple arithmetic language whose features include:

-   Integer values;
-   Mathematical operations including addition, subtraction,
    multiplication, division, modulus, and ternary if-then-else; and
-   Variables which can be initialized and modified by assignment

Expressions are presented to the interpreter, from standard input, in
[postfix
notation](http://en.wikipedia.org/wiki/Reverse_Polish_notation). The
interpreter analyzes the expressions, one line at a time, and produces
their [infix notation](http://en.wikipedia.org/wiki/Infix_notation) as a
string, along with the evaluated result of the expression.

### Read-Eval-Print loop

-   After optionally reading symbol table entries from a file whose name
    is on the command line (see below), the program enters a
    read-eval-print loop that **prompts the user** for an expression to
    process, and reads a line from standard input. If the first
    character in the line is the \"\#\" character, ignore the line and
    continue to the next input line. Otherwise the line should represent
    an entire postfix expression to evaluate. If the line contains the
    \"\#\" character other than at the beginning, ignore everything in
    the line from that character to the end of the line.
-   Once the user enters an expression, it is *interpreted*.
    Interpretation of an expression consists of three steps:
    1.  tokenizing it (splitting the expression into individual textual
        components),
    2.  parsing it (checking the expression for proper syntax and
        building a parse tree), and then
    3.  evaluating it (generating a result).

### Interpretation example

Let\'s follow the steps in evaluating the expression

>     a b + c * 7 +

#### Tokenizing

The parser first enters the tokenizing step. Here it treats the
expression (a single string with comments removed) as a sequence of
meaningful tokens. In this project, tokens in the input expression
consist of strings that are separated by whitespace (spaces, tabs, and
newlines). In our example, the tokens are

>     "a" "b" "+" "c" "*" "7" "+"

As the expression is tokenized from left to right, the individual tokens
are pushed onto a stack. Since a stack is a LIFO structure, the token
`"a"` ends up at the bottom of the stack and the token `"+"` ends up at
the top. The stack is represented here as a linked list.

>     stack top -> "+" -> "7" -> "*" -> "c" -> "+" -> "b" -> "a"

#### Parsing

Parsing a tokenized stack consists of building a *parse tree* whose
interior nodes are operations (addition, subtraction, assignment, etc.)
and whose leaf nodes are either decimal integer literals (strings such
as \"7\" and \"13\") or symbols (strings such as \"c\" and \"xyzzy\").
Consider a data structure called `tree_node`, having a field
representing a pointer to the token residing at this node (a string).
The type name of a `tree_node` is `"tree_node_t"`, as defined in
`tree_node.h`. A `tree_node` comes in two flavors:

-   an `INTERIOR` node with an `op` field identifying an arithmetic
    operation and with two `tree_node` fields `left` and `right`. The
    `op` field of an interior node is an `enum` value defined in
    `tree_node.h`. There are separate identifiers in this `enum` for the
    operations of addition, subtraction, etc. For example, the `op`
    field corresponding to the arithmetic operation `"+"` is `ADD_OP`.
-   a `LEAF` node having an `exp_type` field identifying the token as an
    `INTEGER` or a `SYMBOL`.

These two `tree_node` types will have appropriate constructors, with
pseudo-code prototypes like this:

-   tree\_node \*make\_interior(op, token, left, right)
-   tree\_node \*make\_leaf(exp\_type, token)

Here\'s a (Python-ish) pseudo-code algorithm to produce the parse tree
from the stack. This code does not include how to deal with the ternary
operation.

>     define tree_node parse(stack):
>         token = top(stack)
>         pop(stack)
>         if token is an operation:
>             op = the operation identified by the token
>             right = parse(stack)
>             left = parse(stack)
>             return make_interior(op, token, left, right)
>         else: # token is a decimal integer literal or symbol
>             exp_type = INTEGER or SYMBOL as determined by the token
>             return make_leaf(exp_type, token)

If the stack is empty when entering the `parse` function or if the stack
is nonempty after calling `parse`, then the parse fails, and an
appropriate error should be reported.

When the parser peels a token off the stack, it needs to check if the
token is an operation, a decimal integer literal, or a symbol.

-   Checking for an operation is simple: see if the token string matches
    (exactly) one of the seven operations given in `tree_node.h`. If so,
    build the interior node appropriately as described in the above
    pseudo-code, using the corresponding `op_type` as given in
    `tree_node.h`
-   If the token is not an operation, the token string must be either a
    symbol or a decimal integer literal.
    -   A symbol must start with an alphabetic character and must
        consist of zero or more additional alphanumeric characters. If
        the token string satisfies this property, build a leaf node
        appropriately.
    -   A decimal integer literal must start with a decimal digit and
        must consist of zero or more additional decimal digits. If the
        token string satisfies this property, build a leaf node
        appropriately.
    -   If the token string does not satisfy either of the above, report
        an error and terminate the parse.

Note that this project does now allow negative decimal integers literals
to appear in an expression. This means that an expression like
`"7 -4 +"` is not allowed, since `"-4"` would not be considered a legal
token. However, you can achieve the same effect by expressing the `"-4"`
part as in the following: `"7 0 4 - +"`

Using our earlier example, the resulting parse tree looks like this:

![Display of
exptree.jpg](CSCI-243:%20Mechanics%20of%20Programming%20-%20Project%202_files/exptree.png)

#### Evaluation

All of our expressions will evaluate to an integer. Once the parse tree
is successfully generated, it is evaluated using the following
pseudo-code:

>     define int eval(tree_node):
>         if tree_node is a leaf_node:
>             # return either the value bound to the token symbol
>             # or the integer value of the decimal integer literal
>             return leaf_node value
>         else: # tree_node is an interior_node
>             op = interior_node operation
>             left = interior_node->left
>             right = interior_node->right
>             # perform op on eval(left) and eval(right)
>             # must do the evaluation in left, then right order
>             # to preserve the left associativity of the operations
>             leftVal = eval(left)
>             rightVal = eval(right)
>             result = op(leftVal, rightVal) # perform the operation
>             return result

Consider, for example, what happens when we evaluate the subtree rooted
at the lower-left interior node `"+"` in the above example. Its `op`
field is `"+"`, its `left` field is the leaf node `"a"` and its `right`
field is the leaf node `"b"`. Assume that the symbol table binds the
symbol `"a"` to the value 10, and the symbol `"b"` to the value 20. The
*values* of these leaf nodes are thus 10 and 20 respectively, and their
sum \-- using the `"+"` operation \-- is 30. Thus the result of calling
`eval` on the interior node `"+"` is 30.

This value, then, is used in the evaluation of the left field of the
interior node `"*"`, and so on. This happens in turn until all interior
node operations are evaluated, with the last being at the root of the
parse tree. Assuming that the value bound to the symbol `"c"` is 30, the
resulting value of the entire example expression is 907.

For evaluating a leaf node:

-   obtaining the integer value a decimal integer literal can be
    accomplished by a `strtol` library call;
-   obtaining the integer value of a symbol is done by looking up the
    symbol in the symbol table and using the value bound to that symbol
    as its value. (If the symbol is not in the symbol table, the
    evaluation should fail.)

**Do *not* evaluate decimal integer literals or symbols while building
your parse tree. Make the appropriate evaluations *only* when calling
`eval`.**

If the parse tree traversal was successful, the entire tree is traversed
inorder again (with no evaluation) to print the expression in an easier
to read infix format, with parentheses indicating precedence. In our
example, we would get

>     (((a+b)*c)+7)

-   Printing a leaf node can be done simply by printing the leaf node\'s
    token string.
-   Printing an interior node consists of printing an open paren \"(\",
    printing the left tree\_node (recursively), printing the operation
    token, printing the right tree\_node (recursively), and then
    printing a close paren \")\".

The infix representation should not contain any whitespace. After
printing this infix representation of the parse tree, the (integer)
value of the expression is printed in one line in the format

>     ...infix representation... = value

with exactly one space before and after the \"+\". Using the example
above, you should get the following output:

>     (((a+b)*c)+7) = 907

#### The assignment operation `"="`

The assignment operation `"="` is used to modify a binding of a variable
in the symbol table to a new value, or \-- if the variable does not
exist in the symbol table \-- to create a new symbol table entry binding
the symbol to a value. For example, suppose the symbol `d` does not
exist in the symbol table. Then the expression

>     d 30 =

would create a symbol table entry for `"d"` and would bind this symbol
to the (integer) value 30. The value of this expression is 30.

For another example, if the symbol `a` already exists in the symbol
table and is bound, say, to 10, then the expression

>     a 12 =

would modify the value bound to `a` to the new value 12. The value of
this expression is 12.

Since assignments have values, one can do something like this:

>     two too to 2 = = =

This expression, which evaluates to 2, would modify (or create) the
bindings for all of the variables `two`, `too`, and `to` to the value 2.

The tricky part of the assignment operation is that when you see an
expression like

>     two 2 =

the `left` field in the interior node `"="` of the parse tree *must* be
a symbol, and it should *not* be evaluated. Instead, the `right` field
should be evaluated and its value should be bound to the `left` symbol
(created, if it\'s not already there). So the `eval` procedure
pseudo-code shown above should look now like this:

>     define val eval(tree_node):
>         if tree_node is a leaf_node:
>             # return either the integer value bound to the token symbol
>             # the value of decimal integer literal
>             return leaf_node value
>         else: # an interior_node 
>             op = interior_node operation
>             left = interior_node->left
>             right = interior_node->right
>             if op is not the assignment operation "=":
>                 # perform op on eval(left) and eval(right)
>                 # must do the evaluations in left, then right order
>                 # to preserve the left associativity of the operations
>                 leftVal = eval(left)
>                 rightVal = eval(right)
>                 result = op(leftVal, rightVal) # perform the operation
>                 return result
>             # op must be "=" from here on
>             if left is not a leaf_node SYMBOL:
>                 return ERROR
>             sym = leaf_node token
>             if sym is not in the symbol table:
>                 create a symbol table entry for sym
>             val = eval(right)
>             bind val to sym
>             return val

#### The ternary operation `"?"`

This operation behaves like its C counterpart. Expressions in postfix
notation do not require parentheses and other delimiters as they do in
C, so a C ternary expression such as

>     a ? b : c

will be expressed in our postfix notation simply as

>     a b c ?

The `"a"`, `"b"`, and `"c"` parts can be replaced by any expressions.

Tokenizing this expression produces a stack that looks like this:

>     top of stack -> "?" -> "c" -> "b" -> "a"

When parsing this, the `"?"` token is recognized as a `Q_OP` (\"Q\" for
\"query\"). This means that the parser needs to parse *three*
expressions: the expression-if-false (`"c"`) and expression-if-true
(`"b"`) parts, and the test expression (`"a"`). These parsing steps need
to be carried out in exactly this order.

For a ternary operation, how should the parse tree nodes be constructed?
The simple way to do this is to make the top-level `INTERIOR` node for
this expression have the token `"?"` and `Q_OP` operation. The left
field of this interior node is the node obtained by parsing the test
expression. The right field of this interior node can be constructed *as
an `INTERIOR` node* with a special \'token\' field `":"` (corresponding
to its C counterpart) and a special `ALT_OP` operation, with its left
and right fields obtained by parsing the expression-if-true and
expression-if-false parts, respectively. The `ALT_OP` operation does not
correspond directly to any of the arithmetic tokens that the parser
recognizes, but it conveys the nature of the node as having two
`ALT`ernatives. Its construction also permits an easy translation into
infix form similar to its C counterpart.

The parse tree for the above expression looks like this, where we only
display the token strings.

![ternary parse
tree](CSCI-243:%20Mechanics%20of%20Programming%20-%20Project%202_files/abcq.png)

The infix representation of this expression prints as follows:

>     (a?(b:c))

When *evaluating* a `Q_OP` expression node, first evaluate the test
expression that appears in the left field of the node. If the test
expression evaluates to a nonzero (true) value, evaluate the left
expression field of the `ALT_OP` node and return this value, otherwise
evaluate the right expression field of the `ALT_OP` node and return this
value.

**Do not evaluate both the left and right fields of the `ALT_OP` node.
The semantics of the `Q_OP` operation as being equivalent to its C
counterpart mandate that only one of the alternatives should be
evaluated.**

Interestingly, you can do this evaluation as a one-liner using the C
ternary operation itself!

Here are two examples of ternary expressions entered in the
read-eval-print loop and the output they would produce:

    > 1 3 5 ?
    (1?(3:5)) = 3
    > 0 3 5 ?
    (0?(3:5)) = 5

Assignment details
------------------

### Getting the Code

You can obtain the provided source code for this assignment by typing:

>     get csci243 project2

These are the files are supplied to you:

-   `tree_node.h` - Expression definitions, including the expression
    node
-   `header.mak` - Makefile header for `gmakemake`
-   `interp.h` - Header file for the main program
-   `parser.h` - Parser definitions, including the error types
-   `stack.h` - An interface to a stack structure
-   `stack_node.h` - The definition of a stack node
-   `symtab.h` - The symbol table interface, including the symbol
    definition
-   `sym.1` - A sample symbol table file with 6 symbol entries.

There are a lot of useful comments in the header files to help you
figure out what you need to do.

**Always use `gmakemake` to create your Makefile and use `make` to
compile your source files into an executable.**

Requirements
------------

### Input

#### Main Program

Your program, `interp`, runs on the command line with one optional
argument. If there are any more arguments, you should display the
following usage message to standard error and return with
`EXIT_FAILURE`:

>     Usage: interp [sym-table]

If the `sym-table` argument is present, your program should open the
file for reading. If the file open fails, your program should use
`perror(...)` to print an appropriate error message (with \"\...\"
replaced by the file name string) and should return with `EXIT_FAILURE`.

The contents of the `sym-table` file will be zero or more free-format
lines of the form

>     symbol   value

where `symbol` is an alphanumeric string starting with an alphabetic
character and where `value` is a decimal integer literal. Lines
beginning with the \"\#\" character should be skipped. For each line of
the proper format, your program should create a symbol table entry
binding the given `symbol` to its corresponding integer `value`. These
bindings can then be used to evaluate expressions entered from standard
input. Any failures to process the lines in this file should write an
appropriate error message to standard error and should exit the program
with `EXIT_FAILURE`.

For example, consider a file `sym.1` whose contents appear as follows:

>        a 10
>     b             20
>            c   30
>     #   blah 10
>             x 3
>     y       5
>      zzz 7

After the file is read, but before you are prompted to enter expressions
to evaluate, your program should print the symbol table *exactly* as in
the following example (except possibly for the order in which the
Name/Value lines appear):

>     SYMBOL TABLE:
>         Name: zzz, Value: 7
>         Name: y, Value: 5
>         Name: x, Value: 3
>         Name: c, Value: 30
>         Name: b, Value: 20
>         Name: a, Value: 10

There should be exactly one tab character (`'\t'`) preceding each of the
`Name:` entries on these lines. If the symbol table is empty \-- either
because no `sym-table` file was given or if the file produced no symbol
bindings \-- do not print *any* symbol table output, including the
`SYMBOL TABLE:` heading.

Once symbol table entries have been loaded, your program should print
the following line (no leading whitespace):

>     Enter postfix expressions (CTRL-D to exit):

After this line, your program should enter a read-eval-print loop, where
you will be prompted with the characters `"> "` (a `">"` character
followed by a single space *and no newline*). You can then enter a line
of text containing a postfix expression to be evaluated.

**If the line begins with a `"#"` character, it is a comment line:
ignore the line and continue to the start of the read-eval-print loop.
If the line otherwise contains a `"#"` character somewhere in the line,
ignore everything from that character to the end of the line.** You can
accomplish this simply by replacing the \"\#\" character with a null
byte. \[Hint: use `strchr`.\]

Once a non-comment line has been found, you should evaluate the given
postfix expression. If the evaluation is successful, your program should
print the equivalent fully-parenthesized infix expression followed by
the characters `" = "` (a space, then the `"="` character, and another
space) followed by the expression value. If the evaluation is not
successful, an appropriate error message should be displayed (as
described above), but your program should *not* terminate. The loop
continues until you enter CTRL-D.

Once the loop finishes, you should print the symbol table again, in
exactly the same format a shown above, with any changes in bindings or
additional entries that may have been created by expression evaluation.

Here is a sample run using the `sym.1` file as described above:

>     ./interp sym.1
>     SYMBOL TABLE:
>         Name: zzz, Value: 7
>         Name: y, Value: 5
>         Name: x, Value: 3
>         Name: c, Value: 30
>         Name: b, Value: 20
>         Name: a, Value: 10
>     Enter postfix expressions (CTRL-D to exit):
>     > a
>     a = 10
>     > b 5 +
>     (b+5) = 25
>     > zzz 2 *
>     (zzz*2) = 14
>     > 1 3 5 + *
>     (1*(3+5)) = 8
>     > 1 3 + 5 *
>     ((1+3)*5) = 20
>     > a a 1 + =
>     (a=(a+1)) = 11
>     > a
>     a = 11
>     > <-- user entered CTRL-D
>     SYMBOL TABLE:
>         Name: zzz, Value: 7
>         Name: y, Value: 5
>         Name: x, Value: 3
>         Name: c, Value: 30
>         Name: b, Value: 20
>         Name: a, Value: 11

#### Symbol Table

The symbol table file command-line argument is optional; the user does
not have to specify a symbol table file on the command line, and the
program should create an empty symbol table in that case.

#### Tokens

As noted above, the tokens extracted by the tokenizer from a string
entered at the prompt will fall into one of these categories:

-   A mathematical operation: \'+\', \'-\', \'\*\', \'/\', \'%\', \'?\',
    \'=\'
-   A decimal integer literal: e.g. \'10\', \'3\', \'0\'
-   A variable name: e.g. \'a\', \'iVal\', \'Result99\'

Tokens are separated from one another by one or more whitespace
characters. See `tree_node.h` for an `enum` that provides identifiers of
the form `"..._OP"` for each of the arithmetic operations.

#### Tokenizer, Parser, and Evaluation Errors

There are many errors that can be encountered while reading input,
parsing, or evaluating an expression. All error messages should be
displayed to standard error. Most of the errors should not cause the
execution of the program to end, *with the following exceptions*:

1.  attempt to retrieve the top of an empty stack
2.  attempt to pop from an empty stack
3.  error while reading the symbol table file given on the command line
4.  command line with the wrong number of arguments

If your programs are written correctly, you should never encounter the
first two of these errors, since you should always check for an empty
stack before attempting any `pop` or `top` operation. The third and
fourth errors can only occur at the beginning of program execution,
before entering the read-eval-print loop.

Here are some example non-fatal error messages you will want to consider
including in your program. You may want to give more information about
each error, if appropriate. Once one error message is detected and
displayed, processing the current expression should terminate, and the
user should be allowed to enter the next expression. *Do not display
multiple error messages for a single expression.*

>     Invalid expression, not enough tokens
>     Illegal token
>     Invalid expression, too many tokens
>     Division by zero
>     Unknown operation
>     Undefined symbol
>     Unknown node type
>     Missing l-value
>     Invalid l-value
>     No room in symbol table
>     Input line too long

### Memory Management

Your program must use dynamic memory management (heap allocation) for
the following:

-   Stack: the stack structure and all stack\_nodes
-   Parser: tree\_node, interior\_node, and leaf\_node
-   Symbol table: the symbol table entry, and the data in the entry (a
    string)

After processing each expression, the stack and the tree allocations
must be cleaned up before going on to the next expression. The symbol
table should persist from one expression to another, so no symbol table
cleanup should be done until your program exits. Use valgrind to test
your programs (with valid as well as invalid expressions) and verify
that you do not have any memory leaks.

Coding Suggestions
------------------

### Development Roadmap

This is a very large project spanning over more than three weeks time.
If you do not start early and continually work on it, there is little
chance you will succeed in the end. Here is a suggestion for how to
proceed:

1.  Implement the stack and write a test program that verifies it works
    as advertised (see `stack.h` and `stack_node.h`).
2.  Implement the symbol table and write a test program for it (see
    ` symtab.h`). Make sure that you can read, lookup, display and add
    symbol bindings. (It\'s easy to build symbol table entries by hand
    to test things.)
3.  Write the main program (see `interp.h`). It should handle the
    command line arguments, loading of the symbol table, and contain a
    skeleton of the main read-eval-print loop which continually prompts
    for expressions (and possibly displaying them back as-is) until
    ctrl-D is entered. Do *not* attempt to parse or evaluate these
    expressions yet.
4.  Implement the `make_interior` and `make_leaf` functions (see
    `tree_node.h`) so that you can begin to understand how expressions
    are represented.
5.  The rest of your time will be spent in glorious parser land (see
    `parser.h`). Start off small and see if you can parse an expression
    with only one token (e.g. a symbol or a decimal integer literal,
    yielding a leaf\_node). **Do not attempt to evaluate your
    expressions yet. Just concentrate on building the parse tree.**
6.  Parse the arithmetic expressions next, except for ternary
    expressions.
7.  Finally, handle ternary (`"?"`) expressions.
8.  Make sure you leave time to check your memory management. After
    parsing an expression, you should free any stack and tree memory
    before reading the next expression. Test with all the possible
    errors that can occur and always observe your program in valgrind.
9.  Use `git` frequently to commit your changes as you make them. You
    will be required to submit a `revisions.txt` file that documents
    your `git` activity.

Your last task will be to implement expression evaluation. After your
`parse` routine successfully returns a parse tree, you should implement
the `eval_tree` function. Start simple! Here\'s a roadmap for you to
follow as you implement `eval_tree`:

1.  Evaluate decimal integer literals and pre-defined symbols if they
    were read from the command-line file. For example, the following
    expressions should return the appropriate values which you can
    simply display as integers (assuming that you have loaded a symbol
    table from the command line defining a binding to the variable `a`).

    >     3
    >     a

2.  Evaluate arithmetic operations *except for* the assignment operation
    `"="` and the ternary operation `"?"`.
3.  Evaluate assignment operations. Remember that an assignment
    operation has both a side-effect and a value.
4.  Finally evaluate ternary operations.

For the last two steps above, see the pseudo-code for guidance. Remember
that evaluating an expression may result in an error (such as divide by
zero and referencing an unbound symbol) If this is the case, report on
the error and return from `eval_tree` without printing a value.

The last piece is to implement the `print_infix` function that displays
the expression in fully-parenthesized form before the value of the
expression is displayed. This is actually one of the easiest parts of
this project! After you have done this, you should be able to see your
output formatted properly.

C Language Requirements
-----------------------

### ADTs

One topic that might not have been discussed in lecture yet, but is used
at the core of this project, is the notion of an Abstract Data Type
(ADT). It is required for distinguishing the difference between interior
and leaf nodes of the parse tree. Here is the definition of a
`tree_node` structure in `tree_node.h`

>     typedef struct tree_node_s {
>         node_type_t type;           // the type of the node (INTERIOR or LEAF)
>         char *token;                // the token that derived this node
>         void *node;                 // either an interior_node or leaf_node
>     } tree_node_t;

The `parse` function constructs instances of `tree_node`s by calling
`make_leaf` or `make_interior`. For each node, the `type` field will be
either `LEAF` or `INTERIOR`, and the `token` field will be the token
string popped from the stack.

The `node` field of a `tree_node` is what makes this an ADT. It\'s a
pointer to *nothing*, but when we create the `tree_node`, we want `node`
to point to the appropriate thing depending on whether the `tree_node`
should be a `LEAF` or `INTERIOR`. If it\'s a `LEAF`, we construct an
approprate `leaf_node` structure using `make_leaf` and make `node` point
to it. If it\'s an `INTERIOR`, we construct an appropriate
`interior_node` structure using `make_interior` and make `node` point to
it. We only need to use C casts to allow `node` to point to whatever
node we have constructed. (We do this because the number of bytes
required to construct a `leaf_node` is less than the number of bytes
required to construct an `interior_node`. Allocating the same amount of
space for both, perhaps by using a `union`, could end up being
inefficient.)

While there are other implementations of stacks that could be used in
this project \-- such as the `stackADT` implementation discussed in
lecture \-- use the one described in the provided `stack.h` header file
for this project.

### Other Useful C Routines

You will want to consider using functions from the C standard library.
Here is a brief summary of the major ones:

#### stdio.h

-   `fgets`: For reading in the expressions in the main
-   `fprintf`: For printing messages to standard error
-   `fscanf`: For reading in symbols from the symbol table file
-   `printf`: For standard output printing
-   `sprintf`: Useful for writing a message to a string buffer
-   `sscanf`: For extracting parts of a string in various formats
-   `perror`: For printing a system call error message
-   

#### string.h

-   `strcmp`: For comparing whether two strings are equal or not (used
    heavily whenever checking tokens against operation strings and for
    looking up symbols in the symbol table).
-   `strncpy`: For copying one string into another (useful for
    transferring strings into symbol names).
-   `strtok`: This is very useful for parsing a string into a sequence
    of tokens.

#### stdlib.h

-   `malloc/free`: For dynamic allocation/deallocation of `stack_node`s,
    `tree_node`s, and `symbol`s .
-   `strtol`: For extracting a string as an integer

You may want to explore using the `getline` function in the GNU C
library. The advantage of using `getline` is that you are not required
to allocate a string buffer in advance. If you use this, you will need
to include additional header files.

Submission
----------

Submit your project using the following command:

>     try grd-243 project2-1 interp.c revisions.txt [other-files...]

Where `[other-files...]` is a list of the **other source files that
constitute your program**.

The try program will link in the supplied header files (`tree_node.h`,
`interp.h`, `parser.h` `stack.h`, `stack_node.h`, `symtab.h`) and the
`header.mak` file; you must *not* submit them, and you must *not* make
any changes to the ones that were supplied to you using `get`.

While you may choose your own file names and project organization, you
should consider naming your source files to match the headers, where
each file implements the functions declared in the corresponding header:

>     tree_node.c
>     interp.c
>     parser.c
>     stack.c
>     symtab.c

In any case, you\'ll need to include all your source files on the try
submission line shown above.

Grading
-------

Your program will be graded based on the following criteria:

-   (5%) Stack: Correctly implements the linked stack structure.
-   (5%) Symbol Table: Can build and display a symbol table from a file.
-   (5%) Main loop: Correctly implements expression processing loop.
-   (60%) Parsing and expression evaluation: Can construct the parse
    tree for an expression, detect errors and produce the expression
    result in the specified format.
-   (10%) Memory Management: Valgrind does not report any memory issues
    in any tests.
-   (15%) Style and Documentation: The program code conforms to style
    and documentation standards for the course found in
    [C-Style-Standard.pdf](http://cs.rit.edu/~csci243/resources/C-Style-Standard.pdf).
    This includes adopting and using the *git* **version control system
    (VCS)** throughout the project to manage your project deliverables.
    1.  Are you following a consistent and reasonable coding style?
    2.  Is your code properly commented and documented?
    3.  Is your code logically separated between source files?
    4.  Are you using `git` regularly to save versions as they evolve?

Please check with your instructor regarding any additional requirements.

[normal]{.cleanslate .TridactylStatusIndicator .TridactylModenormal}
