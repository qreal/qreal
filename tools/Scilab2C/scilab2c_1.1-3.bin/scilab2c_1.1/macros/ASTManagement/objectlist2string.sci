function txt=objectlist2string(L)
//auxiliary function for conversion of a list of objects
//into a string vector
  txt=[];
  for o=L,
    if type(o)==15 then //EOL case
      txt=[txt;'<'+o(1)+'>'],
    else
      txt=[txt; string(o)],
    end
  end
  if txt==[] then txt='<empty>',end
endfunction
