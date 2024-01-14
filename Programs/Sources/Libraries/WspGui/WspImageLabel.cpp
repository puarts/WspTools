//** WspImageLabel.cpp
//** Author: Junichi Nishikata

#include <QtGui>

#include <wsp/gui/WspImageLabel.h>

WspImageLabel::WspImageLabel(QWidget *parent)
	: QLabel(parent)
{
	setMinimumSize(200, 200);
	//setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
	setAlignment(Qt::AlignCenter);
	setAcceptDrops(true);
	setAutoFillBackground(true);
	clear();
}

void WspImageLabel::dragEnterEvent(QDragEnterEvent *event)
{
	//setText(tr("<drop content>"));
	setBackgroundRole(QPalette::Highlight);

	event->acceptProposedAction();
	emit changed(event->mimeData());
}

void WspImageLabel::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

void WspImageLabel::dropEvent(QDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();

	printf("%d %d %d %d\n", mimeData->hasImage(), mimeData->hasText(), mimeData->hasUrls());

	if (mimeData->hasImage()) {
		setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
	} else if (mimeData->hasHtml()) {
		setText(mimeData->html());
		setTextFormat(Qt::RichText);
	} else if (mimeData->hasText()) {
		setText(mimeData->text());
		setTextFormat(Qt::PlainText);
	} else if (mimeData->hasUrls()) {
		QList<QUrl> urlList = mimeData->urls();
		QString text;
		for (int i = 0; i < urlList.size() && i < 32; ++i) {
			QString url = urlList.at(i).path();
			text += url + QString("\n");
		}
		setText(text);
	} else {
		setText(tr("Cannot display data"));
	}

	setBackgroundRole(QPalette::Dark);
	event->acceptProposedAction();
}

void WspImageLabel::dragLeaveEvent(QDragLeaveEvent *event)
{
	clear();
	event->accept();
}

void WspImageLabel::clear()
{
	//setText(tr("<drop content>"));
	setBackgroundRole(QPalette::Dark);

	emit changed();
}