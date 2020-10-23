#include <unistd.h>
#include <sys/ioctl.h>
#include "camerawindow.h"
#include "ui_camerawindow.h"

const uint8_t VISCA_POWER_ON = 2;
const uint8_t VISCA_POWER_OFF = 3;
const int VISCA_DIGITAL_EFFECT_LEVEL_FLASH_TRAIL_MAX = 0x18;
const int VISCA_DIGITAL_EFFECT_LEVEL_LUMI_STILL_MAX =0x20;
const int VISCA_AE_MODES[] =
{
    VISCA_AUTO_EXP_FULL_AUTO,
    VISCA_AUTO_EXP_MANUAL,
    VISCA_AUTO_EXP_SHUTTER_PRIORITY,
    VISCA_AUTO_EXP_IRIS_PRIORITY,
    VISCA_AUTO_EXP_GAIN_PRIORITY,
    VISCA_AUTO_EXP_SHUTTER_AUTO,
    VISCA_AUTO_EXP_IRIS_AUTO,
    VISCA_AUTO_EXP_GAIN_AUTO,
    VISCA_AUTO_EXP_BRIGHT
};

// Enable states of the sliders for the different AE modes.
//                            Auto,  Man,  S Pri, I Pri, G Pri, S Aut, I Aut, G Aut, Bright
const bool SHUTTER_ENABLED[] = {false, true, true, false, false, false, true, true, false};
const bool IRIS_ENABLED[]    = {false, true, false, true, false, true, false, true, false};
const bool GAIN_ENABLED[]    = {false, true, false, false, true, true, true, false, false};
const bool BRIGHT_ENABLED[]  = {false, false, false, false, false, false, false, false, true};

CameraWindow::CameraWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::CameraWindow),
      ttyDev("/dev/ttyUSB1"),
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

void CameraWindow::UpdateAESliders(int index)
{
    ui->shutterSlider->setEnabled(SHUTTER_ENABLED[index]);
    ui->irisSlider->setEnabled(IRIS_ENABLED[index]);
    ui->gainSlider->setEnabled(GAIN_ENABLED[index]);
    ui->brightSlider->setEnabled(BRIGHT_ENABLED[index]);
    ui->backLightCompensationCheckBox->setEnabled(index  == 0);
}

