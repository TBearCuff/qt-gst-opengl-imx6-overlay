#ifndef VIDPIPELINE_H
#define VIDPIPELINE_H

#include <QDomElement>
#include <gst/gst.h>
#include "GstElementComponent.h"

typedef QList<QSharedPointer<GstElementComponent>> FilterList;

class VidPipeline
{
    GstElement *m_pipeline;
    QString m_strName;

    friend class PipelineFactory;

    QSharedPointer<GstElementComponent> m_pSource;
    QSharedPointer<GstElementComponent> m_pSink;

    //this will need to change to a list of m_pFilter elements
    FilterList m_pFilter;

private:
    VidPipelineId m_id;
public:

    VidPipeline(VidPipelineId id);
    ~VidPipeline();

    bool Init(QDomElement &data);
    bool PostInit();

    bool SetState(GstState state);


    bool AddComponent(StrongPipelineComponentPtr pComponent);
};

#endif // VIDPIPELINE_H
