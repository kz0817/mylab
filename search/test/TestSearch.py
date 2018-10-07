#!/usr/bin/env python3
import unittest
import subprocess
import re

class TestSearch(unittest.TestCase):

    def setUp(self):
        self.__re_target = re.compile(r'^Match: ')

    def __extract_positions(self, lines):
        for line in lines:
            match = self.__re_target.match(line)
            if not match:
                continue
            dec_str, hex_str = line[match.end():].split()
            dec_val = int(dec_str)
            hex_val = int(hex_str, 16)
            self.assertEqual(dec_val, hex_val)
            yield dec_val

    def __run(self, pattern):
        material_file = 'material1.txt'
        ret = subprocess.run(['../search', material_file, pattern],
                             stdout=subprocess.PIPE)
        self.assertEqual(ret.returncode, 0)
        lines = ret.stdout.decode('utf-8').split('\n')
        return list(self.__extract_positions(lines))


    def test_top(self):
        matches = self.__run('abc')
        self.assertEqual(matches, [0,])

    def test_not_top(self):
        matches = self.__run('cd')
        self.assertEqual(matches, [2,])

    def test_tail(self):
        matches = self.__run('xyz')
        self.assertEqual(matches, [12,])

    def test_multiple(self):
        matches = self.__run('f0')
        self.assertEqual(matches, [5, 10])

if __name__ == '__main__':
    unittest.main()

