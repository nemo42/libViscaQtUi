#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <QMainWindow>

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
    Ui::CameraWindow *ui;
};

#endif // CAMERAWINDOW_H
