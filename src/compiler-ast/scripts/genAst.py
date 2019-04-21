import sys
import argparse
import shutil
import os

from collections import namedtuple


#TODO this is horrible but whatever.

Expression = namedtuple('Expression', 'name members')
Member = namedtuple('Member', 'type name')


def writeExpr(outPath, expression):
    filename = outPath + expression.name + ".h"
    with open(filename, 'w') as outFile:
        outFile.write('// auto-generated file\n\n')
        outFile.write('#ifndef ' + expression.name.upper() + '_H\n')
        outFile.write('#define ' + expression.name.upper() + '_H\n\n')
        outFile.write('#include <memory>\n')
        outFile.write('#include <../Expr.h>\n')
        outFile.write('#include <../Token.h>\n\n')
        outFile.write("class " + expression.name + ";\n\n")
        outFile.write("typedef std::shared_ptr<" + expression.name + "> " + expression.name + "Ptr;\n\n")
        outFile.write('class ' + expression.name + ": public Expr {\n\n")

        outFile.write('public:\n')
        outFile.write('static ExprPtr  make(')

        for m_counter, member in enumerate(expression.members):
            if m_counter > 0:
                outFile.write(',')
            outFile.write(member.type + ' ' + member.name)

        outFile.write(') {\n')

        outFile.write('    struct make_shared_enabler_' + expression.name + ' : public ' + expression.name + ' { make_shared_enabler_' + expression.name + '(')

        for m_counter, member in enumerate(expression.members):
            if m_counter > 0:
                outFile.write(',')
            outFile.write(member.type + ' ' + member.name)
        outFile.write(') :' + expression.name + '(')

        for m_counter, member in enumerate(expression.members):
            if m_counter > 0:
                outFile.write(',')
            outFile.write(member.name)

        outFile.write(') {}};\n')

        outFile.write('    auto e = std::make_shared<make_shared_enabler_' + expression.name + '>(')

        for m_counter, member in enumerate(expression.members):
            if m_counter > 0:
                outFile.write(',')
            outFile.write(member.name)
        outFile.write(');\n')

        for member in expression.members:
            if member.type == 'ExprPtr':
                outFile.write('    ' + member.name + "->parent = e;\n")

        outFile.write('    return e;\n}\n\n')
        outFile.write('virtual void accept(Visitor& v) override\n{\n    v.visit(*this);\n}\n\n')

        for member in expression.members:
            outFile.write(member.type + " " + member.name + ";\n")

        outFile.write('private:\n\n')

        outFile.write(expression.name + '(')

        for m_counter, member in enumerate(expression.members):
            if m_counter > 0:
                outFile.write(',')
            outFile.write(member.type + ' ' + member.name )
        outFile.write(') : ')

        for m_counter, member in enumerate(expression.members):
            if m_counter > 0:
                outFile.write(',')
            outFile.write(member.name +'(' +member.name +')')
        outFile.write('{ }\n\n')

        outFile.write('};\n\n')
        outFile.write('#endif')


def writeHeaders(expressions):
    output_files = set()

    for node in expressions:
        output_files.add(node.name + ".h")

    output_files.add("Visitor.h")

    sys.stdout.write(';'.join(output_files))


def writeVisitor(outPath, expressions):
    className = "Visitor"
    filename = outPath + className + ".h"
    with open(filename, 'w') as outFile:
        outFile.write('// auto-generated file\n\n')
        outFile.write('#ifndef ' + className.upper() + '_H\n')
        outFile.write('#define ' + className.upper() + '_H\n\n')
        outFile.write('#include <memory>\n')

        for expr in expressions:
            outFile.write("class " + expr.name + ";\n")

        outFile.write('class ' + className + "{\n\n")
        outFile.write('public:\n')

        for expr in expressions:
            outFile.write(
                "    virtual void visit(" + expr.name + "&" + " " + expr.name[0].lower() + expr.name[1:] + ") = 0;\n\n")

        outFile.write('};\n\n')
        outFile.write('#endif')


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

    expressions = [Expression('BinaryExpr', [Member('ExprPtr', 'left'),
                                             Member('Token', 'op'),
                                             Member('ExprPtr', 'right')]),
                   Expression('UnaryExpr', [Member('ExprPtr', 'left'),
                                            Member('Token', 'op')]),
                   Expression('LiteralExpr', [Member('Token', 'literal')])]

    # If headers flag is set write all headers that this script will generate to std::cout.
    if (results.headers):
        writeHeaders(expressions)

    # If an outpath is specified write the headers.
    if (results.out):
        shutil.rmtree(results.out)
        os.makedirs(results.out)
        for expression in expressions:
            writeExpr(results.out, expression)
        writeVisitor(results.out, expressions)


if __name__ == "__main__":
    main(sys.argv[1:])
