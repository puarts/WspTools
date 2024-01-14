//*****************************************************************************
// Application Class
//
//(c) 2003-2006 by Martin Christen. All Rights reserved.
//******************************************************************************/

#ifndef WSP_GUI_GL_APPLICATION_H
#define WSP_GUI_GL_APPLICATION_H

//! \defgroup app Application
#include <wsp/common/_define_commoncore.h>
#include <list>

namespace wsp {
    namespace opengl {
        //! \ingroup app
        class WSP_DLL_EXPORT GlApplication
        {
        public:
            GlApplication(void);
            virtual ~GlApplication(void);

            //! Return Name of Application or 0 if undefined.
            char* GetAppName(){ return 0; }

            //! Set Name of Application
            void SetAppName(char* sAppName);

            //! Starts Application (OnInit() -> MainLoop() -> OnExit())
            void Run();

            //! ShowConsole is for Windows only: Show a console where cout/printf etc are redirected. This should not be used
            // for productive applications, but is "nice to have" when developing certain small applications.
            void ShowConsole();

        protected:
            //! OnInit is called when application starts. This is for example a good place to create (main) windows.
            virtual void OnInit() {};

            //! OnExit is called when application terminates.
            virtual void OnExit() {};

            //! The mainloop of the application, override if you want to specify your own, which is not recommended in
            // most cases and may not even work properly with all implementations! [Basically reserved for future use]
            virtual bool MainLoop();

        protected:
            static std::list<GlApplication*> s_app_instances_;
        };
    }
}

#endif