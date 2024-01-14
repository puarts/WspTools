//** WspImageLabel.h
//** Author: Junichi Nishikata

#ifndef _WspImageLabel_H_
#define _WspImageLabel_H_

#include <QLabel>

class QMimeData;

class WspImageLabel : public QLabel
{
	Q_OBJECT
public:
	WspImageLabel(QWidget *parent = 0);

public slots:
	void clear();

signals:
	void changed(const QMimeData *mimeData = 0);

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dropEvent(QDropEvent *event);

private:
	QLabel *label;
};

#endif