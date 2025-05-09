CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

TARGET = cash_flow_minimizer
SRC = code.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean 