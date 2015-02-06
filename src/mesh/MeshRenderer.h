#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <QGLBuffer>
#include "Mesh.h"

class MeshRenderer {

public:
    MeshRenderer();
    ~MeshRenderer();

public:
    void paintGL();

    void convertToBuffer();
    void update();

private:
    Mesh *m_mesh;

    // Rendering
    QVector<QVector3D> m_coords;
    QVector<GLuint> m_indices;

    // GPU Buffer
    QGLBuffer m_vertexbuffer;
    QGLBuffer m_indicebuffer;
};

#endif // MESHRENDERER_H