void CameraWindow::on_cameraPowerButton_clicked()
{
    uint8_t power = VISCA_POWER_OFF;
//    VISCA_get_power(&interface, &camera, &power);
    // TODO: Power off seems to wait forever for a response, only do power on
    VISCA_set_power(&interface, &camera, power == VISCA_POWER_ON ? VISCA_POWER_OFF : VISCA_POWER_ON);
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
    // TODO: Update on separate thread
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

void CameraWindow::on_wBComboBox_currentIndexChanged(int index)
{
    VISCA_set_whitebal_mode(&interface, &camera, index);
    ui->bGainSlider->setEnabled(index == VISCA_WB_MANUAL);
    ui->rGainSlider->setEnabled(index == VISCA_WB_MANUAL);

    uint16_t rGain, bGain;
    VISCA_get_rgain_value(&interface, &camera, &rGain);
    VISCA_get_bgain_value(&interface, &camera, &bGain);
    ui->bGainSlider->setValue(bGain);
    ui->rGainSlider->setValue(rGain);
}

void CameraWindow::on_rGainSlider_valueChanged(int value)
{
    VISCA_set_rgain_value(&interface, &camera, value);
}

void CameraWindow::on_bGainSlider_valueChanged(int value)
{
    VISCA_set_bgain_value(&interface, &camera, value);
}

void CameraWindow::on_aEComboBox_currentIndexChanged(int index)
{
    VISCA_set_auto_exp_mode(&interface, &camera, VISCA_AE_MODES[index]);
    UpdateAESliders(index);
}

void CameraWindow::on_shutterSlider_valueChanged(int value)
{
    VISCA_set_shutter_value(&interface, &camera, value);
}

void CameraWindow::on_irisSlider_valueChanged(int value)
{
    VISCA_set_iris_value(&interface, &camera, value);
}

void CameraWindow::on_gainSlider_valueChanged(int value)
{
    VISCA_set_gain_value(&interface, &camera, value);
}

void CameraWindow::on_brightSlider_valueChanged(int value)
{
    VISCA_set_bright_value(&interface, &camera, value);
}

void CameraWindow::on_apertureSlider_valueChanged(int value)
{
    VISCA_set_aperture_value(&interface, &camera, value);
}

void CameraWindow::on_exposureCompensationSlider_valueChanged(int value)
{
    VISCA_set_exp_comp_value(&interface, &camera, value);
}

void CameraWindow::on_exposureCompensationCheckBox_stateChanged(int arg1)
{
    switch (arg1)
    {
    case Qt::Unchecked:
        ui->exposureCompensationSlider->setEnabled(false);
        VISCA_set_exp_comp_power(&interface, &camera, VISCA_POWER_OFF);
        break;
    case Qt::Checked:
        ui->exposureCompensationSlider->setEnabled(true);
        VISCA_set_exp_comp_power(&interface, &camera, VISCA_POWER_ON);
        break;
    }

}

void CameraWindow::on_backLightCompensationCheckBox_stateChanged(int arg1)
{
    switch (arg1)
    {
    case Qt::Unchecked:
        VISCA_set_backlight_comp(&interface, &camera, VISCA_POWER_OFF);
        break;
    case Qt::Checked:
        VISCA_set_backlight_comp(&interface, &camera, VISCA_POWER_ON);
        break;
    }

}

void CameraWindow::on_slowShutterAutoCheckBox_stateChanged(int arg1)
{
    switch (arg1)
    {
    case Qt::Unchecked:
        VISCA_set_slow_shutter_auto(&interface, &camera, VISCA_POWER_OFF);
        break;
    case Qt::Checked:
        VISCA_set_slow_shutter_auto(&interface, &camera, VISCA_POWER_ON);
        break;
    }

}

void CameraWindow::on_wideModeComboBox_currentIndexChanged(int index)
{
    VISCA_set_wide_mode(&interface, &camera, index);
}

void CameraWindow::on_pictureEffectComboBox_currentIndexChanged(int index)
{
    VISCA_set_picture_effect(&interface, &camera, index);
}

void CameraWindow::on_digitalEffectComboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case VISCA_DIGITAL_EFFECT_FLASH:
    case VISCA_DIGITAL_EFFECT_TRAIL:
        ui->digitalEffectLevelSpinBox->setMaximum(VISCA_DIGITAL_EFFECT_LEVEL_FLASH_TRAIL_MAX);
        ui->digitalEffectLevelSpinBox->setEnabled(true);
        break;
    case VISCA_DIGITAL_EFFECT_STILL:
    case VISCA_DIGITAL_EFFECT_LUMI:
        ui->digitalEffectLevelSpinBox->setMaximum(VISCA_DIGITAL_EFFECT_LEVEL_LUMI_STILL_MAX);
        ui->digitalEffectLevelSpinBox->setEnabled(true);
        break;
    default:
        ui->digitalEffectLevelSpinBox->setEnabled(false);
        break;
    }

    VISCA_set_digital_effect(&interface, &camera, index);
}

void CameraWindow::on_digitalEffectLevelSpinBox_valueChanged(int arg1)
{
    VISCA_set_digital_effect_level(&interface, &camera, arg1);
}

void CameraWindow::on_pictureFreezeCheckBox_stateChanged(int arg1)
{
    switch (arg1)
    {
    case Qt::Unchecked:
        VISCA_set_freeze(&interface, &camera, VISCA_POWER_OFF);
        break;
    case Qt::Checked:
        VISCA_set_freeze(&interface, &camera, VISCA_POWER_ON);
        break;
    }
}

void CameraWindow::on_pictureMirrorCheckBox_stateChanged(int arg1)
{
    switch (arg1)
    {
    case Qt::Unchecked:
        VISCA_set_mirror(&interface, &camera, VISCA_POWER_OFF);
        break;
    case Qt::Checked:
        VISCA_set_mirror(&interface, &camera, VISCA_POWER_ON);
        break;
    }
}

void CameraWindow::on_memoryListWidget_itemSelectionChanged()
{
    ui->memoryButtonsContainerWidget->setEnabled(true);
}

void CameraWindow::on_memoryResetButton_clicked()
{
    VISCA_memory_reset(&interface, &camera, ui->memoryListWidget->currentRow());
}

void CameraWindow::on_memorySetButton_clicked()
{
    VISCA_memory_set(&interface, &camera, ui->memoryListWidget->currentRow());
}

void CameraWindow::on_memoryRecallButton_clicked()
{
    VISCA_memory_recall(&interface, &camera, ui->memoryListWidget->currentRow());
}
