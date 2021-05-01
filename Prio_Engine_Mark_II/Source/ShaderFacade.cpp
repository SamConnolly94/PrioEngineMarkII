#include "pch.h"
#include "ShaderFacade.h"
#include "EnumUtilities.h"
#include "Logger.h"

using namespace PrioEngineII;

ShaderFacade::ShaderFacade(std::string directory, std::string filename, EShaderType shaderType, bool bDefault/* = false*/)
{
	mDirectory = directory;
	mFilename = filename;
	mShaderType = shaderType;
	mDefault = bDefault;
}

void ShaderFacade::Compile()
{
	std::string strShaderFile = mDirectory + mFilename;
	std::wstring wideStrShaderFile(strShaderFile.begin(), strShaderFile.end());

	std::string shaderType;
	std::string shaderVersion;
	switch (mShaderType)
	{
	case EShaderType::Vertex:
		shaderType = "VS";
		shaderVersion = "vs_5_0";
		break;
	case EShaderType::Geometry:
		shaderType = "GS";
		shaderVersion = "gs_5_0";
		break;
	case EShaderType::Pixel:
		shaderType = "PS";
		shaderVersion = "ps_5_0";
		break;
	default:
		Logger::GetInstance()->Write("Unable to convert shader type from '" + EnumUtilities::ToString(mShaderType) + "' to a shader type DirectX will recognise.", ELogVerbosity::Error);
		break;
	}

	mShaderByteCode = d3dUtil::CompileShader(wideStrShaderFile, nullptr, shaderType, shaderVersion);
}

bool ShaderFacade::IsDefault() const
{
	return mDefault;
}

EShaderType ShaderFacade::GetShaderType() const
{
	return mShaderType;
}

ComPtr<ID3DBlob> PrioEngineII::ShaderFacade::GetByteCode() const
{
	return mShaderByteCode;
}

LPVOID ShaderFacade::GetBufferPtr() const
{
	return mShaderByteCode->GetBufferPointer();
}