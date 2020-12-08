#pragma once

namespace FluidHTN
{
class Domain;
}

class Item
{
protected:
	std::shared_ptr<FluidHTN::Domain> _domain;

	std::vector<KeywordsToActionEntry> _KeywordsToAction;

	Item(std::shared_ptr<FluidHTN::Domain> dom)
	{
		_domain = dom;
	}
public:
	std::shared_ptr<FluidHTN::Domain> Domain() { return _domain; }

	virtual const std::string& Description() = 0;

	const std::vector<KeywordsToActionEntry>& KeywordsToAction() { return _KeywordsToAction; }
};
