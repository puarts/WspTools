//******************************************************************************/
// There are many implementations of IGlWindow, using glut, SDL, wxWidgets, QT.
//
// This is the FreeGLUT Version.
//
//(c) 2003-2006 by Martin Christen. All Rights reserved.
//******************************************************************************/

#include <GL/glew.h>

#ifdef _WIN32
#include <GL/freeglut.h>
#else
#include <GLUT/GLUT.h>
#endif

#include "glut_abstract_window.h"

#include <wsp/common/fn-debug.h>

#include <iostream>
#include <vector>

using namespace wsp::opengl;


//-----------------------------------------------------------------------------
bool GlutAbstractWindow::left_mouse_button_down_;

namespace {
	static std::list<GlutAbstractWindow*> s_window_instances;
	static int s_menu_id = 0;
}

namespace {
    struct MenuItemImpl
    {
        int id;
        void(*proc)(GlutAbstractWindow*);
        GlutAbstractWindow* window;
    };

    std::list<MenuItemImpl*> s_menu_items;

    int g_dragging_mouse_x = 0;
    int g_dragging_mouse_y = 0;

    /******************************************************************************
    Popup menu handler.
    ******************************************************************************/
    void PopupMenuHandler(int id)
    {
        for (std::list<MenuItemImpl*>::iterator iter = s_menu_items.begin(), end = s_menu_items.end();
        iter != end; ++iter)
        {
            MenuItemImpl* item = (*iter);
            if (item->id == id)
            {
                item->proc(item->window);
                break;
            }
        }
    }
}

