#include <PluS.h>

#include <iostream>

int main()
{
	std::string payloadPath = "C:\\dev\\proj\\PluS\\out\\build\\x64-Debug\\test\\TestPayload\\TestPayload.dll";
	PluS::ProcessID processID = 21200;

	auto& pm = PluS::PluginManager::get();

	PluS::PayloadID payloadID = pm.injectPayload(payloadPath, processID);

	if (!payloadID)
		return -1;

	auto payload = pm.getPayloadHandle(payloadID);

	std::string input;
	while (true)
	{
		std::cout << " >>> ";
		std::getline(std::cin, input);

		if (input == "exit")
			break;

		if (!payload->call("CallMeIfYouCan", input.c_str(), input.size() + 1))
			return -1;
	}

	return 0;
}