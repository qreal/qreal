/* $Id: gvc.h,v 1.66 2009/10/04 20:15:42 ellson Exp $ $Revision: 1.66 $ */
/* vim:set shiftwidth=4 ts=8: */

/**********************************************************
*      This software is part of the graphviz package      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2004 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
**********************************************************/

#ifndef			GVC_H
#define			GVC_H

#include "types.h"
#include "gvplugin.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef GVDLL
#define extern __declspec(dllexport)
#else
#define extern
#endif

/*visual studio*/
#ifdef WIN32_DLL
#ifndef GVC_EXPORTS
#define extern __declspec(dllimport)
#endif
#endif
/*end visual studio*/
	
/* misc */
/* FIXME - this needs eliminating or renaming */
extern void gvToggle(int);

/* set up a graphviz context */
extern GVC_t *gvNEWcontext(const lt_symlist_t *builtins, int demand_loading);

/*  set up a graphviz context - and init graph - retaining old API */
extern GVC_t *gvContext(void);
/*  set up a graphviz context - and init graph - with builtins */
extern GVC_t *gvContextPlugins(const lt_symlist_t *builtins, int demand_loading);

/* get information associated with a graphviz context */
extern char **gvcInfo(GVC_t*);
extern char *gvcVersion(GVC_t*);
extern char *gvcBuildDate(GVC_t*);

/* parse command line args - minimally argv[0] sets layout engine */
extern int gvParseArgs(GVC_t *gvc, int argc, char **argv);
extern graph_t *gvNextInputGraph(GVC_t *gvc);
extern graph_t *gvPluginsGraph(GVC_t *gvc);

/* Compute a layout using a specified engine */
extern int gvLayout(GVC_t *gvc, graph_t *g, const char *engine);

/* Compute a layout using layout engine from command line args */
extern int gvLayoutJobs(GVC_t *gvc, graph_t *g);

/* Render layout into string attributes of the graph */
extern void attach_attrs(graph_t *g);

/* Parse an html string */
#ifndef WITH_CGRAPH
extern char *agstrdup_html(char *s);
#endif
extern int aghtmlstr(char *s);

/* Render layout in a specified format to an open FILE */
extern int gvRender(GVC_t *gvc, graph_t *g, const char *format, FILE *out);

/* Render layout in a specified format to an open FILE */
extern int gvRenderFilename(GVC_t *gvc, graph_t *g, const char *format, const char *filename);

/* Render layout in a specified format to an external context */
extern int gvRenderContext(GVC_t *gvc, graph_t *g, const char *format, void *context);

/* Render layout in a specified format to a malloc'ed string */
extern int gvRenderData(GVC_t *gvc, graph_t *g, const char *format, char **result, unsigned int *length);

/* Render layout according to -T and -o options found by gvParseArgs */
extern int gvRenderJobs(GVC_t *gvc, graph_t *g);

/* Clean up layout data structures - layouts are not nestable (yet) */
extern int gvFreeLayout(GVC_t *gvc, graph_t *g);

/* Clean up graphviz context */
extern void gvFinalize(GVC_t *gvc);
extern int gvFreeContext(GVC_t *gvc);

/** Add a library from your user application
 * @param gvc Graphviz context to add library to
 * @param lib library to add
 */
extern void gvAddLibrary(GVC_t *gvc, gvplugin_library_t *lib);

#undef extern

#ifdef __cplusplus
}
#endif

#endif			/* GVC_H */
