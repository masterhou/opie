//========================================================================
//
// FormWidget.cc
//
// Copyright 2000 Derek B. Noonburg
//
//========================================================================

#ifdef __GNUC__
#pragma implementation
#endif

#include <aconf.h>
#include "gmem.h"
#include "Object.h"
#include "Gfx.h"
#include "FormWidget.h"

//------------------------------------------------------------------------
// FormWidget
//------------------------------------------------------------------------

FormWidget::FormWidget(XRef *xrefA, Dict *dict) {
  Object apObj, asObj, obj1, obj2;
  fouble t;

  ok = gFalse;
  xref = xrefA;

  if (dict->lookup("AP", &apObj)->isDict()) {
    if (dict->lookup("AS", &asObj)->isName()) {
      if (apObj.dictLookup("N", &obj1)->isDict()) {
	if (obj1.dictLookupNF(asObj.getName(), &obj2)->isRef()) {
	  obj2.copy(&appearance);
	  ok = gTrue;
	}
	obj2.free();
      }
      obj1.free();
    } else {
      if (apObj.dictLookupNF("N", &obj1)->isRef()) {
	obj1.copy(&appearance);
	ok = gTrue;
      }
      obj1.free();
    }
    asObj.free();
  }
  apObj.free();

  if (dict->lookup("Rect", &obj1)->isArray() &&
      obj1.arrayGetLength() == 4) {
    //~ should check object types here
    obj1.arrayGet(0, &obj2);
    xMin = obj2.getNum();
    obj2.free();
    obj1.arrayGet(1, &obj2);
    yMin = obj2.getNum();
    obj2.free();
    obj1.arrayGet(2, &obj2);
    xMax = obj2.getNum();
    obj2.free();
    obj1.arrayGet(3, &obj2);
    yMax = obj2.getNum();
    obj2.free();
    if (xMin > xMax) {
      t = xMin; xMin = xMax; xMax = t;
    }
    if (yMin > yMax) {
      t = yMin; yMin = yMax; yMax = t;
    }
  } else {
    //~ this should return an error
    xMin = yMin = 0;
    xMax = yMax = 1;
  }
  obj1.free();
}

FormWidget::~FormWidget() {
  appearance.free();
}

void FormWidget::draw(Gfx *gfx) {
  Object obj;

  if (appearance.fetch(xref, &obj)->isStream()) {
    gfx->doWidgetForm(&obj, xMin, yMin, xMax, yMax);
  }
  obj.free();
}

//------------------------------------------------------------------------
// FormWidgets
//------------------------------------------------------------------------

FormWidgets::FormWidgets(XRef *xref, Object *annots) {
  FormWidget *widget;
  Object obj1, obj2;
  int size;
  int i;

  widgets = NULL;
  size = 0;
  nWidgets = 0;

  if (annots->isArray()) {
    for (i = 0; i < annots->arrayGetLength(); ++i) {
      if (annots->arrayGet(i, &obj1)->isDict()) {
	obj1.dictLookup("Subtype", &obj2);
	if (obj2.isName("Widget") ||
	    obj2.isName("Stamp")) {
	  widget = new FormWidget(xref, obj1.getDict());
	  if (widget->isOk()) {
	    if (nWidgets >= size) {
	      size += 16;
	      widgets = (FormWidget **)grealloc(widgets,
						size * sizeof(FormWidget *));
	    }
	    widgets[nWidgets++] = widget;
	  } else {
	    delete widget;
	  }
	}
	obj2.free();
      }
      obj1.free();
    }
  }
}

FormWidgets::~FormWidgets() {
  int i;

  for (i = 0; i < nWidgets; ++i) {
    delete widgets[i];
  }
  gfree(widgets);
}
