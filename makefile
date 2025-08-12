CXX = g++
CXXFLAGS = -I /usr/include/eigen3 -std=c++17 -O2
TARGET = main
SRC = ./V1/include/Knn_algorithm.cpp ./V1/include/planar_segmentation_algorithm.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
