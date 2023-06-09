/******************************************************************************************************************************************/
// File : Application.cpp
// Purpose : Implementing the Application
/******************************************************************************************************************************************/
#include "Application.hpp"

using namespace Starting;

Application::Application()
{
    m_service_builder = std::make_unique<Builders::ServiceBuilder>();
    m_engine_builder = std::make_unique<Builders::EngineBuilder>();
}

void Application::EndingBuilders()
{
    m_service_builder->OnBuilderEnd();
    m_engine_builder->OnBuilderEnd();

    m_service_builder.reset();
    m_engine_builder.reset();
}

void Application::StartAllBuilders()
{
    std::unique_ptr<IoC::IocModule> ioc_module = std::make_unique<IoC::IocModule>();
    ioc_module->StartBuilder(m_service_builder.get());
    ioc_module->StartBuilder(m_engine_builder.get());
    ioc_module.reset();
}