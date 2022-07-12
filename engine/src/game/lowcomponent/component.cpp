#include "game/entity.hpp"
#include "game/entity_manager.hpp"
#include "game/transform.hpp"
#include "game/lowcomponent/component.hpp"

#include "debug/log.hpp"

using namespace Game;

std::vector<ClassMetaData*>& Component::GetRegistry()
{
    static std::vector<ClassMetaData*> componentRegistry;
    return componentRegistry;
}

ClassMetaData* Component::FindMetaData(const std::string& i_compName)
{
    std::vector<ClassMetaData*>& registry = GetRegistry();
    auto it = std::find_if(registry.begin(), registry.end(), [&i_compName](ClassMetaData* md){ return i_compName == md->className;});

    if (it != registry.end())
        return *it;

    Log::Error("Can't find component ", i_compName);

    return nullptr;
}

Component::Component()
{
    enabled.setter = [&](const bool& value)
    {
        bool prevValue = enabled.get();
        enabled.set(value);
        if (value && !prevValue)
            OnEnable();
        else if (!value && prevValue)
            OnDisable();
    };
    owner.setter = std::bind(&Component::SetOwner, this, std::placeholders::_1);
    enabled = true;
}

void Component::Start()
{
    if (!isStarted)
    {
        isStarted = true;
        OnStart();
    }
}

bool Component::IsStarted() const
{
    return isStarted;
}

void Component::SetOwner(Entity* entity) 
{
    owner.set(entity);
}

Property<Vector3>& Component::Position() const
{
    return owner.get()->transform.position;
}

Property<Vector3>& Component::Scale() const
{
    return owner.get()->transform.scale;
}

Property<Vector3>& Component::Rotation() const
{
    return owner.get()->transform.rotation;
}

EntityContainer& Component::GetEntityContainer() const
{
    return *owner.get()->entityStore;
}

Transform& Component::GetTransform() const
{
    return owner.get()->transform;
}

Transform& Component::GetRootTransform() const
{
    return owner.get()->GetRootEntity()->transform;
}