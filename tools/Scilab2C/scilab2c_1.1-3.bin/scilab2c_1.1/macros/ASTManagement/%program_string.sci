function txt=%program_string(p)
//overloading function for "program" type tlist string function
//main (root) node of the Abstract Formal Tree
//fields:
//      name      : string (the function name)
//      outputs   : list of "variable" type tlist (the output arg names)
//      inputs    : list of "variable" type tlist (the intput arg names)
//      statements: list of "equal" type tlist and list('EOL') (the
//                           instructions list)
//      nblines   : number (the number of lines in the scilab function)
  txt=['Program'
       'Name   : '+p.name
       'Outputs: '+strcat(objectlist2string(p.outputs),' ')
       'Inputs : '+strcat(objectlist2string(p.inputs),' ')
       'Statements '
       '    '+objectlist2string(p.statements)
       'EndProgram'
      ]
endfunction
