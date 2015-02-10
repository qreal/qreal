// Scintilla source code edit control
/** @file ExternalLexer.cxx
 ** Support external lexers in DLLs.
 **/
// Copyright 2001 Simon Steele <ss@pnotepad.org>, portions copyright Neil Hodgson.
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include <string>

#include "Platform.h"

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "LexerModule.h"
#include "Catalogue.h"
#include "ExternalLexer.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

LexerManager *LexerManager::theInstance = nullptr;

//------------------------------------------
//
// ExternalLexerModule
//
//------------------------------------------

void ExternalLexerModule::SetExternal(GetLexerFactoryFunction fFactory, int index) {
	fneFactory = fFactory;
	fnFactory = fFactory(index);
}

//------------------------------------------
//
// LexerLibrary
//
//------------------------------------------

LexerLibrary::LexerLibrary(const char *ModuleName) {
	// Initialise some members...
	first = nullptr;
	last = nullptr;

	// Load the DLL
	lib = DynamicLibrary::Load(ModuleName);
	if (lib->IsValid()) {
		m_sModuleName = ModuleName;
		//Cannot use reinterpret_cast because: ANSI C++ forbids casting between pointers to functions and objects
		GetLexerCountFn GetLexerCount = (GetLexerCountFn)(sptr_t)lib->FindFunction("GetLexerCount");

		if (GetLexerCount) {
			ExternalLexerModule *lex;
			LexerMinder *lm;

			// Find functions in the DLL
			GetLexerNameFn GetLexerName = (GetLexerNameFn)(sptr_t)lib->FindFunction("GetLexerName");
			GetLexerFactoryFunction fnFactory = (GetLexerFactoryFunction)(sptr_t)lib->FindFunction("GetLexerFactory");

			// Assign a buffer for the lexer name.
			char lexname[100];
			strcpy(lexname, "");

			int nl = GetLexerCount();

			for (int i = 0; i < nl; i++) {
				GetLexerName(i, lexname, 100);
				lex = new ExternalLexerModule(SCLEX_AUTOMATIC, nullptr, lexname, nullptr);
				Catalogue::AddLexerModule(lex);

				// Create a LexerMinder so we don't leak the ExternalLexerModule...
				lm = new LexerMinder;
				lm->self = lex;
				lm->next = nullptr;
				if (first != nullptr) {
					last->next = lm;
					last = lm;
				} else {
					first = lm;
					last = lm;
				}

				// The external lexer needs to know how to call into its DLL to
				// do its lexing and folding, we tell it here.
				lex->SetExternal(fnFactory, i);
			}
		}
	}
	next = nullptr;
}

LexerLibrary::~LexerLibrary() {
	Release();
	delete lib;
}

void LexerLibrary::Release() {
	LexerMinder *lm;
	LexerMinder *lmNext;
	lm = first;
	while (nullptr != lm) {
		lmNext = lm->next;
		delete lm->self;
		delete lm;
		lm = lmNext;
	}

	first = nullptr;
	last = nullptr;
}

//------------------------------------------
//
// LexerManager
//
//------------------------------------------

/// Return the single LexerManager instance...
LexerManager *LexerManager::GetInstance() {
	if (!theInstance)
		theInstance = new LexerManager;
	return theInstance;
}

/// Delete any LexerManager instance...
void LexerManager::DeleteInstance() {
	delete theInstance;
	theInstance = nullptr;
}

/// protected constructor - this is a singleton...
LexerManager::LexerManager() {
	first = nullptr;
	last = nullptr;
}

LexerManager::~LexerManager() {
	Clear();
}

void LexerManager::Load(const char *path) {
	LoadLexerLibrary(path);
}

void LexerManager::LoadLexerLibrary(const char *module) {
	for (LexerLibrary *ll = first; ll; ll= ll->next) {
		if (strcmp(ll->m_sModuleName.c_str(), module) == 0)
			return;
	}
	LexerLibrary *lib = new LexerLibrary(module);
	if (nullptr != first) {
		last->next = lib;
		last = lib;
	} else {
		first = lib;
		last = lib;
	}
}

void LexerManager::Clear() {
	if (nullptr != first) {
		LexerLibrary *cur = first;
		LexerLibrary *next;
		while (cur) {
			next = cur->next;
			delete cur;
			cur = next;
		}
		first = nullptr;
		last = nullptr;
	}
}

//------------------------------------------
//
// LexerManager
//
//------------------------------------------

LMMinder::~LMMinder() {
	LexerManager::DeleteInstance();
}

LMMinder minder;
