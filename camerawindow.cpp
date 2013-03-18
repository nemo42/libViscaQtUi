#include "camerawindow.h"
#include "ui_camerawindow.h"

CameraWindow::CameraWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::CameraWindow),
      ttyDev("/dev/ttyS0"),
      panSpeed(1),
      tiltSpeed(1)
{
    ui->setupUi(this);
    OpenInterface();

    // Init UI
    ui->vendorLineEdit->setText(QString("0x%1").arg(QString::number(camera.vendor, 16), 4, '0'));
    ui->rOMVerLineEdit->setText(QString("0x%1").arg(QString::number(camera.rom_version, 16), 4, '0'));
    ui->modelLineEdit->setText(QString("0x%1").arg(QString::number(camera.model, 16), 4, '0'));
    ui->socketLineEdit->setText(QString("0x%1").arg(QString::number(camera.socket_num, 16), 4, '0'));
}

CameraWindow::~CameraWindow()
{
    CloseInterface();
    delete ui;
}

void CameraWindow::OpenInterface()
{
    int camera_num;
    if (VISCA_open_serial(&interface, ttyDev)!=VISCA_SUCCESS) {
        fprintf(stderr,"camera_ui: unable to open serial device %s\n",ttyDev);
        exit(1);
    }

    interface.broadcast=0;
    VISCA_set_address(&interface, &camera_num);
    camera.address=1;
    VISCA_clear(&interface, &camera);
    VISCA_get_camera_info(&interface, &camera);
}

void CameraWindow::CloseInterface()
{
    unsigned char packet[3000];
    int i, bytes;

    // read the rest of the data: (should be empty)
    ioctl(interface.port_fd, FIONREAD, &bytes);
    if (bytes>0) {
        fprintf(stderr, "ERROR: %d bytes not processed: ", bytes);
        read(interface.port_fd, &packet, bytes);
        for (i=0;i<bytes;i++) {
            fprintf(stderr,"%2x ",packet[i]);
        }
        fprintf(stderr,"\n");
    }
    VISCA_close_serial(&interface);
}

void CameraWindow::on_cameraPowerButton_clicked()
{
    UInt8_t power = 3;
//    VISCA_get_power(&interface, &camera, &power);
    // TODO: Power off (3) seems to wait forever for a response, only do power on (2)
    VISCA_set_power(&interface, &camera, power == 2 ? 3 : 2);
}

void CameraWindow::on_cameraAutoOffSpinBox_valueChanged(int arg1)
{

}

void CameraWindow::on_cameraDisplayCheckBox_stateChanged(int arg1)
{
    switch (arg1)
    {
    case Qt::Unchecked:
        VISCA_set_datascreen_off(&interface, &camera);
        break;
    case Qt::Checked:
            VISCA_set_datascreen_on(&interface, &camera);
        break;
    }
}

void CameraWindow::on_cameraIrReceiveCheckBox_stateChanged(int arg1)
{
    switch (arg1)
    {
    case Qt::Unchecked:
        VISCA_set_irreceive_off(&interface, &camera);
        break;
    case Qt::Checked:
            VISCA_set_irreceive_on(&interface, &camera);
        break;
    }

}

void CameraWindow::on_cameraIrReturnCheckBox_stateChanged(int arg1)
{

}

void CameraWindow::on_panTiltJoystickUpLeftButton_clicked()
{
    VISCA_set_pantilt_upleft(&interface, &camera, panSpeed, tiltSpeed);
}

void CameraWindow::on_panTiltJoystickUpButton_clicked()
{
    VISCA_set_pantilt_up(&interface, &camera, panSpeed, tiltSpeed);
}

void CameraWindow::on_panTiltJoystickUpRightButton_clicked()
{
    VISCA_set_pantilt_upright(&interface, &camera, panSpeed, tiltSpeed);
}

void CameraWindow::on_panTiltJoystickRightButton_clicked()
{
    VISCA_set_pantilt_right(&interface, &camera, panSpeed, tiltSpeed);
}

void CameraWindow::on_panTiltJoystickDownRightButton_clicked()
{
    VISCA_set_pantilt_downright(&interface, &camera, panSpeed, tiltSpeed);
}

void CameraWindow::on_panTiltJoystickDownButton_clicked()
{
    VISCA_set_pantilt_down(&interface, &camera, panSpeed, tiltSpeed);
}

void CameraWindow::on_panTiltJoystickDownLeftButton_clicked()
{
    VISCA_set_pantilt_downleft(&interface, &camera, panSpeed, tiltSpeed);
}

void CameraWindow::on_panTiltJoystickLeftButton_clicked()
{
    VISCA_set_pantilt_left(&interface, &camera, panSpeed, tiltSpeed);
}

void CameraWindow::on_panTiltJoystickStopButton_clicked()
{
    VISCA_set_pantilt_stop(&interface, &camera, panSpeed, tiltSpeed);
}

void CameraWindow::on_panTiltHomeButton_clicked()
{
    VISCA_set_pantilt_home(&interface, &camera);
}

void CameraWindow::on_panTiltResetButton_clicked()
{
    VISCA_set_pantilt_reset(&interface, &camera);
}

void CameraWindow::on_zoomSlider_valueChanged(int value)
{
    VISCA_set_zoom_value(&interface, &camera, value);
}

void CameraWindow::on_zoomTeleButton_clicked()
{
    VISCA_set_zoom_tele(&interface, &camera);
}

void CameraWindow::on_zoomStopButton_clicked()
{
    VISCA_set_zoom_stop(&interface, &camera);
}

void CameraWindow::on_zoomWideButton_clicked()
{
    VISCA_set_zoom_wide(&interface, &camera);
}
