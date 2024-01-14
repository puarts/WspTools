/**
 * @file wspfx-abst_mdisubwin.hpp
 * @author Jun-ichi Nishikata
 */

#ifndef __WSPFX_ABST_MDI_SUB_WIN_HPP__
#define __WSPFX_ABST_MDI_SUB_WIN_HPP__

#include <vector>

#include <QtGui/QtGui>
#include "_define_gui.h"



namespace wsp{
    namespace gui{
        WSP_DLL_GLOBAL_VAR std::vector<s32> g_window_stack_;
    }

    class WSP_DLL_EXPORT QtAbstractMdiSubWin
        : public QMdiSubWindow
    {
        Q_OBJECT
    public:
        QtAbstractMdiSubWin(QWidget *parent=NULL)
            : QMdiSubWindow(parent)
            , id_(reinterpret_cast<s32>(this))
            , win_type_(wsp::gui::WINDOW_TYPE_UNKNOWN)
        {}
        virtual ~QtAbstractMdiSubWin(){}

        s32                  id      () const{ return id_; }
        wsp::gui::WindowType win_type() const{ return win_type_; }

        void set_id( s32 id ){ id_ = id; }

    signals:
        void signal_destroyed( s32 );

    protected:
        void updateWindowStack()
        {
            WSP_COMMON_LOG("  focus window id = %d\n", id_);
            WSP_COMMON_LOG("  stack:\n");
            wsp::gui::g_window_stack_.push_back( id_ );
            int stack_size = wsp::gui::g_window_stack_.size();
            for( int i=0; i<stack_size; ++i )
            {
                WSP_COMMON_LOG("    %d\n", (wsp::gui::g_window_stack_)[i]);
            }
        }
        virtual void closeEvent(QCloseEvent *event)
        {
            emit signal_destroyed(id_);
            event->accept();
        }
        //virtual void focusInEvent ( QFocusEvent * event )
        //{
        //    WSP_COMMON_DEBUG_LOG("\n");
        //    //updateWindowStack();
        //    event->accept();
        //}
        //virtual void mousePressEvent ( QMouseEvent * event )
        //{
        //    WSP_COMMON_DEBUG_LOG("\n");
        //    //updateWindowStack();
        //    event->accept();
        //}

    protected:
        s32 id_;
        wsp::gui::WindowType win_type_;

    };


    class WSP_DLL_EXPORT QtAbstractDialog
        : public QDialog
    {
        Q_OBJECT
    public:
        QtAbstractDialog(QWidget *parent=NULL)
            : QDialog(parent)
            , id_(reinterpret_cast<s32>(this))
            , win_type_(wsp::gui::WINDOW_TYPE_UNKNOWN)
        {}
        ~QtAbstractDialog(){}

        s32                  id      () const{ return id_; }
        wsp::gui::WindowType win_type() const{ return win_type_; }

        void set_id( s32 id ){ id_ = id; }

    signals:
        void signal_destroyed( s32 );

    protected:
        virtual void closeEvent(QCloseEvent *event)
        {
            emit signal_destroyed(id_);
            event->accept();
        }

    protected:
        s32 id_;
        wsp::gui::WindowType win_type_;

    };



    template<typename _QtWidgetType, wsp::gui::WindowType _WinType=wsp::gui::WINDOW_TYPE_GENERAL_VIEW>
    class QtMdiSubWindowWapper
        : public QtAbstractMdiSubWin
    {
    public:
        QtMdiSubWindowWapper(QWidget *parent=NULL)
            : QtAbstractMdiSubWin(parent)
        { 
            this->setWidget( &widget_data_ ); 
            win_type_ = _WinType;
        }
        virtual ~QtMdiSubWindowWapper(){}

        /* getter */
        _QtWidgetType*       widget_data()      { return &widget_data_; }
        virtual QSize sizeHint() const{ return widget_data_.sizeHint(); }
    private:
        _QtWidgetType widget_data_;
    };


    template<typename _QtWidgetType, wsp::gui::WindowType _WinType=wsp::gui::WINDOW_TYPE_GENERAL_VIEW>
    class QtDialogWapper
        : public QtAbstractDialog
    {
    public:
        QtDialogWapper(QWidget *parent=NULL)
            : QtAbstractDialog(parent)
            , main_layout_( new QVBoxLayout() )
            , widget_data_( new _QtWidgetType() )
        { 
            main_layout_->addWidget( widget_data_ );
            this->setLayout( main_layout_ ); 
            win_type_ = _WinType;
        }
        ~QtDialogWapper()
        {
            delete widget_data_;
            delete main_layout_;
        }

        /* getter */
        _QtWidgetType*       widget_data()      { return widget_data_; }
        virtual QSize sizeHint() const{ return widget_data_->sizeHint(); }
    private:
        _QtWidgetType* widget_data_;
        QVBoxLayout*   main_layout_;
    };

}

#define DEFINE_MDI_SUB_WIN_CLASS_WITH_TYPE( class_name, win_type ) \
    class WSP_DLL_EXPORT class_name##MdiSubWin\
        : public wsp::QtAbstractMdiSubWin\
    {\
    public:\
        class_name##MdiSubWin(QWidget *parent=NULL)\
            : wsp::QtAbstractMdiSubWin( parent )\
        {\
            win_type_ = win_type;\
            this->setWidget( &widget_data_ );\
        }\
        class_name* widget_data(){ return &widget_data_; }\
        virtual QSize sizeHint() const{ return widget_data_.sizeHint(); }\
    private:\
        class_name widget_data_;\
    }

#define DEFINE_MDI_SUB_WIN_CLASS( class_name ) DEFINE_MDI_SUB_WIN_CLASS_WITH_TYPE(class_name, wsp::gui::WINDOW_TYPE_GENERAL_VIEW)


#endif
