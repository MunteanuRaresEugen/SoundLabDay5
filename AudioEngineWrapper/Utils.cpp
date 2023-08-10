#include "pch.h"
#include "Utils.h"
#include <msclr/marshal_cppstd.h>

std::string AudioEngineWrapper::Utils::ConvertToStdString(System::String^ str)
{
    return msclr::interop::marshal_as<std::string>(str);
}
