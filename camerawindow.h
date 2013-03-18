#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <QMainWindow>
#include <libvisca/libvisca.h>

namespace Ui {
class CameraWindow;
}

class CameraWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    CameraWindow(QWidget *parent = 0);
    ~CameraWindow();

private:
    void OpenInterface();
    void CloseInterface();

private:
    Ui::CameraWindow *ui;
    VISCAInterface_t interface;
    VISCACamera_t camera;
    char* ttyDev;
};

#endif // CAMERAWINDOW_H
