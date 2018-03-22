#!/usr/bin/env python3

import os
import sys
import shutil
import subprocess
import argparse


class IsolationTest:

    def __init__(self, test_dir, compiler_bin_path):
        self.input_dir = os.path.join(test_dir, "input")
        self.result_dir = os.path.join(test_dir, "result")
        self.expected_dir = os.path.join(test_dir, "expected")
        self.compiler_bin_path = compiler_bin_path
        self.error_cases = []
        self.ok_num = 0
        self.fail_num = 0
        
        if os.path.exists(self.result_dir):
            shutil.rmtree(self.result_dir)
        os.mkdir(self.result_dir)

    def run_test(self, input_file):
        test_name = os.path.basename(input_file)
        result_file = os.path.join(self.result_dir, test_name)
        fresult = open(result_file, "w")
        proc = subprocess.Popen([self.compiler_bin_path, input_file],
                                stdout=subprocess.PIPE,
                                stderr=fresult)
        proc.wait()
        fresult.close()
        diffs = self.generate_diff(test_name)
        if diffs:
            self.error_cases.append(diffs)
            print("test %s: FAILED" % test_name, file=sys.stderr)
            self.fail_num += 1
        else:
            print("test %s: OK" % test_name, file=sys.stderr)
            self.ok_num += 1

    def run(self):
        line = "\n" + 80 * "%" + "\n\n"
        input_files = os.listdir(self.input_dir)
        for fn in input_files:
            self.run_test(os.path.join(self.input_dir, fn))

        total = self.fail_num + self.ok_num
        print("Summary: run total %d tests, %d ok, %d failed" % (total, self.ok_num, self.fail_num))
        if self.error_cases:
            print(line.join(self.error_cases))

    def generate_diff(self, test):
        result = os.path.join(self.result_dir, test)
        expected = os.path.join(self.expected_dir, test)
        cmd = ["diff", "-c", result, expected]
        return subprocess.check_output(cmd)
                                                   
        
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--dir", help="test root dir", type=str)
    parser.add_argument("-c", "--compiler", help="compiler bibnary path", type=str)
    args = parser.parse_args()
    
    test = IsolationTest(args.dir, args.compiler)
    test.run()
    sys.exit(0)
