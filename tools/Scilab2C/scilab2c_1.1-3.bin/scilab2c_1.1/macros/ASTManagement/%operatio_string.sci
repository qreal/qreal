function txt=%operatio_string(O)
//overloading function for "operation" type tlist string function
//this is a node of the AST
//fields:
//     operands: a list
//     operator: a string
  txt=['Operation'
       '   Operands:'
       '      '+objectlist2string(O.operands)
       '   Operator: '+O.operator
       'EndOperation'
      ]
endfunction