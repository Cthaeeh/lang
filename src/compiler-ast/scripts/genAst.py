import sys
import argparse
import shutil
import os

from collections import namedtuple

Expression = namedtuple('Expression', 'name members')
Member = namedtuple('Member', 'type name')


def writeExpr(outPath, expression):
    filename = outPath + expression.name + ".h"
    with open(filename, 'w') as outFile:
        outFile.write('// auto-generated file\n\n')
        outFile.write('#ifndef ' + expression.name.upper() + '_H\n')
        outFile.write('#define ' + expression.name.upper() + '_H\n\n')
        outFile.write('#include <memory>\n')
        outFile.write('#include <Expr.h>\n\n')
        outFile.write('class ' + expression.name + "{\n\n")
        outFile.write('};\n\n')
        outFile.write('#endif')



def writeHeaders(expressions):
    output_files = []

    for node in expressions:
        output_files += (node.name + ".h")

    sys.stdout.write(';'.join(output_files))


def main(args):
    parser = argparse.ArgumentParser()
    parser.add_argument('-H', '--headers',
                        action='store_true',
                        help='output the names of the generated files to std::out.')

    parser.add_argument('-O', '--out',
                        help='path to the dersired output location.'
                             'WARNING: will delete all files in that folder.',
                        required=False)
    results = parser.parse_args(args)

    expressions = [Expression('BinaryExpr', [Member('Expression', 'lhs'),
                                             Member('Expression', 'rhs'),
                                             Member('Token', 'op')]),
                   Expression('UnaryExpr', [Member('Expression', 'lhs'),
                                            Member('Token', 'op')])]

    # If headers flag is set write all headers that this script will generate to std::cout.
    if (results.headers):
        writeHeaders(expressions)

    # If an outpath is specified write the headers.
    if (results.out):
        shutil.rmtree(results.out)
        os.makedirs(results.out)
        for expression in expressions:
            writeExpr(results.out, expression)


if __name__ == "__main__":
    main(sys.argv[1:])
