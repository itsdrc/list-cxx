#include <gtest/gtest.h>
#include "../list/list.h"
#include "helpers/resource.h"

using intlist = list<int>;

namespace
{
	testing::AssertionResult compareList(intlist list, intlist list2)
	{
		if (list == list2)
			return testing::AssertionSuccess();
		else
		{
			auto fillStringWithList = [](std::string& str, intlist target)
				{
					for (int e : target)
						str.append(std::to_string(e) + " ");
				};

			std::string list1String;
			std::string list2String;

			fillStringWithList(list1String, list);
			fillStringWithList(list2String, list2);

			return testing::AssertionFailure() << "List 1: " << list1String
				<< " List2: " << list2String;
		}
	}
}

// initializer list

TEST(initializerListConstructor, shouldConstructTheList)
{
	intlist list{ 1, 2, 3, 4, 5 };
	EXPECT_EQ(list.size(), 5);
}

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

TEST(front_constlist, shouldReturnTheFront)
{
	const int front = 10;
	const intlist list{ front,2,3 };
	EXPECT_EQ(list.front(), front);
}

TEST(front_constlist, shouldThrowLengthError)
{
	const intlist emptylist;
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

TEST(back_constlist, shouldReturnTheBack)
{
	const int back = 10;
	const intlist list{ 1,2,back };
	EXPECT_EQ(list.back(), back);
}

TEST(back_constlist, shouldThrowLengthError)
{
	const intlist emptylist;
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

TEST(pop_back, shouldPoptheBack)
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

TEST(pop_back, shouldChangeSize)
{
	intlist list{ 1,2,3 };
	const auto currentSize = list.size();
	list.pop_back();
	EXPECT_EQ(list.size(), currentSize-1);
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

TEST(pop_front, shouldChangeSize)
{
	intlist list{ 1,2,3 };
	const auto currentSize = list.size();
	list.pop_front();
	EXPECT_EQ(list.size(),currentSize-1);
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

TEST(emplace_back, shouldInsertInTheBack)
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

TEST(emplace_front, shouldInsertInTheFront)
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

TEST(constructorByMovement, shouldUpdateNelms)
{
	intlist list{ 1,2,3 };
	intlist list2 = std::move(list);
	EXPECT_EQ(list.size(), 0);
	EXPECT_EQ(list2.size(), 3);
}

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

TEST_F(testResourceList, updateNelmsUsingPushFrontByMovement)
{
	intlist list{ 1,2,3 };
	const auto currentsize = list.size();
	list.push_front(std::move(1));
	EXPECT_EQ(list.size(), currentsize+1);
}

// iterator class

TEST(iterator, beginShouldReturnIteratorToBegin)
{
	const int begin = 1;
	intlist list{ begin,2,3 };
	EXPECT_EQ(*(list.begin()), begin);
}

TEST(iterator, endShouldReturnIteratorToEnd)
{	
	intlist list{ 1,2,3 };
	EXPECT_ANY_THROW(*(list.end()));
}

TEST(iterator, preincrementOperator)
{
	intlist list{ 1, 2, 3 };
	auto it = list.begin();
	EXPECT_EQ(*(++it), 2);
	EXPECT_EQ(*(++it), 3);
}

TEST(iterator, postincrementOperator)
{
	intlist list{ 1, 2, 3 };
	auto it = list.begin();
	EXPECT_EQ(*(it++), 1);
	EXPECT_EQ(*(it++), 2);
	EXPECT_EQ(*(it++), 3);
}

TEST(iterator, predecrementOperator)
{
	intlist list{ 1, 2, 3 };
	auto it = list.end();
	EXPECT_EQ(*(--it), 3);
	EXPECT_EQ(*(--it), 2);
}

TEST(iterator, postdecrementOperator)
{
	intlist list{ 1,2,3 };
	auto it = list.end();
	it--;
	EXPECT_EQ(*(it--), 3);
	EXPECT_EQ(*(it--), 2);
	EXPECT_EQ(*(it--), 1);
}

TEST(iterator, shouldThrowExceptionIfTriesToGetHeadValue)
{
	intlist list;
	auto it = list.end();
	EXPECT_THROW(*it, std::runtime_error);
}

TEST(iterator, shouldBeAbleToUseStdAdvance)
{
	intlist list{ 1, 2, 3 };
	auto it = list.begin();
	std::advance(it, 2);
	EXPECT_EQ(*it, 3);
}

TEST(iterator, equalityOperator)
{
	intlist list{ 1, 2, 3 };
	auto it1 = list.begin();
	auto it2 = list.begin();
	EXPECT_TRUE(it1 == it2);
}

TEST(iterator, inequalityOperator)
{
	intlist list{ 1, 2, 3 };
	auto it1 = list.begin();
	auto it2 = list.begin();
	++it2;
	EXPECT_FALSE(it1 == it2);
}

TEST(iterator, copyConstructor)
{
	intlist list{ 1, 2, 3, 4, 5 };
	auto it1 = list.begin();
	std::advance(it1, 2);
	auto it2 = it1;
	EXPECT_EQ(*it1, *it2);
}

TEST(iterator, equalOperator)
{
	intlist list{ 1, 2, 3, 4, 5 };
	auto it1 = list.begin();
	std::advance(it1, 2);
	auto it2 = list.begin();
	it2 = it1;
	EXPECT_EQ(*it1, *it2);
}

TEST(iterator, desreferenceShouldReturnTheCorrectValue)
{
	const int expected = 1;
	intlist list{ expected, 2, 3 };
	auto it = list.begin();
	EXPECT_EQ(*it, expected);
}

// iterators - insert

TEST(insertUsingIterator, shouldInsertBeforeTheIteratorGiven)
{
	intlist list{ 1, 2, 3 };
	auto it = list.begin();

	const int newNumber = 100;
	list.insert(it, newNumber);

	EXPECT_EQ(*(list.begin()), newNumber);
}

TEST(insertUsingIterator, shouldReturnIteratorToNewValue)
{
	intlist list{ 1, 2, 3 };
	auto it = list.begin();
	auto newIt = list.insert(it, 100);
	EXPECT_EQ(*(newIt), 100);
}

TEST(insertUsingIterator, shouldIncrementNelms)
{
	intlist list;
	list.insert(list.end(), 1);
	list.insert(list.end(), 1);
	list.insert(list.end(), 1);
	EXPECT_EQ(list.size(), 3);
}

// iterators emplace 

TEST(emplaceUsingIterator, shouldInsertBeforeTheIteratorGiven)
{
	const int newvalue = 100;
	intlist list{ 1,2,3 };
	list.emplace(list.begin(), newvalue);
	EXPECT_EQ(list.front(), newvalue);
}

TEST_F(testResourceList, emplaceUsingIteratorShouldNotCopy)
{
	listForTesting.emplace(listForTesting.end(), testResource());
	EXPECT_EQ(testResource::instancesCreated, 1);
}

TEST(emplaceUsingIterator, shouldUpdateNelms)
{
	intlist list{ 1,2,3 };
	list.emplace(list.end(), 10);
	EXPECT_EQ(list.size(), 4);
}

// iterator by movement 

TEST(insertUsingIteratorByMovement, shouldInsertBeforeTheIteratorGiven)
{
	int newvalue = 100;
	intlist list{ 1,2,3 };
	list.insert(list.begin(), std::move(newvalue));
	EXPECT_EQ(list.front(), newvalue);
}

TEST_F(testResourceList, insertByMovementUsingIteratorShouldNotCopy)
{
	listForTesting.insert(listForTesting.end(), testResource());
	EXPECT_EQ(testResource::instancesCreated, 1);
}

TEST(insertUsingIteratorByMovement, shouldUpdateNelms)
{
	intlist list{ 1,2,3 };
	list.insert(list.end(), 10);
	EXPECT_EQ(list.size(), 4);
}

// iterator pop

TEST(popUsingIterator, shouldDeleteTheElementAtIterator)
{
	intlist list{ 1, 2, 3 };
	list.pop(list.begin());
	EXPECT_EQ(list.front(), 2);
}

TEST(popUsingIterator, shouldReturnIteratorToNextElement)
{
	intlist list{ 1, 2, 3 };
	auto it = list.begin();
	auto nextElement = list.pop(it);
	EXPECT_EQ(*nextElement, 2);
}

TEST(popUsingIterator, shouldDecrementNelms)
{
	intlist list{ 1, 2, 3 };
	list.pop(list.begin());
	EXPECT_EQ(list.size(), 2);
}

TEST(popUsingIterator, shouldThrowLengthErrorIfListEmpty)
{
	intlist list;
	EXPECT_THROW(list.pop(list.begin()), std::length_error);
}

TEST(popUsingIterator, shouldThrowExceptionIfIteratorPointsToHead)
{
	intlist list{ 1, 2 };
	auto it = list.end();
	EXPECT_THROW(list.pop(it), std::runtime_error);
}

// operator == 

TEST(operatorEquality, shouldReturnTrueifListsEmpty)
{
	intlist list;
	intlist list2;
	EXPECT_TRUE(compareList(list, list2));
}

TEST(operatorEquality, shouldReturnTrue)
{
	intlist list{ 1, 2, 3, 4, 5 };
	intlist list2{ 1, 2, 3, 4, 5 };
	EXPECT_TRUE(compareList(list, list2));
}

TEST(operatorEquality, shouldReturnFalseIfSizeDifferent)
{
	intlist list{ 1, 2, 3, 4, 5 };
	intlist list2{ 1, 2, 3, 4, 5, 6 };
	EXPECT_FALSE(compareList(list, list2));
}

TEST(operatorEquality, shouldReturnFalse)
{
	intlist list{ 1, 2, 3, 4, 5, 7 };
	intlist list2{ 1, 2, 3, 4, 5, 6 };
	EXPECT_FALSE(compareList(list, list2));
}

// operator = 

TEST(operatorEqual, shouldClearAndAssign)
{
	intlist list{ 1, 2, 3 };
	intlist newlist{ 3, 2, 1 };
	newlist = list;
	EXPECT_TRUE(compareList(newlist, list));
}

TEST(operatorEqual, dontClearifItsTheSameList)
{
	intlist list{ 1, 2, 3 };
	list = list;
	EXPECT_TRUE(compareList(list, intlist{ 1, 2, 3 }));
}

// const_iterator

TEST(const_iterator, cbeginShouldReturnIteratorToBegin)
{
	const int begin = 1;
	intlist list{ begin,2,3 };
	EXPECT_EQ(*(list.cbegin()), begin);
}

TEST(const_iterator, cendShouldReturnIteratorToEnd)
{	
	intlist list{ 1,2,3 };
	EXPECT_ANY_THROW(*(list.cend()));
}

TEST(const_iterator, preIncrementOperator)
{
	intlist list{ 1,2,3 };
	auto cit = list.cend();
	EXPECT_EQ(*(++cit), 1);
	EXPECT_EQ(*(++cit), 2);
	EXPECT_EQ(*(++cit), 3);
}

TEST(const_iterator, postincrementOperator)
{
	intlist list{ 1,2,3 };
	auto cit = list.cbegin();
	EXPECT_EQ(*(cit++), 1);
	EXPECT_EQ(*(cit++), 2);
	EXPECT_EQ(*(cit++), 3);
}

TEST(const_iterator, preDecrementIterator)
{
	intlist list{ 1,2,3 };
	auto cit = list.cend();
	EXPECT_EQ(*(--cit), 3);
	EXPECT_EQ(*(--cit), 2);
	EXPECT_EQ(*(--cit), 1);
}

TEST(const_iterator, postDecrementIterator)
{
	intlist list{ 1,2,3 };
	auto cit = list.cend();
	cit--;
	EXPECT_EQ(*(cit--), 3);
	EXPECT_EQ(*(cit--), 2);
	EXPECT_EQ(*(cit--), 1);
}

TEST(const_iterator, shouldThrowExceptionIfTriesToGetHeadValue)
{
	intlist list;
	auto head = list.cbegin();
	EXPECT_THROW(*head, std::runtime_error);
}

TEST(const_iterator, shouldBeAbleToUseStdAdvance)
{
	intlist list{ 1,2,3 };
	auto it = list.cbegin();
	std::advance(it, 1);
	EXPECT_EQ(*it, 2);
	std::advance(it, 1);
	EXPECT_EQ(*it, 3);
}

TEST(const_iterator, equalityOperator)
{
	intlist list{ 1,2,3 };
	auto it1 = list.cbegin();
	auto it2 = list.cbegin();
	EXPECT_TRUE(it1 == it2);
}

TEST(const_iterator, inequalityOperator)
{
	intlist list{ 1,2,3 };
	auto it1 = list.cbegin();
	auto it2 = list.cbegin();
	std::advance(it2, 1);
	EXPECT_TRUE(it1 != it2);
}

TEST(const_iterator, copyConstructor)
{
	intlist list{ 1,2,3 };
	auto it1 = list.cbegin();
	intlist::const_iterator it2 = it1;
	EXPECT_TRUE(it2 == it1);
}

TEST(const_iterator, equalOperator)
{
	intlist list{ 1,2,3 };
	auto it1 = list.cbegin();
	auto it2 = list.cbegin();
	std::advance(it1, 2); 
	it2 = it1;
	EXPECT_TRUE(it2 == it1);
}

TEST(const_iterator, desreferenceShouldReturnTheCorrectValue)
{
	intlist list{ 1,2,3 };
	auto it = list.cbegin();
	std::advance(it, 2);
	EXPECT_EQ(*it, 3);
}

TEST(const_iterator, shouldBeConstructedUsingAnIterator)
{
	intlist list{ 1,2,3 };
	intlist::iterator normalIterator = list.begin();
	intlist::const_iterator constIterator = normalIterator;
	EXPECT_EQ(*constIterator, *normalIterator);
}

TEST(const_iterator, equalityOperatorWithIterator)
{
	intlist list{ 1,2,3 };
	auto normalIterator = list.begin();
	auto constIterator = list.cbegin();
	EXPECT_TRUE(normalIterator == constIterator);
}

// reverse iterator

TEST(reverse_iterator, rbeginShouldReturnIteratorToLastElement)
{
	const int revbegin = 3;
	intlist list{ 1,2, revbegin};
	EXPECT_EQ(*(list.rbegin()), revbegin);
}

TEST(reverse_iterator, rendShouldReturnIteratorToHead)
{
	intlist list{ 1,2,3 };
	EXPECT_ANY_THROW(*(list.rend()));
}

TEST(reverse_iterator, preIncrementOperator)
{
	intlist list{ 1,2,3 };
	auto revit = list.rbegin();
	EXPECT_EQ(*(++revit), 2);
	EXPECT_EQ(*(++revit), 1);	
}

TEST(reverse_iterator, postIncrementOperator)
{
	intlist list{ 1,2,3 };
	auto revit = list.rbegin();
	EXPECT_EQ(*(revit++), 3);
	EXPECT_EQ(*(revit++), 2);
	EXPECT_EQ(*(revit++), 1);
}

TEST(reverse_iterator, preDecrementOperator)
{
	intlist list{ 1,2,3 };
	auto revit = list.rend();
	EXPECT_EQ(*(--revit), 1);
	EXPECT_EQ(*(--revit), 2);
	EXPECT_EQ(*(--revit), 3);
}

TEST(reverse_iterator, postDecrementOperator)
{
	intlist list{ 1,2,3 };
	auto revit = list.rend();	
	revit--;
	EXPECT_EQ(*(revit--), 1);
	EXPECT_EQ(*(revit--), 2);	
	EXPECT_EQ(*(revit--), 3);	
}

TEST(reverse_iterator, shouldThrowExceptionIfTriesToGetHeadValue)
{
	intlist list;
	auto revit = list.rend();
	EXPECT_ANY_THROW(*revit);
}

TEST(reverse_iterator, shouldBeAbleToUseStdAdvance)
{
	const int target = 100;
	intlist list{ target,2,3 };
	auto revit = list.rbegin();
	std::advance(revit, 2);
	EXPECT_EQ(*revit, target);
}

TEST(reverse_iterator, equalityOperator)
{
	const int target = 100;
	intlist list{ 1,2,target };
	auto revit1 = list.rbegin();
	auto revit2 = list.rbegin();
	EXPECT_TRUE(revit1 == revit2);
}

TEST(reverse_iterator, inequalityOperator)
{
	const int target = 100;
	intlist list{ 1,2,target };
	auto revit1 = list.rbegin();
	auto revit2 = list.rbegin();
	++revit2;
	EXPECT_TRUE(revit1 != revit2);
}

TEST(reverse_iterator, copyConstructor)
{
	intlist list{ 1,2,3 };
	intlist::reverse_iterator revit = list.rbegin();
	intlist::reverse_iterator revit2 = revit;
	EXPECT_TRUE(revit == revit2);
}

TEST(reverse_iterator, equalOperator)
{
	intlist list{ 1,2,3};
	auto revit1 = list.rbegin();
	auto revit2 = list.rbegin();
	++revit1;
	revit2 = revit1;
	EXPECT_TRUE(revit2 == revit1);
}

TEST(reverse_iterator, desreferenceShouldReturnTheCorrectValue)
{
	const int target = 3;
	intlist list{ 1,2,target };
	auto revit = list.rbegin();
	EXPECT_EQ(*revit, target);
}

TEST(reverse_iterator, shouldBeConstructedUsingAnIterator)
{
	intlist list{ 1,2,3 };
	auto it = list.begin();
	intlist::reverse_iterator revit = it;
	EXPECT_EQ(*revit, *it);
}

TEST(reverse_iterator, equalityOperatorWithIterator)
{
	intlist list{ 1,2,3 };
	intlist::reverse_iterator revit = list.rbegin();
	intlist::iterator it = --(list.end());
	EXPECT_TRUE(revit == it);
}