#ifndef HEADERS_H
#define HEADERS_H

#include "wx/wxprec.h"

#define USE_CANVAS_SET_FOCUS
//#include "Clipboard.h"
//#include "Book.h"
//#include "Objects.h"
//#include "UCIEngine.h"
//#include "ChessRules.h"
//#include "Repository.h"
//#include "ByteBoard.h"

//using namespace thc;

//Objects objs;


#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/app.h"
#include "wx/grid.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/artprov.h"
#include "wx/clipbrd.h"
#include "wx/image.h"
#include "wx/colordlg.h"
#include "wx/wxhtml.h"
#include "wx/imaglist.h"
#include "wx/dataobj.h"
#include "wx/dcclient.h"
#include "wx/bmpbuttn.h"
#include "wx/menu.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"

#include "wx/aui/aui.h"


#include "wx/defs.h"
#include "wx/dcclient.h"
#include "wx/wfstream.h"

#if wxUSE_ZLIB
#include "wx/zstream.h"
#endif
//#include <gl/glew.h>
//#include "wx/glcanvas.h"
//#include "dxfrenderer.h"
//#include "Image.h"
//#include "ARB_Multisample.h"
//#include "Drawable.h"

#ifdef __WXMSW__
#include "wx/msw/private.h"
#endif

//#include "MyGLCanvas.h"


#include "wx/imaglist.h"
#include "wx/listctrl.h"
#include "wx/timer.h"           // for wxStopWatch
#include "wx/colordlg.h"        // for wxGetColourFromUser
#include "wx/settings.h"
#include "wx/sysopt.h"
#include "wx/numdlg.h"
#include "wx/radiobut.h"
#include "wx/radiobox.h"
#include "wx/wrapsizer.h"
#include "wx/datectrl.h"
#include "wx/filepicker.h"

#include "wx/clipbrd.h"
//#include "lcdwindow.h"



#ifdef __DARWIN__
    #include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif

#include "wx/sstream.h"

#include "wx/richtext/richtextctrl.h"
#include "wx/richtext/richtextstyles.h"
#include "wx/richtext/richtextxml.h"
#include "wx/richtext/richtexthtml.h"
#include "wx/richtext/richtextformatdlg.h"
#include "wx/richtext/richtextsymboldlg.h"
#include "wx/richtext/richtextstyledlg.h"
#include "wx/richtext/richtextprint.h"



// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers


    #include "wx/log.h"
    #include "wx/frame.h"
    #include "wx/panel.h"

	#include "wx/utils.h"




    #include "wx/filedlg.h"
    #include "wx/choicdlg.h"

    #include "wx/button.h"
#include "wx/tglbtn.h"
    #include "wx/textctrl.h"
    #include "wx/listbox.h"

    #include "wx/sizer.h"


#include "wx/txtstrm.h"
#include "wx/numdlg.h"
#include "wx/textdlg.h"
#include "wx/ffile.h"
#include "wx/stopwatch.h"
#include "wx/dataview.h"

#include "wx/process.h"

#include "wx/mimetype.h"


#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/progdlg.h"

#include "wx/propgrid/propgrid.h"
#include "wx/propgrid/advprops.h"
#include "wx/propgrid/manager.h"

#include "wx/mediactrl.h"   //for wxMediaCtrl

#include "wx/gauge.h"


#include "wx/colordlg.h"
#include "wx/image.h"
#include "wx/artprov.h"
#include "wx/dcgraph.h"
#include "wx/overlay.h"
#include "wx/graphics.h"
#include "wx/filename.h"
#include "RenderTimer.h"


#include "wx/stdpaths.h"

#define wxUSE_GENERIC_DRAGIMAGE 1

#if wxUSE_GENERIC_DRAGIMAGE
#include "wx/generic/dragimgg.h"
#define wxDragImage wxGenericDragImage
#else
#include "wx/dragimag.h"
#endif

#include <wx/dcbuffer.h>


#define THEME_COLOR_A wxColour( 150, 150, 255, 128 )
#define THEME_COLOR_B wxColour( 128, 128, 128, 128 )
#define THEME_COLOR_C wxColour( 100, 100, 100, 128 )
#define THEME_COLOR_D *wxLIGHT_GREY
#define THEME_COLOR_E wxColour( 0, 140, 64, 128 )
#define THEME_COLOR_F wxColour( 0, 100, 54, 128 )
#endif