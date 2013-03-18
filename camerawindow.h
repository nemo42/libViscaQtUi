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

private slots:
    void on_cameraPowerButton_clicked();

    void on_cameraAutoOffSpinBox_valueChanged(int arg1);

    void on_cameraDisplayCheckBox_stateChanged(int arg1);

    void on_cameraIrReceiveCheckBox_stateChanged(int arg1);

    void on_cameraIrReturnCheckBox_stateChanged(int arg1);

    void on_panTiltJoystickUpLeftButton_clicked();

    void on_panTiltJoystickUpButton_clicked();

    void on_panTiltJoystickUpRightButton_clicked();

    void on_panTiltJoystickRightButton_clicked();

    void on_panTiltJoystickDownRightButton_clicked();

    void on_panTiltJoystickDownButton_clicked();

    void on_panTiltJoystickDownLeftButton_clicked();

    void on_panTiltJoystickLeftButton_clicked();

    void on_panTiltJoystickStopButton_clicked();

    void on_panTiltHomeButton_clicked();

    void on_panTiltResetButton_clicked();

    void on_zoomSlider_valueChanged(int value);

    void on_zoomTeleButton_clicked();

    void on_zoomStopButton_clicked();

    void on_zoomWideButton_clicked();

private:
    void OpenInterface();
    void CloseInterface();

private:
    Ui::CameraWindow *ui;
    VISCAInterface_t interface;
    VISCACamera_t camera;
    char* ttyDev;

    unsigned int panSpeed;
    unsigned int tiltSpeed;
};

#endif // CAMERAWINDOW_H
