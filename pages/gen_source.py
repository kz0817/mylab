#!/usr/bin/env python3

def show_func(idx):
    func_string_arr = [
        'int func%04d(const int a, const int b)' % idx,
        '{',
        '    return a % b;',
        '}',
        '',
    ]
    print('\n'.join(func_string_arr));

def main():
    for i in range(50000):
        show_func(i)

if __name__ == '__main__':
    main()

