#pragma once

#include <Windows.h>
#include <PrioEngineFactory.h>
#include <d3dUtil.h>

using namespace std;
using namespace PrioEngineII;

std::shared_ptr<PrioEngine> CreateEngine(const HINSTANCE& hInstance);
std::shared_ptr<PrioEngine> myEngine;