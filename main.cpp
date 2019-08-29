#include <QApplication>
#include "mainwindow.h"

#include <QWidget>
#include <QTimer>
#include <glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "PipelineFactory.h"
#include "VidPipeline.h"


#define MXCFB_SET_GBL_ALPHA _IOW('F', 0x21, struct mxcfb_gbl_alpha)
#define MXCFB_SET_CLR_KEY _IOW('F', 0x22, struct mxcfb_color_key)

struct mxcfb_gbl_alpha {
        int enable;
        int alpha;
};

struct mxcfb_color_key {
        int enable;
        unsigned int color_key;
};

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    QApplication app(argc, argv);


    PipelineFactory *m_pPipelineFactory = new PipelineFactory;
    StrongPipelinePtr pPipeline = m_pPipelineFactory->CreatePipeline(QString("/run/media/mmcblk2p6/xml/MainPipeline.xml"));
    if(pPipeline)
    {
        pPipeline->SetState(GST_STATE_PLAYING);
    }
    else
    {
        qDebug() << "Pipeline was not created!";
    }


    // Set OpenGL Version information
    // Note: This format must be set before show() is called.
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGLES);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(2,0);

    int fd_fb = open("/dev/fb1", O_RDWR, 0);
    if (fd_fb < 0) {
        printf("unable to open fb1\n");
        return 0;
    }


    struct mxcfb_gbl_alpha a;
    a.enable = 1;
    a.alpha = 255;
    int err = ioctl(fd_fb,MXCFB_SET_GBL_ALPHA,&a);
    if ( err ) {
        perror( "MXCFB_SET_GBL_ALPHA");
        return 0;
    }

    struct mxcfb_color_key c;
    c.enable = 1;
    c.color_key = 0x0;
    err = ioctl(fd_fb,MXCFB_SET_CLR_KEY,&c);
    if ( err ) {
      perror( "MXCFB_SET_CLR_KEY");
      return 0;
    }
    close (fd_fb);


    //Prepare the UI
    MainWindow window;
    window.setFormat(format);
    window.showFullScreen();


    int ret = app.exec();

//    window.hide();

    return ret;
}

