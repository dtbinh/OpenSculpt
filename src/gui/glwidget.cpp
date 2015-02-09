#include "gui/glwidget.h"

#if defined(__APPLE__) && (__MACH__)
    #include <GLUT/glut.h>
#elif defined(__WIN32__)
    #define GLUT_DISABLE_ATEXIT_HACK
    #include <GL/glut.h>
#else
    #include <GL/glut.h>
#endif

GLWidget::GLWidget(QWidget *parent ) : QGLWidget(parent)
{
    activeTool = NOTOOL;
    activeMesh = -1;
    mode_fill = true;
    brushSize = 3;

    connect(&m_timer, SIGNAL(timeout()),this, SLOT(updateGL()));
    connect(&m_timer, SIGNAL(timeout()),this, SLOT(updateActiveMesh()));

    m_timer.start(16);
}


void GLWidget::initializeGL()
{
    // View & rotation settings
    distance = -15.0;
    offsetX = 0;
    offsetY = 0;
    x_rot = 0;
    y_rot = 0;
    z_rot = 0;

    // GL options
    qglClearColor(Qt::black);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, mode_fill ? GL_FILL : GL_LINE);

    // Model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(offsetX, -offsetY, -distance,
              offsetX, -offsetY, 0.0,
              0.0, 1.0, 0.0);

    glRotatef(x_rot, 1.0f, 0.0f, 0.0f);
    glRotatef(y_rot, 0.0f, 1.0f, 0.0f);
    glRotatef(z_rot, 0.0f, 0.0f, 1.0f);

    int LightPos[4] = {(int)(offsetX - distance),(int)-offsetY,(int)(-distance/2),1};
    glLightiv(GL_LIGHT0,GL_POSITION,LightPos);

    // Projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0f, 1.0*width()/height(), 0.1f, 100.0f);

    // Draw map
    if(mode_fill) {
        //qglColor(Qt::lightGray);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_manager.paintGL();
    }

    //qglColor(Qt::black);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_manager.paintGL();

    // Brush
    /*glColor3d(1,0,0);
    glBegin(GL_TRIANGLE_FAN);
        for (int i=0; i < 16; ++i) {
            float alpha = i * 360 / 16.;
            glVertex3f(last_pos.x() + cosd(alpha)*brushSize, last_pos.y() + sind(alpha)*brushSize, 0);
        }
    glEnd();*/
}
void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F) {

        mode_fill = !mode_fill;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    last_pos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - last_pos.x();
    int dy = event->y() - last_pos.y();

    if (event->buttons() & Qt::RightButton)
    {
            rotateBy(dy, dx, 0);
    }

    if (event->buttons() & Qt::LeftButton)
    {
        if(activeTool == WTROTATE)
        {
            rotateBy(dy, dx, 0);
        }
        else if(activeTool == WTSCALE)
        {
            distance *= 1.0 + (1.0 * dy / 300.0);
        }
        else if(activeTool == WTMOVE)
        {
            offsetX += (distance * dx / 600.0);
            offsetY += (distance * dy / 600.0);
        }
        else if(activeTool != NOTOOL && m_manager.getMesh(activeMesh) != NULL)
        {
            QVector3D rotation = QVector3D(x_rot, y_rot, z_rot);
            QVector3D move = rotateXYZ(QVector3D(-dx, dy, 0), rotation) * (distance / 900.0);
            bool needUpdate = MeshProcessing::subdivideAuto(m_manager.getMesh(activeMesh), 5.);

            switch(activeTool) {

            // GLOBAL TOOLS
            case GTMOVE:
                m_tool.gtmove(m_manager.getMesh(activeMesh), move);
                break;

            case GTROTATE:
                m_tool.gtrotate(m_manager.getMesh(activeMesh), QVector3D(dy, dx, 0));
                break;

            case GTSCALE:
                m_tool.gtscale(m_manager.getMesh(activeMesh), QVector3D(0, dy, 0));
                break;

            // LOCAL TOOLS
            case LTADD:
                m_tool.ltadd(m_manager.getMesh(activeMesh), last_pos, move, brushSize);
                break;

            case LTINFLATE:
                m_tool.ltinflate(m_manager.getMesh(activeMesh), last_pos, move, brushSize);
                break;

            case LTMOVE:
                m_tool.ltmove(m_manager.getMesh(activeMesh), last_pos, move, brushSize);
                break;

            case LTPINCH:
                m_tool.ltpinch(m_manager.getMesh(activeMesh), last_pos, move, brushSize);
                break;

            case LTSMOOTH:
                m_tool.ltsmooth(m_manager.getMesh(activeMesh), last_pos, move, brushSize);
                break;

            default:
                qDebug() << "The tool isn't defined";
                break;
            }

            if(needUpdate) m_manager.updateMesh(activeMesh);
        }
        else
        {
            qDebug() << "NOTOOL or Mesh NULL";
        }
    }

    last_pos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    distance *= 1.0 - (1.0 * event->delta() / 1200.0);
}

void GLWidget::rotateBy(int x, int y, int z)
{
    x_rot += x;
    y_rot += y;
    z_rot += z;
}

void GLWidget::enableTool(TOOL tool) { activeTool = tool; }

void GLWidget::selectMesh(Mesh *mesh) {
    activeMesh = -1;
    for(int i=0 ; i < m_manager.getSize() ; ++i) {
        if(m_manager.getMesh(i) == mesh) {
            activeMesh = i;
        }
    }
}

void GLWidget::updateActiveMesh() {
    if(activeMesh >= 0) {
        m_manager.updateMesh(activeMesh);
    }
    else {
        m_manager.updateLastMesh();
    }
}

void GLWidget::addMesh(Mesh *mesh) { m_manager.addMesh(mesh); }
void GLWidget::removeMesh(Mesh *mesh) {  m_manager.removeMesh(mesh); }
void GLWidget::clear() { m_manager.clear(); }

void GLWidget::resetView()
{
    distance = -15.0;
    offsetX = offsetY = x_rot = y_rot = z_rot = 0.0;
}

void GLWidget::undo()
{

}

void GLWidget::redo()
{

}
