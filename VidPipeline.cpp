
#include "VidPipeline.h"
#include <QDebug>
#include "GstElementComponent.h"

VidPipeline::VidPipeline(VidPipelineId id) : m_id(id)
{

}

VidPipeline::~VidPipeline()
{
    qDebug() << "Pipeline: Destroying Video Pipeline: " << m_id;
    gst_element_set_state(m_pipeline, GST_STATE_NULL);
    gst_object_unref(m_pipeline);
}

bool VidPipeline::Init(QDomElement& data)
{
    if(data.tagName() != "Pipeline")
    {
        qDebug() << "Tag element does not indicate that this is a pipeline XML file.";
        return false;
    }

    QString pipeName = data.attribute("name");
    qDebug() << data.tagName() << ": Initializing Pipeline: " << m_id << ", " << pipeName;
    const char* name = pipeName.toStdString().c_str();
    m_pipeline = gst_pipeline_new(name);
    return true;
}

void VidPipeline::PostInit()
{
    QSharedPointer<GstElementComponent> pElementComponent = filter.staticCast<GstElementComponent>();
    //After the components are added to the pipeline, they need to be linked
    gst_element_link_many(source, pElementComponent->GetElement(), sink, NULL);

}

bool VidPipeline::SetState(GstState state)
{
    GstStateChangeReturn sret = gst_element_set_state(m_pipeline, state);
    if(sret == GST_STATE_CHANGE_FAILURE)
    {
        gst_element_set_state(m_pipeline, GST_STATE_NULL);
        gst_object_unref(m_pipeline);
        return false;
    }
    return true;
}

void VidPipeline::AddComponent(StrongPipelineComponentPtr pComponent)
{
    QSharedPointer<GstElementComponent> pElementComponent = pComponent.staticCast<GstElementComponent>();
    gst_bin_add(GST_BIN(m_pipeline), pElementComponent->GetElement());
    if(pElementComponent->GetType() == "source")
    {
        source = pElementComponent->GetElement();
    }
    if(pElementComponent->GetType() == "sink")
    {
        sink = pElementComponent->GetElement();
    }
    if(pElementComponent->GetType() == "filter")
    {
        filter = pElementComponent;
    }

}
