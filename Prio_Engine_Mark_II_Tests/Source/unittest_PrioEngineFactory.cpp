// unittest_PrioEngineFactory.cpp : Defines the entry point for the console application.

#include <gtest/gtest.h>
#include <PrioEngineFactory.h>
#include "EngineCreationException.h"

using namespace PrioEngineII;


TEST(testEngineFactory, factoryReturnsDx12Engine)
{
    HINSTANCE hInstance = NULL;
    auto enginePtr = PrioEngineFactory::CreateEngine(EEngineTypes::DX3D12, hInstance);
    ASSERT_NE(enginePtr, nullptr);
}

TEST(testEngineFactory, dx11EngineNotImplemented)
{
    HINSTANCE hInstance = NULL;
    try 
    {
        auto enginePtr = PrioEngineFactory::CreateEngine(EEngineTypes::DX3D11, hInstance);
        FAIL() << "Expected engine exception to be thrown, however no exception was thrown.";
    } 
    catch (EngineCreationException& e)
    {
        EXPECT_EQ(e.what(), "An exception occured when creating the engine.");
    }
    catch (...)
    {
        FAIL() << "Unexpected exception was thrown, was expecting EngineCreationException.";
    }
}