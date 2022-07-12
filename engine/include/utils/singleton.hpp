#pragma once

#include <memory>

template <class T>
class Singleton
{
protected:
    Singleton() = default;

    static std::shared_ptr<T> currentInstance;

    [[nodiscard]] static T& Instance()
    {
        if (!currentInstance)
        {
            struct MakeSharedEnabler : public T {};
            currentInstance = std::make_shared<MakeSharedEnabler>();
        }

        return *currentInstance.get();
    }

public:
    Singleton(T&) = delete;

    void operator=(const T&) = delete;
};

template<class T>
std::shared_ptr<T> Singleton<T>::currentInstance = nullptr;