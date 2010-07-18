function txt=%comment_string(e)
//overloading function for "comment" type tlist string function
//fields:
//  text: a string
//this is a leaf of the AST
  txt=['Comment : '+e.text]
endfunction
