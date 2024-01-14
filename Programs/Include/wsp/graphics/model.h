#ifndef MODEL_H
#define MODEL_H

#include <wsp/common/_define_commoncore.h>

#ifdef USE_QT
#include <QtCore/QString>
#include <QtCore/QVector>
#endif

#include <math.h>

#include "point3d.h"
#ifdef USE_QT
class WSP_DLL_EXPORT Model
{
public:
    Model() {}
    Model(const QString &filePath);

    void render(bool wireframe = false, bool normals = false) const;

    QString fileName() const { return m_fileName; }
    int faces() const { return m_pointIndices.size() / 3; }
    int edges() const { return m_edgeIndices.size() / 2; }
    int points() const { return m_points.size(); }

private:
    QString m_fileName;
    QVector<Point3d> m_points;
    QVector<Point3d> m_normals;
    QVector<int> m_edgeIndices;
    QVector<int> m_pointIndices;
};
#endif

#endif
