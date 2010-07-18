function txt=%for_string(F)
//overloading function for "for" type tlist string function
//this is a node of the AST
//fields:
//   expression : "expression" type tlist (the loop expression)
//   statements : list of "equal" type tlist and list('EOL') (the
//                           for instructions list)
//NUT: raf cambiato ForExpression e ForStatements
  txt=['For'
       '  ForExpression:'
       '    '+string(F.expression)
       '  ForStatements:'
       '    '+objectlist2string(F.statements)
       'EndFor']
endfunction