// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 2009 - Arnaud TORSET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function y=findDeps(x, deps)
//give the dependances of x
//

nbArgsIn = size(deps.x,'c');
//stocke deps.x dans y
y=deps.x;
for i=1:nbArgsIn, 
	//on boucle sur le nombre d'entrées afin de pouvoir extraire chaque éléments
	//l'element est stockédans deps(deps.a(i))
	for j=1:size(deps(deps.a(i)),'c'), 
		//on boucle sur les dependances de cet element afin de savoir s'il faut rajoutere des dependances
		nouveau = 0;
		for k=1:size(y,'c'), 
			//on boucle sur le nombre d'elements de y afin de faire une comparaison entre les elements 
			//presents et absents de la liste.
			//S'il est absent de la liste(nouveau=0), on le rajoute à y, sinon(nouveau=1) on fait rien
			if (deps(deps.a(i))(j)==deps.y(k)) then nouveau=nouveau+1;end;
		end;
		if (nouveau==0) then y(1,size(y,'c')+1)=deps(deps.a(i))(j);end;
	end;
end;

//si le nb de parametres du début est égal à celui de la fin alors on arrete, sinon on continue
nbArgsOut = size(deps.y,'c');
if (nbArgsOut>nbArgsIn) then findDeps(y, deps);
else if (nbArgsOut<nbArgsIn) then disp("error");end;
end;

endfunction




