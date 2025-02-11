# SimpleDB

A C++ Version of the SimpleDB project from the [sqlite tutorial](https://cstack.github.io/db_tutorial/).

Current progress: [part 5](https://cstack.github.io/db_tutorial/part_5.html)

#### Project Structure

```
.
├── CMakelists.txt
├── README.md
├── include
│   ├── DB.hpp
│   ├── InputBuffer.hpp
│   ├── MetaCommand.hpp
│   ├── Pager.hpp
│   ├── Row.hpp
│   ├── Statement.hpp
│   ├── StatementParser.hpp
│   ├── Table.hpp
│   └── VirtualMachine.hpp
├── src
│   ├── DB.cpp
│   ├── InputBuffer.cpp
│   ├── MetaCommand.cpp
│   ├── Pager.cpp
│   ├── Row.cpp
│   ├── StatementParser.cpp
│   ├── Table.cpp
│   ├── VirtualMachine.cpp
│   └── main.cpp
└── tests
    └── test.py
```


#### Test

We assume that the program is stored in the `build` directory, if not, please change the path in the `test.py` file.

```
$ python tests/test.py
test_disk_persistence passed
.test_insert_and_select passed
.test_max_length_strings passed
.test_negative_id passed
.test_table_full passed
.test_too_long_strings passed
.
----------------------------------------------------------------------
Ran 6 tests in 0.328s
```
