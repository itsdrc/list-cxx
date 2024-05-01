#include <gtest/gtest.h>
#include "../list/list.h"
#include "helpers/resource.h"

using intlist = list<int>;

// front

TEST(front, shouldReturnTheFront)
{
	const int frontExpected = 1;
	intlist list;
	list.push_back(frontExpected);
	list.push_back(2);
	list.push_back(3);
	EXPECT_EQ(list.front(), frontExpected);
}

TEST(front, shouldThrowLengthError)
{
	intlist emptylist;
	EXPECT_THROW(emptylist.front(), std::length_error);
}

// back

TEST(front, shoudlReturnTheBack)
{
	const int valueExpected = 3;
	intlist list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	EXPECT_EQ(list.back(), valueExpected);
}

TEST(back, shouldThrowLengthError)
{
	intlist emptylist;
	EXPECT_THROW(emptylist.back(), std::length_error);
}

// size 

TEST(size, shouldReturnTheCorrectSize)
{
	intlist list;
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);
	EXPECT_EQ(list.size(), 3);
}

TEST(size, sizeShouldBe0ifListEmpty)
{
	intlist list;
	EXPECT_EQ(list.size(), 0);
}

// empty

TEST(empty, shouldReturnFalseIfListNotEmpty)
{
	intlist list;
	list.push_back(1);
	EXPECT_FALSE(list.empty());
}

TEST(empty, shouldReturnTrueIfListEmpty)
{
	intlist list;
	EXPECT_TRUE(list.empty());
}

// pop back


TEST(pop_back, shouldPoptheback)
{
	intlist list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);

	list.pop_back();

	EXPECT_EQ(list.back(), 2);
}

TEST(pop_back, ifListEmptyShouldThrowException)
{
	intlist emptylist;
	EXPECT_THROW(emptylist.pop_back(), std::length_error);
}

// pop front

TEST(pop_front, shouldPopTheFront)
{
	intlist list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);

	list.pop_front();
	EXPECT_EQ(list.front(), 2);
}

TEST(pop_front, ifListEmptyShouldThrowException)
{
	intlist emptylist;
	EXPECT_THROW(emptylist.pop_front(), std::length_error);
}

// initializer list

TEST(initializerListConstructor, shouldConstructTheList)
{
	intlist list{ 1, 2, 3, 4, 5 };
	EXPECT_EQ(list.size(), 5);
}

// copy constructor

TEST(copyConstructor, shouldCopySuccefully)
{
	intlist list{ 1, 2, 3 };
	intlist list2 = list;
	ASSERT_EQ(list.front(), 1);
	list.pop_front();
	ASSERT_EQ(list.front(), 2);
	list.pop_front();
	ASSERT_EQ(list.front(), 3);
}

TEST(copyConstructor, shouldsetNelms)
{
	intlist list{ 1, 2, 3, 4, 5 };
	intlist list2 = list;
	EXPECT_EQ(list.size(), list2.size());
}

// emplace back

TEST(emplace_back, shouldBeInsertInTheBack)
{
	const int newvalue = 4;
	intlist testlist{ 1,2,3 };
	testlist.emplace_back(newvalue);
	EXPECT_EQ(testlist.back(), newvalue);
}

TEST_F(testResourceList, dontCopyUsingEmplaceBack)
{
	listForTesting.emplace_back(100);
	EXPECT_EQ(testResource::instancesCreated, 1);
}

TEST_F(testResourceList, updateNelmsUsingEmplaceBack)
{
	listForTesting.emplace_back(100);
	EXPECT_EQ(listForTesting.size(), 1);
}

// emplace front

TEST(emplace_front, shouldBeInsertInTheFront)
{
	const int newvalue = 4;
	intlist testlist{ 1,2,3 };
	testlist.emplace_front(newvalue);
	EXPECT_EQ(testlist.front(), newvalue);
}

TEST_F(testResourceList, dontCopyUsingEmplaceFront)
{
	listForTesting.emplace_front(100);
	EXPECT_EQ(testResource::instancesCreated, 1);
}

TEST_F(testResourceList, updateNelmsUsingEmplaceFront)
{
	listForTesting.emplace_front(100);
	EXPECT_EQ(listForTesting.size(), 1);
}

// constructor by movement 

TEST_F(testResourceList, constructorByMovementshouldNotMakeCopies)
{
	listForTesting.emplace_back(testResource());
	listForTesting.emplace_back(testResource());
	listForTesting.emplace_back(testResource());
	list<testResource> resourceList = std::move(listForTesting);
	EXPECT_EQ(testResource::copyConstructor, 0);
}

// clear

TEST(destructor_clear, noProblemUsingDestructorAfterClear)
{
	{
		intlist list{ 1, 2, 3 };
		list.clear();
	}
	EXPECT_EQ(1, 1);
}

TEST(destructor_clear, nelmsShouldBeZero)
{
	intlist list{ 1, 2, 3 };
	list.clear();
	std::cout << list.size();
}

TEST(destructor_clear, noProblemIfListItsEmpty)
{
	intlist list;
	list.clear();
	EXPECT_EQ(1, 1);
}

TEST(destructor_clear, canPushAfterClearWithNoProblem)
{
	intlist list{ 1, 2, 3 };
	list.clear();
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	ASSERT_EQ(list.size(), 3);
	ASSERT_EQ(list.front(), 1);
	ASSERT_EQ(list.back(), 3);
}



// push back &&

TEST(push_backByMovement, shouldInsertInTheBack)
{
	int newvalue = 4;
	intlist testlist{ 1,2,3 };
	testlist.push_back(std::move(newvalue));
	EXPECT_EQ(testlist.back(), newvalue);
}

TEST_F(testResourceList, pushBackDontCopyUsingStdMove)
{
	testResource resource;
	listForTesting.push_back(std::move(resource));
	EXPECT_EQ(testResource::instancesCreated, 1);
	EXPECT_EQ(testResource::movementConstructor, 1);
}

TEST_F(testResourceList, updateNelmsUsingPushBackbyMovement)
{
	testResource resource;
	listForTesting.push_back(std::move(resource));
	EXPECT_EQ(listForTesting.size(), 1);
}

// push front && 

TEST_F(testResourceList, updateNelmsUsingPushFrontbyMovement)
{
	testResource resource;
	listForTesting.push_front(std::move(resource));
	EXPECT_EQ(listForTesting.size(), 1);
	EXPECT_EQ(testResource::movementConstructor, 1);
}

TEST_F(testResourceList, pushFrontDontCopyUsingStdMove)
{
	testResource resource;
	listForTesting.push_front(std::move(resource));
	EXPECT_EQ(testResource::instancesCreated, 1);
}