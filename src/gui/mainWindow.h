#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include "gui/glwidget.h"
#include <QString>
#include <QLabel>
#include<QGridLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionNewProject_triggered();
    void on_actionA_propos_triggered();
    void on_gtmove_clicked();
    void on_gtrotate_clicked();
    void on_redo_clicked();
    void on_gtscale_clicked();
    void keyPressEvent(QKeyEvent *event);
    void on_initCube_clicked();
    void on_initSphere_clicked();
    void on_pushCancel_clicked();
    void on_pushValid_clicked();
    void updateLastModel();
    void disableTool();
    void hideDialog();


    //void on_pushButton_clicked();

    void on_actionFullscreen_triggered();

    void on_actionSave_as_triggered();

    void on_actionNewObject_triggered();


    void on_actionCr_er_un_Cube_triggered();

    void on_rotate_clicked();

    void on_zoom_clicked();

private:
    Ui::MainWindow *ui;
    Model *m_model;
    QWidget *modelList;
    QVBoxLayout *modelListLayout;
};

#endif // MAINWINDOW_H
