function txt=%variable_string(v)
  global anscounter; //NUT: just to fix problem with ans variables.
//overloading function for "variable" type tlist string function
//fields: name
//this is a leaf of the AST
//NUT: changed here. For me %i is a number not a variable.
  if (v.name == "%T" | ...
      v.name == "%t" | ...
      v.name == "%F"| ...
      v.name == "%f"| ...
      v.name == "%nan"| ...
      v.name == "%inf"| ...
      v.name == "%e" | ...
      v.name == "%pi")
      txt=['Number_x: '+v.name];
   elseif (v.name == "%i")
      txt=['Number_X: '+v.name];
   else
     if (v.name == 'ans')
        anscounter = anscounter + 1;
        txt=['Variable: '+v.name+string(anscounter)];
     else
        txt=['Variable: '+v.name];
     end
  end
endfunction