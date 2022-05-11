#include "tests.hpp"

int final() {
	Test1();
	Test2();
	TestReserveConstructor();
	TestReserveMethod();

    TestTemporaryObjConstructor();
    TestTemporaryObjOperator();
    TestNamedMoveConstructor();
    TestNamedMoveOperator();
    TestNoncopiableMoveConstructor();
    TestNoncopiablePushBack();
    TestNoncopiableInsert();
    TestNoncopiableErase();
    TestNoncopiableResize();

	return 0;
}