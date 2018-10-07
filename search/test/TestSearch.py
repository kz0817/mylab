#!/usr/bin/env python3
import unittest
import subprocess
import re

class TestSearch(unittest.TestCase):

    def setUp(self):
        self.__re_target = re.compile(r'^Match: ')

    def __run(self, pattern):
        material_file = 'material1.txt'
        ret = subprocess.run(['../search', material_file, pattern],
                             stdout=subprocess.PIPE)
        self.assertEqual(ret.returncode, 0)
        lines = ret.stdout.decode('utf-8').split('\n')

        match_pairs = map(lambda l: (self.__re_target.match(l), l), lines)
        match_lines = filter(lambda m: m[0], match_pairs)
        return [int(line[match.end():]) for match, line in match_lines]

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

