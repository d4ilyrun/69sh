project(
  '42sh',
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
  include_directories('include'),
]

common = [
  'src/ast/ast.c',
  'src/lexer/lexer.c',
  'src/parser/parser.c',
  'src/builtins/builtins.c',
  'src/xalloc/xalloc.c',
]

build = executable(
  '42sh',
  common + [ 'src/main.c' ],
  include_directories: incdirs,
  build_always_stale: true
)
