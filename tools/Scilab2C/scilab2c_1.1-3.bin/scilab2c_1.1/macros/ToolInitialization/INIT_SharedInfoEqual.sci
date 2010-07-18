function SharedInfo = INIT_SharedInfoEqual(SharedInfo)
// function SharedInfo = INIT_SharedInfoEqual(SharedInfo)
// -----------------------------------------------------------------
// #RNU_RES_B
// This function initializes the SCI2C tool according
// to the input parameters recorded in the SCI2CParameters.
// All info will be stored into FileInfoDatFile.
//
// Input data:
// SCI2CInputPrmFile: name of the .sce file containing input parameters.
//
// Output data:
// FileInfoDatFile: name of the .dat file containing the FileInfo structure.
// SharedInfoDatFile: it is a buffer containing parameters that are exchanged by the
//                    functions of the SCI2C tool.
// #RNU_RES_E
// Status:
// 13-Apr-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// #RNU_RES_B
//NUT: verifica se le variabili sotto elencate ti servono davvero.
//NUT: Sarebbe interessante accorpare tutte le variabili usate per la equal sotto un'unica struttura
//NUT: per esempio SharedInfo.SkipNextEqual
// #RNU_RES_E

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

SharedInfo.Equal.Enabled        = 0; // 1 means enabled -> we are inside an equal AST block.      
SharedInfo.Equal.NInArg         = 0;
SharedInfo.Equal.InArg(1).Name  = '';
SharedInfo.Equal.NOutArg        = 0;
SharedInfo.Equal.OutArg(1).Name = '';
SharedInfo.Equal.Lhs            = 0; 
SharedInfo.Equal.Nins           = 0; // Counts the number of ins found in the lhs of the Equal.

endfunction
