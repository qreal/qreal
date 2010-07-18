function txt=%equal_string(e)
//overloading function for "equal" type tlist string function
//this is a node of the AST

//fields:
//   expression: "expression" type tlist (the right hand side)
//   lhs       : list of "variable" type tlist and "operation" type tlist //   (the assignment)
//   endsymbol : string (the orginal end-of-instruction symbol (, ; <CR>))
  txt=['Equal'
       '  Expression: '
       '    '+string(e.expression)
       '  Lhs       : '
       '     '+objectlist2string(e.lhs)
       'EndEqual'
      ]
endfunction