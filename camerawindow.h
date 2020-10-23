#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <QMainWindow>
#include <visca/libvisca.h>

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

    void on_cameraDisplayCheckBox_stateChanged(int arg1);

    void on_cameraIrReceiveCheckBox_stateChanged(int arg1);

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

    void on_wBComboBox_currentIndexChanged(int index);

    void on_rGainSlider_valueChanged(int value);

    void on_bGainSlider_valueChanged(int value);

    void on_aEComboBox_currentIndexChanged(int index);

    void on_shutterSlider_valueChanged(int value);

    void on_irisSlider_valueChanged(int value);

    void on_gainSlider_valueChanged(int value);

    void on_brightSlider_valueChanged(int value);

    void on_apertureSlider_valueChanged(int value);

    void on_exposureCompensationSlider_valueChanged(int value);

    void on_exposureCompensationCheckBox_stateChanged(int arg1);

    void on_backLightCompensationCheckBox_stateChanged(int arg1);

    void on_slowShutterAutoCheckBox_stateChanged(int arg1);

    void on_wideModeComboBox_currentIndexChanged(int index);

    void on_pictureEffectComboBox_currentIndexChanged(int index);

    void on_digitalEffectComboBox_currentIndexChanged(int index);

    void on_digitalEffectLevelSpinBox_valueChanged(int arg1);

    void on_pictureFreezeCheckBox_stateChanged(int arg1);

    void on_pictureMirrorCheckBox_stateChanged(int arg1);

    void on_memoryListWidget_itemSelectionChanged();

    void on_memoryResetButton_clicked();

    void on_memorySetButton_clicked();

    void on_memoryRecallButton_clicked();

private:
    void OpenInterface();
    void CloseInterface();
    void UpdateAESliders(int index);

private:
    Ui::CameraWindow *ui;
    VISCAInterface_t interface;
    VISCACamera_t camera;
    const char* ttyDev;

    unsigned int panSpeed;
    unsigned int tiltSpeed;
};

#endif // CAMERAWINDOW_H
