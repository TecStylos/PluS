#include <PluS.h>

int main()
{
	std::string payloadPath = "C:\\dev\\proj\\PluS\\out\\build\\x64-Debug\\test\\TestPayload\\TestPayload.dll";
	PLUS_PROCESS_ID processID = 13656;

	auto lib = LoadLibraryA(payloadPath.c_str());

	auto& pm = PluS::PluginManager::get();

	PluS::PayloadID payloadID = pm.injectPayload(payloadPath, processID);

	if (!payloadID)
		return -1;

	auto payload = pm.getPayloadHandle(payloadID);

	if (!payload->call("CallMeIfYouCan", nullptr))
		return -1;

	return 0;
}