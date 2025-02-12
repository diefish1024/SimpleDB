#!/usr/bin/env python
import os
import subprocess
import time
import unittest

class TestSimpleDBCLI(unittest.TestCase):
    def setUp(self):
        self.clean_up()

    def clean_up(self):
        if os.path.exists("test.db"):
            os.remove("test.db")

    def run_commands(self, commands, timeout=2, clean_up=True):
        """
        Launch SimpleDB.exe, send commands, terminate with .exit, and return the output.
        """
        exe_path = os.path.join("build", "SimpleDB.exe")
        all_input = "\n".join(commands + [".exit"]) + "\n"
        try:
            proc = subprocess.Popen(
                [exe_path, "test.db"],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                universal_newlines=True
            )
            out, err = proc.communicate(input=all_input, timeout=timeout)
            if clean_up:
                self.clean_up()
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

    def get_max_rows(self):
        """
        use --config to get the max rows
        """
        exe_path = os.path.join("build", "SimpleDB.exe")
        result = subprocess.run(
            [exe_path, "--show-config"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        for line in result.stdout.split("\n"):
            if line.startswith("MAX_ROWS = "):
                return int(line.split("=")[1].strip())
        raise ValueError("MAX_ROWS not found in config")

    def test_table_full(self):
        """
        Test error message when table is full.
        """
        max_rows = self.get_max_rows()
        # print(f"MAX_ROWS = {max_rows}")
        script = [f"insert {i} user{i} user{i}@example.com" for i in range(0, max_rows + 1)]
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

    def test_disk_persistence(self):
        """
        Test that data persists after .exit and restarting the program.
        """
        # Insert a record and exit
        commands_insert = [
            "insert 1 user1 user1@example.com"
        ]
        result_insert, err = self.run_commands(commands_insert, clean_up=False)
        self.assertEqual(result_insert, [
            "db > Executed.",
            "db > "
        ])
        
        # Re-run the program and select the data
        commands_select = [
            "select"
        ]
        result_select, err = self.run_commands(commands_select)
        expected_output = [
            "db > 1 user1 user1@example.com",
            "Executed.",
            "db > "
        ]
        self.assertEqual(result_select, expected_output)
        print("test_disk_persistence passed")


if __name__ == "__main__":
    unittest.main()