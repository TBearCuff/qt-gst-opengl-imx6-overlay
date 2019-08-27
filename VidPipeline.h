#ifndef VIDPIPELINE_H
#define VIDPIPELINE_H

#include <QDomElement>
#include <gst/gst.h>
#include "GstElementComponent.h"

class VidPipeline
{
    GstElement *m_pipeline;

    friend class PipelineFactory;

    GstElement *source;
    GstElement *sink;

    //this will need to change to a list of filter elements
    StrongPipelineComponentPtr filter;

private:
    VidPipelineId m_id;
public:

    VidPipeline(VidPipelineId id);
    ~VidPipeline();

    bool Init(QDomElement &data);
    void PostInit();

    bool SetState(GstState state);


    void AddComponent(StrongPipelineComponentPtr pComponent);
};

#endif // VIDPIPELINE_H
