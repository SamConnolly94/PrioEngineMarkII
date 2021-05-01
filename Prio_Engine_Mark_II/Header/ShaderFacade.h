#pragma once
#include "EShaderType.h"
#include <Windows.h>
#include "d3dUtil.h"

using Microsoft::WRL::ComPtr;
using namespace std;

namespace PrioEngineII
{
	class ShaderFacade
	{
	public:
		ShaderFacade(std::string directory, std::string filename, EShaderType shaderType);
	private:
		std::string mDirectory;
		std::string mFilename;
		EShaderType mShaderType;
		ComPtr<ID3DBlob> mShaderByteCode = nullptr;
	public:
		void Compile();
	};
}
