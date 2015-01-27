# Main program for first homework of Utilization of datastructures
# and Tietorakenteet ja algoritmit.

import os
import random
import string
import timeit
import time
import gc
import sys
import binascii
import random

from datastructure import Datastructure

# Constants for commands
ADD = 'A'
READ = 'R'
PRINT = 'P'
MAXMIN = 'X'
EMPTY = 'E'
QUIT = 'Q'

GENERATE = 'G'
EFFICIENCY = 'F'
TEST = 'T'

# Other constants
OLD = 'old'
YOUNG = 'young'
E_FILE_NOT_FOUND = 'Error: file not found'
E_INT_EXPECTED = 'Error: birth year and enlisting year must be integers'
PROMPT = '> '
SEPARATOR = ' '

def test(datastructure, test_range=30):
    datastructure.empty()

    def test_print(datastructure):
        generate(datastructure, 1)
        datastructure.print()

    with open("sort.dat", 'w', encoding="utf-8") as output_file:
        output_file.write("#x y\n")

        for j in range(test_range):
            generate(datastructure, 250)
            t = timeit.Timer(lambda: test_print(datastructure), 'gc.enable()')
            sum_ = t.timeit(number=5) / 5
            size = datastructure.size()
            output_file.write("{0:d} {1:f}\n".format(size, sum_))
        datastructure.empty()

    def test_add(datastructure):
        generate(datastructure, 10)

    def test_oldest(datastructure):
        generate(datastructure,1)
        datastructure.oldest()

    def test_youngest(datastructure):
        generate(datastructure,1)
        datastructure.youngest()

    add_file = open('add.dat', 'w', encoding="utf-8")
    add_file.write("#x y\n")
    old_file = open('old.dat', 'w', encoding="utf-8")
    old_file.write("#x y\n")
    young_file = open('young.dat', 'w', encoding="utf-8")
    young_file.write("#x y\n")

#        t1 = timeit.Timer(lambda: test_add(datastructure), 'gc.enable()')
    t1 = timeit.Timer(lambda: generate(datastructure, 10), 'gc.enable()')
    t2 = timeit.Timer(lambda: test_oldest(datastructure), 'gc.enable()')
    t3 = timeit.Timer(lambda: test_youngest(datastructure), 'gc.enable()')
    for j in range(test_range):
        generate(datastructure, 250)
        add_sum = 0.0
        old_sum = 0.0
        young_sum = 0.0
        for i in range(5):
            add_sum += t1.timeit(number=1)
            old_sum += t2.timeit(number=1)
            young_sum += t3.timeit(number=1)
        size = datastructure.size()
        add_file.write("{0:d} {1:f}\n".format(size, add_sum / 5))
        old_file.write("{0:d} {1:f}\n".format(size, old_sum / 5))
        young_file.write("{0:d} {1:f}\n".format(size, young_sum / 5))
    add_file.close()
    old_file.close()
    young_file.close()

    datastructure.empty()

    del_file = open('del.dat', 'w', encoding="utf-8")
    del_file.write("#x y\n")
    t4 = timeit.Timer(lambda: datastructure.empty())
    for j in range(1, test_range + 1):
        generate(datastructure, j*250)
        size = datastructure.size()
        sum_ = t4.timeit(number=5) / 5
        del_file.write("{0:d} {1:f}\n".format(size, sum_))
    del_file.close()

    datastructure.empty()

def generate(datastructure, amount):
    colors = ["gold", "blue", "red"]

    for i in range(amount):
        # These are 2 times slower than method below, so use faster, but not
        # so pythonic methods to acquire random values for rank & name
#        rank = ''.join(random.choice(string.ascii_letters) for j in range(8))
#        name = ''.join(random.choice(string.ascii_letters) for j in range(10))
        rank = '%018x' % random.randrange(16**18)
        name = rank[8:]
        rank = rank[:8]

        year = random.randint(0, 200000)
        enlisting_year = random.randint(0,60) + 2200 + year
        birth_year = year + 2200
        shirt_color = random.choice(colors)
        datastructure.add(rank, birth_year, enlisting_year, shirt_color,
                          name)

def efficiency_test(datastructure, amount):
    # Generate first, because python is slow to generate random data
    generate(datastructure, amount)

    start = time.time()
    datastructure.print()
    datastructure.oldest()
    datastructure.youngest()
    datastructure.print()
    datastructure.print()
    datastructure.print()
    datastructure.print()
    datastructure.print()
    datastructure.print()
    datastructure.print()
    end = time.time()
    print("{0:f}".format(end - start), file=sys.stderr)

def read_file(filename, datastructure):
    """ Reads data from filename and adds it to the datastructure
    (READ command) """
    if not os.path.isfile(filename):
        print(E_FILE_NOT_FOUND)
        return

    with open(filename, 'r', encoding='utf-8') as input_file:
        for line in input_file:
            input_line = line.split(SEPARATOR)
            # Skip invalid lines
            if len(input_line) < 5:
                continue
            rank = input_line[0]
            try:
                birth_year = int(input_line[1])
                enlisting_year = int(input_line[2])
            except ValueError as err:
                print(E_INT_EXPECTED)
                return
            shirt_color = input_line[3]
            name = SEPARATOR.join(input_line[4:]).rstrip('\n')
            datastructure.add(rank, birth_year, enlisting_year, shirt_color,
                              name)


def main():
    """ Main program contains a simple shell which handles input and uses the
    datastructure accordingly """

    ds = Datastructure()
    input_line = ''
    command = ''
    while command != QUIT:
        
        try:
            input_line = input(PROMPT).split()
        except EOFError as err:
            print("")
            return

        # Skip empty lines
        if len(input_line) == 0:
            continue
        command = input_line[0]
        
        if command == ADD and len(input_line) >= 6:
            rank = input_line[1]
            try:
                birth_year = int(input_line[2])
                enlisting_year = int(input_line[3])
            except ValueError as err:
                print(E_INT_EXPECTED)
                continue

            shirt_color = input_line[4]
            name = SEPARATOR.join(input_line[5:])

            print(command, rank, birth_year, enlisting_year,
                  shirt_color, name, sep=SEPARATOR)
            
            ds.add(rank, birth_year, enlisting_year, shirt_color, name)
        elif command == READ and len(input_line) == 2:
            filename = input_line[1]
            print(command, filename, sep=SEPARATOR)
            read_file(filename, ds)
        elif command == PRINT:
            print(command, sep=SEPARATOR)
            ds.print()
        elif command == MAXMIN and len(input_line) == 2:
            parameter = input_line[1]
            print(command, parameter, sep=SEPARATOR)
            if parameter == OLD:
                ds.oldest()
            elif parameter == YOUNG:
                ds.youngest()
        elif command == EMPTY:
            print(command, sep=SEPARATOR)
            ds.empty()
        elif command == GENERATE and len(input_line) == 2:
            amount = int(input_line[1])
            generate(ds, amount)
        elif command == EFFICIENCY and len(input_line) == 2:
            amount = int(input_line[1])
            efficiency_test(ds, amount)

        elif command == TEST:
            if len(input_line) == 2:
                count = int(input_line[1])
            else:
                count = 30
            test(ds, count)
    print(command)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt as err:
        print("")

