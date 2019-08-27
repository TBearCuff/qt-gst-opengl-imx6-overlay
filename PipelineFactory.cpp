
#include "PipelineFactory.h"
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include "VidPipeline.h"
#include "GstElementComponent.h"

PipelineFactory::PipelineFactory() : m_lastPipelineId(INVALID_PIPELINE_ID)
{

}

StrongPipelinePtr PipelineFactory::CreatePipeline(QString pipelineResource)
{
    //pipelineResource is the name of a file containing the pipeline
    //grab the root node
    QDomDocument doc;
    QFile file(pipelineResource);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open file: " << pipelineResource;
        return StrongPipelinePtr();
    }

    if(!doc.setContent(&file))
    {
        qDebug() << "Failed to parse the XML file" << pipelineResource;
        file.close();
        return StrongPipelinePtr();
    }

    file.close();

    QDomElement root = doc.documentElement();

    if(!root.isElement())
    {
        qDebug() << "Failed to create pipeline from resource: " << pipelineResource;
        return StrongPipelinePtr();
    }

    //create the pipeline instance
    VidPipelineId nextPipelineId = GetNextPipelineId();
    StrongPipelinePtr pPipeline(new VidPipeline(nextPipelineId));
    if(!pPipeline->Init(root))
    {
        qDebug() << "Failed to initialize pipeline: " << pipelineResource;
        return StrongPipelinePtr();
    }

    //Loop through each child element and load the component
    for(QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement e = n.toElement();
        StrongPipelineComponentPtr pComponent(VCreateComponent(e));
        if(pComponent.isNull())
        {
            //If an error occurs we bail on pipeline creation because the pipeline
            //will be incomplete and hosed
            return StrongPipelinePtr();
        }
        else
        {
            //it's good so add component to pipeline and pipeline owner of component
            pPipeline->AddComponent(pComponent);
            pComponent->SetOwner(pPipeline);
        }
    }

    pPipeline->PostInit();

    return pPipeline;
}

StrongPipelineComponentPtr PipelineFactory::VCreateComponent(QDomElement &data)
{
    QString name = data.tagName();

    qDebug() << "Adding Component: " << name;
    //this creates only one component: the GstElementComponent
    //replace this with a 'component factory' that returns a component based upon name
    StrongPipelineComponentPtr pComponent(new GstElementComponent);

    //initialize the component if we found one
    if(pComponent.isNull())
    {
        qDebug() << "Couldn't find pipeline component named: " << name;
        return StrongPipelineComponentPtr();
    }
    else
    {
        if(!pComponent->VInit(data))
        {
            qDebug() << "Component failed to initialize: " << name;
            return StrongPipelineComponentPtr();
        }
    }

    return pComponent;
}
