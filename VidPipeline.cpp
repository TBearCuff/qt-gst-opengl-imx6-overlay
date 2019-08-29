
#include "VidPipeline.h"
#include <QDebug>
#include "GstElementComponent.h"

VidPipeline::VidPipeline(VidPipelineId id) :
    m_pipeline(0),
    m_pSource(0),
    m_pSink(QSharedPointer<GstElementComponent>()),
    m_id(id)
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

    QString m_strName = data.attribute("name");
    qDebug() << data.tagName() << ": Initializing Pipeline: " << m_id << ", " << m_strName;
    const char* name = m_strName.toStdString().c_str();
    m_pipeline = gst_pipeline_new(name);
    return true;
}

bool VidPipeline::PostInit()
{
    //if no source or sink elements then this pipeline will not function
    if(m_pSource.isNull() || m_pSink.isNull())
    {
        qDebug() << "The pipeline needs source and sink elements to be valid.";
        qDebug() << "Elements were not linked.";
        return false;
    }


    //After the components are added to the pipeline, they need to be linked
    if(m_pFilter.isEmpty())
    {
        //link m_pSource to m_pSink
        gst_element_link(m_pSource->GetElement(), m_pSink->GetElement());
    }
    else
    {
        QSharedPointer<GstElementComponent> pPreviousElement = m_pSource;
        FilterList::iterator it = m_pFilter.begin();
        while (it != m_pFilter.end())
        {
            QSharedPointer<GstElementComponent> pNextElement = (*it);
            gst_element_link(pPreviousElement->GetElement(), pNextElement->GetElement());
            pPreviousElement = (*it);
            ++it;
        }
        gst_element_link(pPreviousElement->GetElement(), m_pSink->GetElement());
    }

    return true;
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

bool VidPipeline::AddComponent(StrongPipelineComponentPtr pComponent)
{
    QSharedPointer<GstElementComponent> pElementComponent = pComponent.staticCast<GstElementComponent>();
    bool success = (bool)gst_bin_add(GST_BIN(m_pipeline), pElementComponent->GetElement());
    if(!success)
    {
        qDebug() << "Failed to add component to bin";
        return false;
    }
    if(pElementComponent->GetType() == "source")
    {
        m_pSource = pElementComponent;
    }
    if(pElementComponent->GetType() == "sink")
    {
        m_pSink = pElementComponent;
    }
    if(pElementComponent->GetType() == "filter")
    {
        m_pFilter.append(pElementComponent);
    }

    return true;
}
