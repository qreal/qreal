function txt=%while_string(W)
//overloading function for "while" type tlist string function
//this is a node of the AST
//fields:
//   expression : "expression" type tlist (the loop expression)
//   statements : list of "equal" type tlist and list('EOL') (the
//                           while instructions list)
  txt=['While'
       '  WhileExpression:'
       '    '+string(W.expression)
       '  WhileStatements:'
       '    '+objectlist2string(W.statements)
       'EndWhile']
endfunction
