function cardvalue = ReadStringCard(filename,cardname,commentdelim,enableerror)
// function cardvalue = ReadStringCard(filename,cardname,commentdelim,enableerror)
// -----------------------------------------------------------------
// #RNU_RES_B
// Reads the string associated to the card cardname placed 
// in filename.
// The value of cardname is assumed to be a string.
// If the card is not found an error will occur.
// 
// Input data:
// filename: full path + name of the file where the card
//           is being searched.
// cardname: string with the name of the card.
// commentdelim: specifies a character for an eventual comment
//               (to be discarded) after the card value.
// enableerror: 'y' enable error message.
//              'n' enable warning message.
//
// Output data:
// cardvalue: string associated to the card. Blanks characters 
//            are discarded.
// #RNU_RES_E
//
// Status:
// 06-Feb-2004 -- Nutricato Raffaele: Author.
// 06-Feb-2004 -- Nutricato Raffaele: TEST OK.
// 25-Jun-2004 -- Nutricato Raffaele: Added Comment delimiter
//                                    and enableerror as input parameter.
// 13-Apr-2007 -- Intelligente Fabio: Rewritten from Matlab to Scilab.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,3);

if argn(2) == 2 then
   commentdelim = ' ';
   enableerror  = 'y';
      
elseif argn(2) == 3 then
   enableerror  = 'y';
end

[flag_found,requested_line,dummy2] = ... 
   KeyString2FileStringPos(filename,cardname,'cut');
cardvalue = stripblanks(strtok(requested_line,commentdelim));
clear requested_line dummy2

if (flag_found == 0) then
   if (enableerror == 'y') then
      SCI2Cerror([cardname,' not found']); 
   else
      warning([cardname,' not found']);
   end
end
 
endfunction
