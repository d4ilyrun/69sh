project(
  'example',
  'c',
  version : '1.0',
  default_options: [
    'c_std=c99',
    'warning_level=3',
    'werror=true',
  ],
)

cflags = ['-D_GNU_SOURCE', '-D_POSIX_C_SOURCE=200809L']
add_project_arguments(cflags, language: 'c')

incdirs = [
  include_directories('src/ast'),
  include_directories('src/utils'),
  include_directories('src/lexer'),
  include_directories('src/parser'),
]

common = [
  'src/ast/ast.c',
  'src/lexer/lexer.c',
  'src/lexer/token.c',
  'src/parser/parser.c',
]

token_printer = executable(
    # name of the output executable
    'token_printer',
    # source files for the executable
    common + [ 'src/eval/token_printer.c' ],
    # these are passed as -I
    include_directories: incdirs
)

ast_print = executable(
    # name of the output executable
    'ast_print',
    # source files for the executable
    common + [ 'src/eval/ast_print.c' ],
    # these are passed as -I
    include_directories: incdirs
)

rpn_print = executable(
    # name of the output executable
    'rpn_print',
    # source files for the executable
    common + [ 'src/eval/rpn_print.c' ],
    # these are passed as -I
    include_directories: incdirs
)
