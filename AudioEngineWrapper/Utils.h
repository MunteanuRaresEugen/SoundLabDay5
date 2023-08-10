#pragma once

#include <vector>
#include <functional>
#include <string>

namespace AudioEngineWrapper
{
    ref class Utils
    {
    public:
        template<class T>
        static array<T>^ ConvertToMananged(const std::vector<T>& data, size_t size)
        {
            return ConvertToMananged<T, T>(data, size);
        }

        template<class T, class R>
        static array<R>^ ConvertToMananged(const std::vector<T>& data, size_t size)
        {
            array<R>^ managedData = gcnew array<R>(size);

            for (size_t i = 0; i < size; i++)
            {
                managedData[i] = data[i];
            }

            return managedData;
        }

        template<class T, class R>
        static array<R>^ ConvertToManangedWithFunc(const std::vector<T>& data, std::function<R(const T&)> f, size_t size)
        {
            array<R>^ managedData = gcnew array<R>(size);

            for (size_t i = 0; i < size; i++)
            {
                managedData[i] = f(data[i]);
            }

            return managedData;
        }

        static std::string ConvertToStdString(System::String^ str);
    };
}

