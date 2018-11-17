#!/usr/bin/env python3

import unittest
import subprocess
import os.path

class TestFunction(unittest.TestCase):

    def __run(self, msg):
        cp = subprocess.run(['md5calc', '-m', msg], stdout=subprocess.PIPE)
        self.assertEqual(cp.returncode, 0)
        return cp.stdout.decode('utf-8')

    def __run_from_file(self, filename):
        datafile = os.path.join(os.path.dirname(__file__), 'data', filename)
        cp = subprocess.run(['md5calc', '-i', datafile], stdout=subprocess.PIPE)
        self.assertEqual(cp.returncode, 0)
        return cp.stdout.decode('utf-8')

    @staticmethod
    def __create_data(length):
        return ''.join(['{:x}'.format(x % 16) for x in range(length)])

    def test_none(self):
        self.assertEqual(self.__run(''), 'd41d8cd98f00b204e9800998ecf8427e')

    def test_one_char(self):
        self.assertEqual(self.__run('A'), '7fc56270e7a70fa81a5935b72eacbe29')

    def test_data55_just_one_block(self):
        data = self.__create_data(55)
        self.assertEqual(self.__run(data), 'd8ea71eb4d2af27f59a5316c971065e6')

    def test_data56_over_one_block(self):
        data = self.__create_data(56)
        self.assertEqual(self.__run(data), 'a68f061e81239660f6305195739ba7f0')

    def test_data57_over_one_block(self):
        data = self.__create_data(57)
        self.assertEqual(self.__run(data), 'e5d5a4d0b3ccbea0dcd2de36fd8b45b1')

    def test_data63_data_and_first_pad_in_one_block(self):
        data = self.__create_data(63)
        self.assertEqual(self.__run(data), 'c3725f9ff3277e098c83525a7aba8dd5')

    def test_data64_only_data_in_one_block(self):
        data = self.__create_data(64)
        self.assertEqual(self.__run(data), 'fe3a1ff59f3b89b2ad3d33f08984874b')

    def test_data65_1byte_in_second_block(self):
        data = self.__create_data(65)
        self.assertEqual(self.__run(data), '5bb51689804d3fd9e3d1f619fd4f397d')

    def test_data1000(self):
        data = self.__create_data(1000)
        self.assertEqual(self.__run(data), 'd38365ddfe58a9bc1d2298f5bb17af48')

    def test_1MB_from_file(self):
        self.assertEqual(self.__run_from_file('test-1MiB.dat'),
                         'c35cc7d8d91728a0cb052831bc4ef372')

    def test_1MBplus1_from_file(self):
        self.assertEqual(self.__run_from_file('test-1MiBp1.dat'),
                         'a7fa9163dc7c40f72c018a926e0cfdb2')


if __name__ == '__main__':
    unittest.main()
