// File src/headers/Interaction.h

#ifndef INTERACTION_H
#define INTERACTION_H

#include "OpenAIClient.h"
#include <algorithm>
#include <iostream>
#include <string>

namespace Interaction
{
	void runMainLoop(OpenAIClient::Client &client);
}

#endif // INTERACTION_H
