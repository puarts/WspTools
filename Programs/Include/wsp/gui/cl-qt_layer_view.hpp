// File Name: WspLayerView.h
// Author: Junichi Nishikata

#ifndef __WspLayerView_H__
#define __WspLayerView_H__

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtGui/qtreewidget.h>
#include <wsp/image/tpl_cl-image.h>
#include <wsp/image/tpl_fn-imgproc.hpp>
#include <wsp/common/_define_qt.h>

//class WspRgb;
//class WspDoubleImg;

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

class QGridLayout;
class QVBoxLayout;

class WspLayerView
	: public QTreeWidget
{
	Q_OBJECT
	//! Variables -------------------------------------------
protected:
	QHeaderView *m_header;
	//QGridLayout *main_glayout;

	//! button elements

public:
	//! parameter setting GUI elements

	//! Methods -------------------------------------------
public:
	WspLayerView(QWidget *parent=0);
	~WspLayerView();

protected slots:
	//void slot_start();

protected:
	virtual void closeEvent(QCloseEvent *e);
};

#endif