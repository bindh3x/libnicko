/*
 * Copyright (c) 2018 bindh3x <os@bindh3x.io>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <string.h>
#include <tcl.h>

#include <nicko.h>

static int Nicko_Cmd(ClientData cdata,
		     Tcl_Interp *interp,
		     int objc,
		     Tcl_Obj * const objv[])
{
  struct nicko_magic *m = NULL;
  const char *name = NULL;
  int len = 0;
  Tcl_Obj *dict = NULL;

  if (objc != 2) {
    Tcl_WrongNumArgs(interp, 0, objv, "nicko filename");
    return TCL_ERROR;
  }
  name = Tcl_GetStringFromObj(objv[1], &len);

  if (nicko(name, &m) < 0) {
    Tcl_SetResult(interp, (char *)Tcl_PosixError(interp), NULL);
    return TCL_ERROR;
  }

  if (m == NULL) {
    Tcl_SetResult(interp, "nicko: no match found", NULL);
    return TCL_ERROR;
  }

  dict = Tcl_NewDictObj();
  Tcl_DictObjPut(interp,
		 dict,
	         Tcl_NewStringObj("name", 4),
		 Tcl_NewStringObj(m->name,
                                  strlen(m->name)));
  Tcl_DictObjPut(interp,
		 dict,
	         Tcl_NewStringObj("group", 5),
		 Tcl_NewStringObj(nicko_get_group_name(m->group),
			          strlen(nicko_get_group_name(m->group))));
  Tcl_DictObjPut(interp,
		 dict,
	         Tcl_NewStringObj("size", 4),
		 Tcl_NewIntObj(m->size));
  Tcl_DictObjPut(interp,
		 dict,
	         Tcl_NewStringObj("offset", 6),
		 Tcl_NewIntObj(m->offset));
  Tcl_SetObjResult(interp, dict);

  return TCL_OK;
}

int Nicko_Init(Tcl_Interp *interp)
{

  if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
         return TCL_ERROR;
  }

  if (Tcl_PkgProvide(interp, "Nicko", "1.0") == TCL_ERROR) {
    return TCL_ERROR;
  }

  Tcl_CreateObjCommand(interp, "nicko", Nicko_Cmd, NULL, NULL);

  return TCL_OK;
}
