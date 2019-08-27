#ifndef GSTELEMENTCOMPONENT_H
#define GSTELEMENTCOMPONENT_H

#include "interfaces.h"
#include <gst/gst.h>

class GstElementComponent : public VidPipelineComponent
{
    GstElement* m_pElement;
    QString m_strType;
public:
    GstElementComponent();

    virtual bool VInit(QDomElement &data) override;

    //accessor function
    GstElement* GetElement() const {return m_pElement;}
    QString GetType() {return m_strType;}

};

#endif // GSTELEMENTCOMPONENT_H
