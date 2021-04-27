# Prio Engine Mark II
Prio Engine Mark II is the follow up engine from Prio Engine. This engine strives to make a much larger effort to adhere to SOLID principles, and will support DirectX 12.

## Installation
Prio Engine Mark II requires Visual Studio 2019 and the Windows SDK containing Direct X 12 in order to build. Please ensure these are present on your system, then simply launch the sln file and build to build.

### Important Installation Note
Currently Prio Engine Mark II relies on some files from Frank Luna's 3D Game Programming With Direct X 12 book. These files are not included within the engine to avoid Copyright infringement. If you own this book and have access to the resources you can compile by including the files:
* Camera.h
* d3dUtil.h
* d3dx12.h
* DDSTextureLoader.h
* GameTimer.h
* GeometryGenerator.h
* MathHelper.h
* UploadBuffer.h

## Usage
The build will produce a static library, you can include that static library in another Windows project and define a main function for a Windows project. The solution consists of three projects, a static library project, a testing project, and a runnable project. The runnable project is for you to invoke and test the engine with, the testing project is for you to write your unit tests, and the static library project will build the static objects which 


### Windows Main Function
```int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)```

## Code Style
The primary purpose of this engine is to create a simplistic rendering engine, that can load models and perform basic transforms. By adhering to SOLID principles, we can ensure that our code is testable. More specifically, we should ensure that we utilise constructor dependency injection, this will enable us to inject mocks into our objects when we test. 

## Testing
Google Test is included in the project. It should be used for unit testing new features of the engine. 

## Workflow
The Workflow of this repository will adhere to GitFlow Workflow. Meaning that there should be appropriately raised tickets, and corresponding feature branches. All tickets should be raised using the GitHub Projects issues tab.