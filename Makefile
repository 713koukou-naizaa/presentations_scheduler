CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude
LDFLAGS = -static -static-libgcc -static-libstdc++

CXXFLAGS_COV = $(CXXFLAGS) -fprofile-arcs -ftest-coverage
LDFLAGS_COV = $(LDFLAGS) -lgcov

MAIN = main.cpp
SRC = src/Scheduler.cpp src/Teacher.cpp src/Room.cpp src/Utils.cpp src/config.cpp
TESTS_SRC = tests/UtilsTest.cpp tests/TeacherTest.cpp tests/SchedulerTest.cpp

APP_OUT = bin/main
TESTS_OUT = bin/tests
COVERAGE_OUT = coverage-report/index.html

all: app-test-coverage

app:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(MAIN) $(SRC) -o $(APP_OUT) $(LDFLAGS)

test:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(TESTS_SRC) $(SRC) -lgtest -lgtest_main -pthread -o $(TESTS_OUT)

coverage:
	mkdir -p bin
	rm -f *.gcda *.gcno
	$(CXX) $(CXXFLAGS_COV) $(TESTS_SRC) $(SRC) -lgtest -lgtest_main -pthread -o $(TESTS_OUT)
	./$(TESTS_OUT)
	mkdir -p coverage-report
	gcovr -r . --html --html-details --exclude 'tests/*' --exclude 'include/*' -o $(COVERAGE_OUT)

test-coverage:
	mkdir -p bin
	rm -f *.gcda *.gcno
	$(CXX) $(CXXFLAGS_COV) $(TESTS_SRC) $(SRC) -lgtest -lgtest_main -pthread -o $(TESTS_OUT)
	./$(TESTS_OUT)
	mkdir -p coverage-report
	gcovr -r . --html --html-details --exclude 'tests/*' --exclude 'include/*' -o $(COVERAGE_OUT)

app-test-coverage:
	mkdir -p bin
	rm -f *.gcda *.gcno
	$(CXX) $(CXXFLAGS) $(MAIN) $(SRC) -o $(APP_OUT)
	$(CXX) $(CXXFLAGS_COV) $(TESTS_SRC) $(SRC) -lgtest -lgtest_main -pthread -o $(TESTS_OUT)
	./$(TESTS_OUT)
	mkdir -p coverage-report
	gcovr -r . --html --html-details --exclude 'tests/*' --exclude 'include/*' -o $(COVERAGE_OUT)


clean:
	rm -f $(APP_OUT)
	rm -f $(TESTS_OUT)
	rm -f *.gcda *.gcno coverage.info
	rm -rf $(COVERAGE_OUT)

clean-app:
	rm -f $(APP_OUT)

clean-test:
	rm -f $(TESTS_OUT)

clean-coverage:
	rm -f *.gcda *.gcno coverage.info
	rm -rf $(COVERAGE_OUT)

clean-test-coverage:
	rm -f $(TESTS_OUT)
	rm -f *.gcda *.gcno coverage.info
	rm -rf $(COVERAGE_OUT)
