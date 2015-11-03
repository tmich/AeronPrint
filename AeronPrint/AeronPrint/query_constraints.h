#pragma once
#include <string>

struct SortConstraint
{
	enum SortDirection
	{
		ASCENDING,
		DESCENDING

	};

	SortConstraint() {}

	SortConstraint(std::string field, SortDirection direction) {
		Field = field;
		Direction = direction;
	}

	std::string Field;
	SortDirection Direction;
};


struct FilterConstraint
{
	enum FilterOperation
	{
		EQUALS,
		LIKE,
		NOT_EQUALS,
		GREATER_THAN,
		LOWER_THAN
	};

	std::string Operator()
	{
		switch (Operation)
		{
		case FilterConstraint::EQUALS:
			return " = ";
		case FilterConstraint::LIKE:
			return " like ";
		case FilterConstraint::NOT_EQUALS:
			return " != ";
		case FilterConstraint::GREATER_THAN:
			return " > ";
		case FilterConstraint::LOWER_THAN:
			return " < ";
		default:
			return "";
		}
	}
	
	FilterConstraint() : Field(""), Operation(FilterConstraint::EQUALS), Value("") {}
	FilterConstraint(std::string field, FilterOperation op, std::string value) : Field(field), Operation(op), Value(value) {};

	std::string Field;
	FilterOperation Operation;
	std::string Value;
};


class QueryConstraints
{
public:
	QueryConstraints();
	virtual ~QueryConstraints();

	void Sort(std::string field, SortConstraint::SortDirection direction);
	SortConstraint Sort() { return sort_; }


	void Limit(int limit);
	int Limit() { return limit_; }


	void Filter(std::string field, FilterConstraint::FilterOperation operation, std::string value);
	FilterConstraint Filter() { return filter_; }

protected:
	SortConstraint sort_;
	int limit_;
	FilterConstraint filter_;
};

