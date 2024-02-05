CXXFLAGS=-Wall -g --std=c++17

# testing target
TESTTARGET=ex2test.out
# runnable target
RUNTARGET=ex2.out

# all source files including test
SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(SOURCES:.cpp=.o)

.PHONY: all clean check run

all: $(RUNTARGET) $(TESTTARGET)

check: $(TESTTARGET)
	./$(TESTTARGET)

run: $(RUNTARGET)
	./$(RUNTARGET)

$(TESTTARGET): $(SOURCES)
	$(CXX) $(CPPFLAGS) -DTESTING $(CXXFLAGS) $^ -o $@

$(RUNTARGET): $(SOURCES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf \
		$(OBJECTS)					\
		$(RUNTARGET)				\
		$(RUNTARGET:.out=.out.dSYM)	\
		$(TESTTARGET)				\
		$(TESTTARGET:.out=.out.dSYM)
