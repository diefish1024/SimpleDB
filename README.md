# SimpleDB

A C++ Version of the SimpleDB project from the [sqlite tutorial](https://cstack.github.io/db_tutorial/).

Current progress: [part 4](https://cstack.github.io/db_tutorial/part_4.html)

#### Project Structure

```
.
├── CMakelists.txt
├── README.md
├── include
│   ├── InputBuffer.hpp
│   ├── MetaCommand.hpp
│   ├── Row.hpp
│   ├── Statement.hpp
│   ├── StatementParser.hpp
│   ├── Table.hpp
│   └── VirtualMachine.hpp
├── src
│   ├── InputBuffer.cpp
│   ├── MetaCommand.cpp
│   ├── StatementParser.cpp
│   ├── Table.cpp
│   ├── VirtualMachine.cpp
│   └── main.cpp
└── tests
    └── test.py
```


#### Test

We assume that the program is stored in the `build` directory.

```
$ python tests/test.py
test_insert_and_select passed
.test_max_length_strings passed
.test_negative_id passed
.test_table_full passed
.test_too_long_strings passed
.
----------------------------------------------------------------------
Ran 5 tests in 0.155s

OK
```
