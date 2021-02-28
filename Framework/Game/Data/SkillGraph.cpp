#include "SkillGraph.hpp"

bool SkillGraphInfo::HasSkillByName(const std::string& name) const
{
	return skills_.find(name) != skills_.end();
}

const SkillGraphInfo::Skill& SkillGraphInfo::GetSkillByName(const std::string& name) const
{
	assert(HasSkillByName(name) && "not skill");

	return skills_.find(name)->second;
}
