#!/usr/bin/env python
import os
import subprocess
import time
import unittest


class TestSimpleDBCLI(unittest.TestCase):
    def run_commands(self, commands, timeout=10):
        """
        Launch SimpleDB.exe, send commands, terminate with .exit, and return the output.
        """
        exe_path = os.path.join("build", "SimpleDB.exe")
        all_input = "\n".join(commands + [".exit"]) + "\n"
        try:
            proc = subprocess.Popen(
                [exe_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                universal_newlines=True
            )
            out, err = proc.communicate(input=all_input, timeout=timeout)
            return out.split("\n"), err
        except subprocess.TimeoutExpired:
            proc.kill()
            self.fail("Program execution timed out")

    def test_insert_and_select(self):
        """
        Test inserting and retrieving a row.
        """
        commands = [
            "insert 1 user1 user1@example.com",
            "select"
        ]
        result, err = self.run_commands(commands)
        expected_output = [
            "db > Executed.",
            "db > 1 user1 user1@example.com",
            "Executed.",
            "db > "
        ]
        self.assertEqual(result, expected_output)
        print("test_insert_and_select passed")

    def test_table_full(self):
        """
        Test error message when table is full.
        Assuming table size is limited to 10000 rows.
        """
        script = [f"insert {i} user{i} user{i}@example.com" for i in range(1, 10000)]
        result, err = self.run_commands(script)
        self.assertEqual(result[-2], "db > Error: Table is full.")
        print("test_table_full passed")

    def test_max_length_strings(self):
        """
        Test inserting strings at the maximum allowed length.
        """
        long_username = "a" * 32
        long_email = "a" * 255
        commands = [
            f"insert 1 {long_username} {long_email}",
            "select"
        ]
        result, err = self.run_commands(commands)
        expected_output = [
            "db > Executed.",
            f"db > 1 {long_username} {long_email}",
            "Executed.",
            "db > "
        ]
        self.assertEqual(result, expected_output)
        print("test_max_length_strings passed")

    def test_too_long_strings(self):
        """
        Test error message when strings exceed the maximum allowed length.
        """
        long_username = "a" * 33
        long_email = "a" * 256
        commands = [
            f"insert 1 {long_username} {long_email}",
            "select"
        ]
        result, err = self.run_commands(commands)
        expected_output = [
            "db > String is too long.",
            "db > Executed.",
            "db > "
        ]
        self.assertEqual(result, expected_output)
        print("test_too_long_strings passed")

    def test_negative_id(self):
        """
        Test error message when ID is negative.
        """
        commands = [
            "insert -1 cstack foo@bar.com",
            "select"
        ]
        result, err = self.run_commands(commands)
        expected_output = [
            "db > ID must be positive.",
            "db > Executed.",
            "db > "
        ]
        self.assertEqual(result, expected_output)
        print("test_negative_id passed")


if __name__ == "__main__":
    unittest.main()
