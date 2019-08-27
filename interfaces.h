#ifndef INTERFACES_H
#define INTERFACES_H

#include <QSharedPointer>
#include <QDomElement>

class VidPipeline;
class VidPipelineComponent;

typedef unsigned int VidPipelineId;
typedef unsigned int ComponentId;

const VidPipelineId INVALID_PIPELINE_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef QSharedPointer<VidPipeline> StrongPipelinePtr;
typedef QWeakPointer<VidPipeline> WeakPipelinePtr;
typedef QSharedPointer<VidPipelineComponent> StrongPipelineComponentPtr;
typedef QWeakPointer<VidPipelineComponent> WeakPipelineComponentPtr;

class VidPipelineComponent
{
    friend class PipelineFactory;

protected:
    StrongPipelinePtr m_pOwner;

public:
    virtual ~VidPipelineComponent() {m_pOwner.reset();}

    // These functions are meant to be overridden by the implementation classes of components
    virtual bool VInit(QDomElement &data) = 0;

private:
    void SetOwner(StrongPipelinePtr pOwner) { m_pOwner = pOwner; }

};


#endif // INTERFACES_H
