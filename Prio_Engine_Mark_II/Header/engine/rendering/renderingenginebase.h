#pragma once
class CRenderingEngineBase
{
public:
    CRenderingEngineBase();
protected:
    bool InitWindow();
    virtual bool InitGraphicsApi() = 0;
    void OnResize();
private:
    bool Initialize();
};

