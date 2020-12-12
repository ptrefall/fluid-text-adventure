#pragma once

namespace FluidHTN
{
class Domain;
}

class Item
{
protected:
	SharedPtr<FluidHTN::Domain> _domain;

	std::vector<KeywordsToActionEntry> _KeywordsToAction;

	Item(SharedPtr<FluidHTN::Domain> dom)
	{
		_domain = dom;
	}
public:
	SharedPtr<FluidHTN::Domain> Domain() { return _domain; }

	virtual const std::string& Description() = 0;

	const std::vector<KeywordsToActionEntry>& KeywordsToAction() { return _KeywordsToAction; }
};
