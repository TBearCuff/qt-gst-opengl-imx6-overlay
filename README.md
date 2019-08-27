# qt-gst-opengl-imx6-overlay
A project using Qt frameworks to display OpenGL objects over GStreamer sink on iMX6 processor.

This was a toy application to see if an openGL 3D cube could be overlayed over a GStreamer sink. The sink is framebuffer 0 (/dev/fb0). The Qt environemnt variable QT_QPA_EGLFS_FB=/dev/fb1.

GStreamer video pipeline is also constructed using the Factory Design Pattern with construction instructions coming from an xml file (data-driven).
