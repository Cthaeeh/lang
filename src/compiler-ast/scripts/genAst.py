import sys
import argparse



def main (args):
    parser = argparse.ArgumentParser()
    parser.add_argument('-H', '--headers',
                        action='store_true',
                        help='output the names of the generated files to std::out.')

    parser.add_argument('-O', '--out',
                        help='path to the dersired output location.',
                        required = False)
    results = parser.parse_args(args)

    output_files = ['someHeader.h']

    if (results.headers) :
        sys.stdout.write(';'.join(output_files))

    if (results.out) :
        filename = results.out + "someHeader.h"
        with open(filename, 'w') as myfile:

            myfile.write('some cool header')





if __name__== "__main__":
    main(sys.argv[1:])
