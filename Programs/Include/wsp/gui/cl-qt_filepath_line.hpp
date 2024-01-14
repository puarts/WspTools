/**
 * @file cl-qt_filepath_line.hpp
 * @author Junichi Nishikata
 */

#ifndef __WSP_GUI_CLASS_QFILEPATH_LINE_H__
#define __WSP_GUI_CLASS_QFILEPATH_LINE_H__

#include <QtGui/QtGui>

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

namespace wsp{
    class WSP_DLL_EXPORT QtFilepathLine
        :public QWidget
    {
        Q_OBJECT

    public:
        typedef enum FileDialogType
        {
            FILE_DIALOG_TYPE_DIR,
            FILE_DIALOG_TYPE_SAVE,
            FILE_DIALOG_TYPE_OPEN_FILE,
            FILE_DIALOG_TYPE_OPEN_FILES,
        } FileDialogType;

    public:
        QtFilepathLine(QWidget *parent=NULL);
        ~QtFilepathLine();

        // accessors
        QString    text     () const{ return path_line_->text(); }
        QLineEdit* path_line()      { return path_line_; }

        // mutators
        void setText        ( const QString &text){ path_line_->setText(text); }
        void set_dialog_type( wsp::QtFilepathLine::FileDialogType dialog_type ){ dialog_type_ = dialog_type; } 
        void set_filter     ( QString filter ){ filter_ = filter; }

    private slots:
        void browse();

    private:
        wsp::QtFilepathLine::FileDialogType dialog_type_;
        QString                             filter_;

        QLineEdit*                          path_line_;
        QPushButton*                        browse_button_;
        QHBoxLayout*                        main_layout_;
    };
}

#endif
