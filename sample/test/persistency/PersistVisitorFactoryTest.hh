/*
 * PersistVisitorFactoryTest.hh
 *
 * @date Jul 8, 2015
 * @author Tim Niggemann, III Phys. Inst. A, RWTH Aachen University
 * @copyright GNU General Public License v3.0
 */

#ifndef TEST_PERSISTENCY_PERSISTVISITORFACTORYTEST_HH_
#define TEST_PERSISTENCY_PERSISTVISITORFACTORYTEST_HH_

#include <gtest/gtest.h>

#include "persistency/PersistVisitorFactory.hh"
#ifdef WITH_ROOT
#include "persistency/RootPersistVisitor.hh"
#endif
#ifdef WITH_SQLITE
#include "persistency/SqlitePersistVisitor.hh"
#endif
#include "persistency/JsonPersistVisitor.hh"

template<typename Base, typename T>
inline bool instanceof(T* p) {
	return dynamic_cast<Base*>(p) != NULL;
}

class PersistVisitorFactoryTest: public testing::Test {
protected:

	virtual void SetUp() {
		//
	}
};

TEST_F(PersistVisitorFactoryTest, create) {
#ifdef WITH_ROOT
	ASSERT_TRUE(instanceof<RootPersistVisitor>(PersistVisitorFactory::getInstance()->create("test.root")));
#endif
#ifdef WITH_SQLITE
	ASSERT_TRUE(instanceof<SqlitePersistVisitor>(PersistVisitorFactory::getInstance()->create("test.sqlite")));
#endif
	ASSERT_TRUE(instanceof<JsonPersistVisitor>(PersistVisitorFactory::getInstance()->create("test.txt")));
}

#endif /* TEST_PERSISTENCY_PERSISTVISITORFACTORYTEST_HH_ */
