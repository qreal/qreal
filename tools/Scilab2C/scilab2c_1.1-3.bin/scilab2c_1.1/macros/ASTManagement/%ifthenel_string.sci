function txt=%ifthenel_string(I)
//overloading function for "ifthenel" type tlist string function
//this is a node of the AST
//fields:
//    expression  : "expression" type tlist (the if expression)
//    then        : list of "equal" type tlist and list('EOL') (the
//                           then instructions list)
//    elseifs     : a list of tlists
//    else        : list of "equal" type tlist and list('EOL') (the
//                           else instructions list)
  txt=['If '
       '  Expression:'
       '     '+string(I.expression)
       '  If Statements'
       '    '+objectlist2string(I.then)]
  for e=I.elseifs
    txt=[txt;
	 '  Else If Expression'
	 '     '+string(e.expression)
	 '  Else If Statements'
       '    '+objectlist2string(e.then)]
  end
  txt=[txt;
       '  Else Statements'
       '    '+objectlist2string(I.else)
       'EndIf']
endfunction
