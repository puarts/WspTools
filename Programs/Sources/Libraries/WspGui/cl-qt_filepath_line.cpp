/**
 * @file cl-qt_filepath_line.cpp
 * @author Jun-ichi Nishikata
 */

#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include <wsp/common/fn-util.h>
#endif

#include "cl-qt_filepath_line.hpp"

#define INIT_WSP_QFILEPATHLINE \
    path_line_    (new QLineEdit())\
    , browse_button_(new QPushButton(QString("browse")))\
    , main_layout_  (new QHBoxLayout())\

wsp::QtFilepathLine::QtFilepathLine(QWidget *parent)
    : INIT_WSP_QFILEPATHLINE
{
    filter_  = QString("All files (*)");
    connect(browse_button_, SIGNAL(clicked()), this, SLOT(browse()));
    main_layout_->addWidget(path_line_);
    main_layout_->addWidget(browse_button_);
    this->setLayout(main_layout_);
}

wsp::QtFilepathLine::~QtFilepathLine()
{
    delete path_line_;
    delete browse_button_;
    delete main_layout_;
}

void wsp::QtFilepathLine::browse()
{
    QString qfilename;
    switch( dialog_type_ )
    {
    case wsp::QtFilepathLine::FILE_DIALOG_TYPE_DIR:
        qfilename = QFileDialog::getOpenFileName(NULL, QString("Open"), QDir::currentPath(), filter_);
        break;
    case wsp::QtFilepathLine::FILE_DIALOG_TYPE_OPEN_FILE:
        qfilename = QFileDialog::getOpenFileName(NULL, QString("Open"), QDir::currentPath(), filter_);
        break;
    case wsp::QtFilepathLine::FILE_DIALOG_TYPE_SAVE:
        qfilename = QFileDialog::getSaveFileName(NULL, QString("Save"), QDir::currentPath(), filter_);
        break;
    case wsp::QtFilepathLine::FILE_DIALOG_TYPE_OPEN_FILES:
    default:
        WSP_COMMON_ERROR_LOG( "Unsupported dialog type\n" );
        return;
    }

    /* set current dir */
    {
        char currentDir[MAX_LENGTH_OF_PATH];
        wsp::GetDir(currentDir, qfilename.toLatin1().data());
        if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }
    }

    if (!qfilename.isEmpty())
    {
        path_line_->setText(qfilename);
    }
}