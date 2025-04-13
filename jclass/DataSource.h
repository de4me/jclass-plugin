#pragma once


#include "JavaClass.hpp"


class DataSource {

public:

	virtual const std::string& filePath() = 0;
	virtual java::JavaClass& java() = 0;
};
