#include "PropertyGroupManagerCore.h"

namespace UIManagerFactory {
    LUMENGINE_API std::unique_ptr<IUIManager> createUIManager();
}

PropertyGroup* PropertyGroupManagerCore::createPropertyGroup(QWidget* parent, const std::string& title)
{
    auto group = new PropertyGroup(title, parent);
    return group;
}