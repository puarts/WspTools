//*****************************************************************************
// Application Class
//
//(c) 2003-2006 by Martin Christen. All Rights reserved.
//******************************************************************************/

#include "gl_application.h"
#include "GL/glew.h"

#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GLUT/GLUT.h>
#endif

using namespace wsp::opengl;

std::list<GlApplication*> GlApplication::s_app_instances_;

//-----------------------------------------------------------------------------

GlApplication::GlApplication(void)
{
   s_app_instances_.push_back(this);
   
   int argc = 1;
   char* argv = "CWCApp";
   
   glutInit(&argc, &argv);
}

//-----------------------------------------------------------------------------

GlApplication::~GlApplication(void)
{
   // remove this instance from application list
   for (std::list<GlApplication*>::iterator iter = s_app_instances_.begin(); iter != s_app_instances_.end(); ++iter)
   {
      if (*iter == this)
      {
         s_app_instances_.erase(iter);
         return;
      } 
   }
}

//-----------------------------------------------------------------------------

void GlApplication::Run(void)
{
   OnInit();
   MainLoop();
   OnExit();
}

//-----------------------------------------------------------------------------

bool GlApplication::MainLoop()
{
   glutMainLoop();
   return true;
}

//-----------------------------------------------------------------------------