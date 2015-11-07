#include "query_constraints.h"



QueryConstraints::QueryConstraints()
{
	limit_ = 0;
}


QueryConstraints::~QueryConstraints()
{
}

void QueryConstraints::Sort(std::string field, SortConstraint::SortDirection direction)
{
	sort_.Field = field;
	sort_.Direction = direction;
}

void QueryConstraints::Limit(int limit)
{
	limit_ = limit;
}

void QueryConstraints::Filter(std::string field, FilterConstraint::FilterOperation operation, std::string value)
{
	filter_.Field = field;
	filter_.Operation = operation;
	filter_.Value = value;
}
