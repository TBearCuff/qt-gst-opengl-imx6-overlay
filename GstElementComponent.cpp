#include "GstElementComponent.h"
#include <QDebug>

GstElementComponent::GstElementComponent()
{

}

bool GstElementComponent::VInit(QDomElement &data)
{
    QString factoryName = data.attribute("factory-name");
    QString name = data.attribute("name");
    m_strType = data.attribute("type");

    qDebug() << "Adding " << data.tagName() << " of type: " << m_strType
             << "with name: " << name << " and factory name: " << factoryName;

    //Elements are of type source, sink, or 'filter'
    if(m_strType == "source" || m_strType == "sink" || m_strType == "filter")
    {
        m_pElement = gst_element_factory_make(factoryName.toStdString().c_str(), name.toStdString().c_str());

        for(QDomNode n = data.firstChild(); !n.isNull(); n = n.nextSibling())
        {
            QDomElement e = n.toElement();
            if(e.tagName() == "Setting")
            {
                QString variable = e.attribute("variable");
                QString value = e.attribute("value");
                QString dataType = e.attribute("data-type", "str");

                //should check if a variable is a property of g_object factoryName
                qDebug() << "Setting variable: " << variable << "of element: " << name << "to " << value;

                if(dataType == "str")
                {
                    g_object_set(G_OBJECT(m_pElement), variable.toStdString().c_str(), value.toStdString().c_str(), NULL);
                }
                else if(dataType == "int")
                {
                    g_object_set(G_OBJECT(m_pElement), variable.toStdString().c_str(), value.toInt(), NULL);
                }

            }
        }
    }

    return true;
}
