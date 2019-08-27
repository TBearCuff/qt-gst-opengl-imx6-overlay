#ifndef PIPELINEFACTORY_H
#define PIPELINEFACTORY_H

#include <QString>
#include "interfaces.h"

class PipelineFactory
{
    VidPipelineId m_lastPipelineId;
public:
    PipelineFactory();

    StrongPipelinePtr CreatePipeline(QString pipelineResource);

    virtual StrongPipelineComponentPtr VCreateComponent(QDomElement &data);


private:
    VidPipelineId GetNextPipelineId() {++m_lastPipelineId; return m_lastPipelineId;}
};

#endif // PIPELINEFACTORY_H
