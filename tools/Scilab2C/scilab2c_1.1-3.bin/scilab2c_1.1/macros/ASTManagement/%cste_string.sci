function txt=%cste_string(c)
//overloading function for "cste" type tlist string function
//this is a leaf of the AST
//fields:
//    value : a number or a string
//NUT: added cste I also need "" for strings in order to be sure that the blanks are
//NUT: correctly considered and not mistaken with additional blanks present in the ast text file.
  stringcvalue = string(c.value);
  if (stringcvalue == "%T"     | ...
      stringcvalue == "%t"     | ...
      stringcvalue == "%F"     | ...
      stringcvalue == "%f"     | ...
      stringcvalue == "%nan"     | ...
      stringcvalue == "%inf"     | ...
      stringcvalue == "%e"	 | ...
      stringcvalue == "%pi")
     txt=['Number_x: '+stringcvalue];
  elseif (SCI2Cisnum(stringcvalue))
     //NUT needed to convert format 1D-14 into 1d-14
     txt=['Number_x: '+strsubst(stringcvalue,'D','e')];
  elseif (stringcvalue == "%i")
     txt=['Number_X: '+stringcvalue];
  else
     txt=['String: ""'+stringcvalue+'""'];
  end
endfunction