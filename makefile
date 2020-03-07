check: check.cpp
	g++ -std=c++11 -o check check.cpp
	./check < check_data.txt
calculate: calculate.cpp
	g++ -std=c++11 -o calculate calculate.cpp
	./calculate < calculate_data.txt
