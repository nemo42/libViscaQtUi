#include "camerawindow.h"
#include "ui_camerawindow.h"

CameraWindow::CameraWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::CameraWindow),
      ttyDev("/dev/ttyS0")
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
