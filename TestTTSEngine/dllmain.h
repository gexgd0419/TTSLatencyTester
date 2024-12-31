// dllmain.h: 模块类的声明。

class CTestTTSEngineModule : public ATL::CAtlDllModuleT< CTestTTSEngineModule >
{
public :
	DECLARE_LIBID(LIBID_TestTTSEngineLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TESTTTSENGINE, "{7c28a18e-4f5e-44eb-a64c-afdc298c2a89}")
};

extern class CTestTTSEngineModule _AtlModule;
