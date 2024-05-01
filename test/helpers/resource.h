#pragma once
#include "../../list/list.h"
#include <gtest/gtest.h>
#include <cassert>

class testResource
{
private:
	uint16_t testvalue;

public:
	testResource() :testvalue(0) { ++instancesCreated; }
	static unsigned copyConstructor;
	static unsigned movementConstructor;
	static unsigned instancesCreated;

	testResource(uint16_t testvalue_) : testvalue(testvalue_) { ++instancesCreated; }
	testResource(const testResource& res) :testvalue(res.testvalue) { ++copyConstructor; }
	testResource(testResource&& res) noexcept : testvalue(std::move(res.testvalue)) { ++movementConstructor; }
};

unsigned testResource::copyConstructor = 0;
unsigned testResource::movementConstructor = 0;
unsigned testResource::instancesCreated = 0;

class testResourceList : public ::testing::Test
{
protected:
	list<testResource> listForTesting;
	void SetUp() override
	{
		testResource::copyConstructor = 0;
		testResource::movementConstructor = 0;
		testResource::instancesCreated = 0;
		listForTesting.clear();
		assert(listForTesting.size() == 0);
	}
};
