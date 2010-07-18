function [flag_found,requested_line,line_position] = KeyStr2FileStrPos(filename,key_string,method)
// function [flag_found,requested_line,line_position] = KeyStr2FileStrPos(filename,key_string,method)
// --------------------------------------------------------------------------------
// #RNU_RES_B
// This function returns a line and its position from a specified ASCII file. 
// The line and the position returned starts with a key string specified in the 
// input parameters.
//
// Input data:
// filename: path + name of the ASCII file.
// key_string: string that specifies the initial portion of the line to return. 
// method: 'cut': in the returned line will be removed the key_string
//         'no_cut': (default), in the returned line will not be removed the key_string
//
// Output data:
// flag_found: 0 if the line is not found or an error occured.
//             1 if the search succeed.
// requested_line: is the line in the file which contains as first characters those
//                 specified in the key_string.
// line_position: position of the line in the file; the first line in the file
//                is the line number 1.
// #RNU_RES_E
// 
//
// Status:
// 08-Jul-2002 -- Author: Raffaele Nutricato
// 08-Jul-2002 -- Raffaele Nutricato: Revision OK
// 23-Nov-2004 -- Raffaele Nutricato: Changed disp to warning in if (flag_found == 0).
//                                    It allows to disable the message it generates
//                                    by using warning off.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,3);


if (argn(2) == 2)
   method = 'no_cut';    
end
method = convstr(method, 'u');

// Initialize output parameters
flag_found = 0;
requested_line = '';
line_position = 0;

// Open the text file (read only)
[fid,mess] = mopen(filename,'r');
if ( fid == -1 )
   disp(['Cannot open: '+filename])
   disp(mess);
   flag_found = 0;
   return;
end

// loop on the lines of the file
num_chars = length(key_string);
while (meof(fid) == 0)
   check_string = fgetl(fid);
   line_position = line_position +  1;
   if (key_string == check_string) & (key_string == num_chars) then
      flag_found = 1;
      requested_line = check_string;
      if (method =='cut') then
         requested_line(1:num_chars) = [];   
      end
      mclose(fid);
      return;
   end
end

if (flag_found == 0)
   warning('Warning: string ' + key_string + ' not found in file: ' + filename);
   mclose(fid);
end
   
mclose(fid);
endfunction