GlutAbstractWindow::GlutAbstractWindow(int window_width, int window_height, int fps)
    : has_initialized_(false)
    , fps_(fps)
    , is_full_screen_(false)
    , init_window_width_(window_width)
    , init_window_height_(window_height)
{
    int argc = 1;
    char* argv = "CWCApp";
    glutInit(&argc, &argv);

    left_mouse_button_down_ = false;
    s_window_instances.push_back(this);

    //-------------------------------------------
    // #TODO: Use Parameters to setup this stuff: 
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    //--------------------------------------------

    window_id_ = glutCreateWindow("OpenGL Output");

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    glutDisplayFunc(sDisplay);
    glutReshapeFunc(sReshape);
    glutMouseFunc(sMouse);
    glutMotionFunc(sMotion);
#ifdef FREEGLUT
    glutMouseWheelFunc(sMouseWheel);
    glutCloseFunc(sClose);
#else
    glutWMCloseFunc(sClose);
#endif
    glutKeyboardFunc(sKeyDown);
    glutSpecialFunc(sSpecialDown);
    glutKeyboardUpFunc(sKeyUp);
    glutSpecialUpFunc(sSpecialUp);
    //glutIdleFunc(sIdle);
    glutTimerFunc(1000 / fps_, sTimer, 0);

    //Make main-menu
    main_menu_id_ = glutCreateMenu(PopupMenuHandler);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//-----------------------------------------------------------------------------

GlutAbstractWindow::~GlutAbstractWindow(void)
{
    for (std::list<MenuItemImpl*>::iterator iter = s_menu_items.begin(), end = s_menu_items.end();
    iter != end; ++iter)
    {
        MenuItemImpl* item = (*iter);
        delete item;
    }

    s_menu_items.clear();

    {
        std::list<GlutAbstractWindow*>::iterator i = s_window_instances.begin();
        while (i != s_window_instances.end())
        {
            if (*i == this)
            {
                s_window_instances.erase(i);
                break;
            }

            ++i;
        }
    }
}

void GlutAbstractWindow::Run()
{
    for (std::list<GlutAbstractWindow*>::iterator iter = s_window_instances.begin();
    iter != s_window_instances.end(); ++iter)
    {
        (*iter)->OnInit();
        (*iter)->has_initialized_ = true;
    }

    glutMainLoop();
}

void GlutAbstractWindow::AddContextMenuItem(const char* menu_title, void(*proc)(GlutAbstractWindow*))
{
    MenuItemImpl* menu_item = WSP_NEW MenuItemImpl();
    menu_item->id = s_menu_id;
    menu_item->proc = proc;
    menu_item->window = this;
    s_menu_items.push_back(menu_item);
    glutAddMenuEntry(menu_title, s_menu_id);

    ++s_menu_id;
}

void GlutAbstractWindow::AddContextMenuItem(
    const char* menu_title,
    std::vector<MenuItem>& sub_menus)
{
    int sub_menu_id = glutCreateMenu(PopupMenuHandler);

    for (std::vector<MenuItem>::iterator iter = sub_menus.begin(), end = sub_menus.end(); iter != end; ++iter)
    {
        MenuItem* item = &*iter;
        glutAddMenuEntry(item->title, s_menu_id);

        MenuItemImpl* menu_item = WSP_NEW MenuItemImpl();
        menu_item->id = s_menu_id;
        menu_item->proc = item->proc;
        menu_item->window = this;
        s_menu_items.push_back(menu_item);

        ++s_menu_id;
    }

    glutSetMenu(main_menu_id_);
    glutAddSubMenu(menu_title, sub_menu_id);
}

int GlutAbstractWindow::GetWindowWidth() const
{
    return glutGet(GLUT_WINDOW_WIDTH);
}

int GlutAbstractWindow::GetWindowHeight() const
{
    return glutGet(GLUT_WINDOW_HEIGHT);
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sClose(void)
{
    int current_window_id = glutGetWindow();

    for (std::list<GlutAbstractWindow*>::iterator iter = s_window_instances.begin();
    iter != s_window_instances.end(); ++iter)
    {
        if ((*iter)->window_id_ == current_window_id)
        {
            (*iter)->OnClose();
            delete (*iter);
            return;
        }
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sReshape(int w, int h)
{
    int CurrentWindow = glutGetWindow();

    for (std::list<GlutAbstractWindow*>::iterator window_iter = s_window_instances.begin(), end = s_window_instances.end();
    window_iter != end; ++window_iter)
    {
        GlutAbstractWindow* window = (*window_iter);
        if (window->window_id_ == CurrentWindow)
        {
            if (window->has_initialized_)
            {
                window->OnResize(w, h);
            }
        }
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sDisplay(void)
{
    int CurrentWindow = glutGetWindow();
    std::list<GlutAbstractWindow*>::iterator i = s_window_instances.begin();

    while (i != s_window_instances.end())
    {
        if ((*i)->window_id_ == CurrentWindow)
        {
            (*i)->OnRender();
        }

        ++i;
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sMouse(int button, int updown, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<GlutAbstractWindow*>::iterator i = s_window_instances.begin();

    for (; i != s_window_instances.end(); ++i)
    {
        if ((*i)->window_id_ != CurrentWindow)
        {
            continue;
        }

        if (!(*i)->has_initialized_)
        {
            continue;
        }

        if (updown)
        {
            if (button == 0)
                left_mouse_button_down_ = false;
            (*i)->OnMouseUp(button, x, y);
        }
        else
        {
            if (button == 0)
                left_mouse_button_down_ = true;
            (*i)->OnMouseDown(button, x, y);
        }
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sMotion(int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<GlutAbstractWindow*>::iterator i = s_window_instances.begin();

    while (i != s_window_instances.end())
    {
        if ((*i)->window_id_ == CurrentWindow)
        {
            if ((*i)->has_initialized_)
            {
                if (left_mouse_button_down_)
                {
                    //(*i)->OnLeftMouseDrag(x, y);
                    g_dragging_mouse_x = x;
                    g_dragging_mouse_y = y;
                }

                (*i)->OnMouseMove(x, y);
            }
        }

        ++i;
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sMouseWheel(int wheel_number, int direction, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<GlutAbstractWindow*>::iterator i = s_window_instances.begin();

    while (i != s_window_instances.end())
    {
        if ((*i)->window_id_ == CurrentWindow)
        {
            if ((*i)->has_initialized_)
            {
                (*i)->OnMouseWheel(wheel_number, direction, x, y);
            }
        }

        ++i;
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sJoystick(unsigned int a, int b, int c, int d)
{
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sSpecialUp(int key, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<GlutAbstractWindow*>::iterator i = s_window_instances.begin();

    while (i != s_window_instances.end())
    {
        if ((*i)->window_id_ == CurrentWindow)
        {
            if ((*i)->has_initialized_)
            {
                (*i)->OnKeyUp(key, 0);
            }
        }

        ++i;
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sKeyUp(unsigned char key, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<GlutAbstractWindow*>::iterator i = s_window_instances.begin();

    while (i != s_window_instances.end())
    {
        if ((*i)->window_id_ == CurrentWindow)
        {
            if ((*i)->has_initialized_)
            {
                (*i)->OnKeyUp(0, key);
            }
        }

        ++i;
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sSpecialDown(int key, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<GlutAbstractWindow*>::iterator i = s_window_instances.begin();

    while (i != s_window_instances.end())
    {
        if ((*i)->window_id_ == CurrentWindow)
        {
            if ((*i)->has_initialized_)
            {
                (*i)->OnKeyDown(key, 0);
            }
        }

        ++i;
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sKeyDown(unsigned char key, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<GlutAbstractWindow*>::iterator i = s_window_instances.begin();

    while (i != s_window_instances.end())
    {
        if ((*i)->window_id_ == CurrentWindow)
        {
            if ((*i)->has_initialized_)
            {
                (*i)->OnKeyDown(0, key);
            }
        }

        ++i;
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::sTimer(int value)
{
    int CurrentWindow = glutGetWindow();
    std::list<GlutAbstractWindow*>::iterator i = s_window_instances.begin();

    while (i != s_window_instances.end())
    {
        if ((*i)->window_id_ == CurrentWindow)
        {
            if ((*i)->has_initialized_)
            {
                (*i)->OnIdle();
            }

            if (left_mouse_button_down_)
            {
                (*i)->OnLeftMouseDrag(g_dragging_mouse_x, g_dragging_mouse_y);
            }
        }
        ++i;
    }

    GlutAbstractWindow* window = s_window_instances.back();
    glutTimerFunc(1000 / window->fps_, sTimer, value);
}

void GlutAbstractWindow::sIdle(void)
{
    int CurrentWindow = glutGetWindow();
    std::list<GlutAbstractWindow*>::iterator i = s_window_instances.begin();

    while (i != s_window_instances.end())
    {
        if ((*i)->window_id_ == CurrentWindow)
        {
            if ((*i)->has_initialized_)
            {
                (*i)->OnIdle();
            }

            if (left_mouse_button_down_)
            {
                (*i)->OnLeftMouseDrag(g_dragging_mouse_x, g_dragging_mouse_y);
            }
        }
        ++i;
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::Repaint()
{
    glutPostWindowRedisplay(window_id_);
}

//-----------------------------------------------------------------------------

void  GlutAbstractWindow::SetFullscreen(bool bFullscreen)
{
    if (bFullscreen)
    {
        glutFullScreen();
        is_full_screen_ = true;
    }
    else
    {
        int w = init_window_width_; // glutGet(GLUT_WINDOW_WIDTH);
        int h = init_window_height_; // glutGet(GLUT_WINDOW_HEIGHT);
        int x = glutGet(GLUT_WINDOW_X);
        int y = glutGet(GLUT_WINDOW_Y);

        glutPositionWindow(x, y);
        glutReshapeWindow(w, h);
        is_full_screen_ = false;
    }
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::Hide()
{
    glutHideWindow();
}

//-----------------------------------------------------------------------------

void GlutAbstractWindow::Show()
{
    glutShowWindow();
}

//-----------------------------------------------------------------------------   

void  GlutAbstractWindow::Close()
{
    glutDestroyWindow(window_id_);
}

//-----------------------------------------------------------------------------