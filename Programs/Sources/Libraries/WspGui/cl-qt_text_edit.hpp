/**
 * @file cl-qt_text_edit.hpp
 * @author Junichi Nishikata
 */

#ifndef __WSP_GUI_CLASS_QT_TEXT_EDIT_H__
#define __WSP_GUI_CLASS_QT_TEXT_EDIT_H__

#include <QtGui/QtGui>

namespace wsp{
    class QtTextEdit
        : public QTextEdit
    {
    public:
        bool canInsertFromMimeData(const QMimeData* source) const;
        void insertFromMimeData(const QMimeData* source);
 
    private:
        void dropImage(const QUrl& url, const QImage& image);
        void dropTextFile(const QUrl& url);
    };
}


inline bool wsp::QtTextEdit::canInsertFromMimeData(const QMimeData* source) const
{
    return source->hasImage() || source->hasUrls() ||
        QTextEdit::canInsertFromMimeData(source);
}

inline void wsp::QtTextEdit::insertFromMimeData(const QMimeData* source)
{
    if (source->hasImage())
    {
        static int i = 1;
        QUrl url(QString("dropped_image_%1").arg(i++));
        dropImage(url, qvariant_cast<QtGui/QImage>(source->imageData()));
    }
    else if (source->hasUrls())
    {
        foreach (QUrl url, source->urls())
        {
            QFileInfo info(url.toLocalFile());
            if (QImageReader::supportedImageFormats().contains(info.suffix().toLower().toLatin1()))
                dropImage(url, QImage(info.filePath()));
            else
                dropTextFile(url);
        }
    }
    else
    {
        QTextEdit::insertFromMimeData(source);
    }
}

inline void wsp::QtTextEdit::dropImage(const QUrl& url, const QImage& image)
{
    if (!image.isNull())
    {
        document()->addResource(QTextDocument::ImageResource, url, image);
        textCursor().insertImage(url.toString());
    }
}

inline void wsp::QtTextEdit::dropTextFile(const QUrl& url)
{
    QFile file(url.toLocalFile());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        textCursor().insertText(file.readAll());
}

#endif