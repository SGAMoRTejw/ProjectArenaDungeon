#include "pch.h"
#include "Component.h"

Component::Component(std::string compName)
	: name(std::move(compName))
{
}