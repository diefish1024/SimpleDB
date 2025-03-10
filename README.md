# SimpleDB

A C++ implementation of a simple database system. Based on the [cstack tutorial](https://cstack.github.io/db_tutorial/).

Use B+ tree for indexing and storing data.

## Code Structure

```
.
├── CMakelists.txt
├── README.md
├── include
│   ├── BPlusNode.hpp
│   ├── BPlusTree.hpp
│   ├── Constants.hpp
│   ├── Cursor.hpp
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
│   ├── BPlusTree.cpp
│   ├── Constants.cpp
│   ├── Cursor.cpp
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

## Test

```bash
$ python tests/test.py
test_disk_persistence passed
.test_insert_and_select passed
.test_max_length_strings passed
.test_negative_id passed
.test_table_full passed
.test_too_long_strings passed
.
----------------------------------------------------------------------
Ran 6 tests in 0.297s

OK
```