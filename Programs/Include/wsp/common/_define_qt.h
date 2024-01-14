/**
 * @file _define_qt.h
 * 
 * Base definition of core module for Qt.
 */



#ifndef __WSP_COMMON_WspDefine_Qt_H__
#define __WSP_COMMON_WspDefine_Qt_H__

#ifndef __WSP_COMMONUTIL_DEFINE_COMMONUTIL_H__
#include "_define_commonutil.h"
#endif

#ifdef USE_QT

#include <QtGui/QMessageBox>

namespace wsp{
    const char kWspImageSupportedFmt[] =
        "Portable Graymap ""(*.pgm);;"
        "Portable Pixmap " "(*.ppm);;"
        "Adaptive Dynamic Range ""(*.adri);;"
        "BIN "             "(*.bin);;"
        ;

    const char kDevIlLoadingSupportedFmt[] = 
        "BMP "                       "(*.bmp);;"
        "CUT "                       "(*.cut);;"
        "DDS "                       "(*.dds);;"
        "DOOM "                      "(*.doom);;"
        "OpenEXR "                   "(*.exr);;"
        "HDR "                       "(*.hdr);;"
        "GIF "                       "(*.gif);;"
        "ICO (Windows Icon)"         "(*.ico);;"
        "JPEG2000 "                  "(*.jp2);;"
        "JPEG "                      "(*.jpg);;"
        "IBM "                       "(*.ibm);;"
        "MDL "                       "(*.mdl);;"
        "MNG "                       "(*.mng);;"
        "PAL "                       "(*.pal);;"
        "PBM "                       "(*.pbm);;"
        "PCD "                       "(*.pcd);;"
        "PCX "                       "(*.pcx);;"
        "PICT file"                  "(*.pic);;"
        "Portable Network Graphics " "(*.png);;"
        "Portable Graymap "          "(*.pgm);;"
        "Portable Pixmap "           "(*.ppm);;"
        "Photoshop "                 "(*.psd);;"
        "PSP "                       "(*.psp);;"
        "RAW "                       "(*.raw);;"
        "SGI "                       "(*.sgi);;"
        "Targa "                     "(*.tga);;"
        "TIFF "                      "(*.tif);;"
        ;

    const char kDevIlSavingSupportedFmt[] = 
        "BMP "                       "(*.bmp);;"
        "DDS "                       "(*.dds);;"
        "C Header "                  "(*.h);;"
        "JPEG "                      "(*.jpg);;"
        "PAL "                       "(*.pal);;"
        "Portable Bitmap "           "(*.pbm);;"
        "Portable Network Graphics " "(*.png);;"
        "Portable Graymap "          "(*.pgm);;"
        "Portable Pixmap "           "(*.ppm);;"
        "RAW "                       "(*.raw);;"
        "SGI "                       "(*.sgi);;"
        "Targa "                     "(*.tga);;"
        "TIFF "                      "(*.tif);;"
        ;
    
    static const QString formatFilterImg = QString(
            "Image files (*.bmp *.gif *.jpg *.png *.pgm *.ppm *.tif *.xbm *.xpm *.adri *.bin);;"
            "BMP (*.bmp);;"
            "GIF (*.gif);;"
            "JPG (*.jpg);;"
            "PNG (*.png);;"
            "PGM (*.pgm);;"
            "PPM (*.ppm);;"
            "TIFF (*.tif);;"
            "XBM (*.xbm);;"
            "XPM (*.xpm);;"
            "ADRI (*.adri);;"
            "BIN (*.bin)"
            ";;All files (*)"
        );

    static const QString formatFilter3dModel = QString(
            "3D Model files (*.obj);;"
            "OBJ (*.obj);;"
            "All files (*)"
        );

    static const QString formatFilterWspLibData = QString("WspLib data files (*.wspid *.wspif *.wspii *wspic *wspiuc);;All files (*)");
    static const QString formatFilterADRI       = QString("ADRI files (*.adri);;All files (*)");
    static const QString formatFilterXml        = QString("XML (*.xml);;All files (*)");
    static const QString formatFilterGLSL       = QString("GLSL (*.vs *.fs);;All files (*)");
    static const QString formatFilterMultimedia = formatFilterImg;

    static const QString kTransparentIconPath( "./icons/transparent_icon.png" );

    inline void QtError(char *file, int line, char *fmt, ...)
    {
        char msg[512];
        va_list ap;
        va_start(ap, fmt);
        vsprintf(msg, fmt, ap);

        QString qmsg=QString("");
#ifdef _DEBUG 
        qmsg.sprintf("%s line %d: \n", file, line);
#endif
        qmsg+=QString(msg);

        QMessageBox::information(NULL, QString("Error"), qmsg);
        va_end(ap);
    }
    inline void QtInfo(char *fmt, ...)
    {
        char msg[512];
        va_list ap;
        va_start(ap, fmt);
        QString qmsg;
        vsprintf(msg, fmt, ap);
        qmsg=QString(msg);
        QMessageBox::information(NULL, QString("Information"), qmsg);
        va_end(ap);
    }
}


#define WSP_QT_DIALOG_ERROR(fmt, ...) wsp::QtError(__FILE__, __LINE__, fmt, __VA_ARGS__)
#define WSP_QT_DIALOG_INFO(fmt, ...) wsp::QtInfo(fmt, __VA_ARGS__)

#endif

#endif
