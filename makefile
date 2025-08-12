CXX = g++
CXXFLAGS = -I /usr/include/eigen3 -std=c++17 -O2
TARGET = main
SRC = Knn_algorithm.cpp planar_segmentation_algorithm.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
