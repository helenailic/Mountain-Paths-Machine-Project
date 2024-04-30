CXX=clang++
INCLUDES=-Iincludes/
CXXFLAGS=-std=c++20 -O0 -gdwarf-4 -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined,implicit-conversion,local-bounds -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize-address-use-after-return=always -Wno-error=unused-parameter $(INCLUDES)

exec: bin/exec
tests: bin/tests

bin/exec: ./src/driver.cc ./includes/color.hpp ./src/color.cc ./includes/elevation_dataset.hpp ./src/elevation_dataset.cc ./includes/grayscale_image.hpp ./src/grayscale_image.cc ./includes/path_image.hpp ./src/path_image.cc ./includes/path.hpp ./src/path.cc
	$(CXX) $(CXXFLAGS) ./src/driver.cc ./src/color.cc ./src/elevation_dataset.cc ./src/grayscale_image.cc ./src/path_image.cc ./src/path.cc -o $@

bin/tests: obj/tests_main.o ./tests/tests.cc ./includes/color.hpp ./src/color.cc ./includes/elevation_dataset.hpp ./src/elevation_dataset.cc ./includes/grayscale_image.hpp ./src/grayscale_image.cc ./includes/path_image.hpp ./src/path_image.cc ./includes/path.hpp ./src/path.cc
	$(CXX) -std=c++20 -Iincludes -gdwarf-4 -fstandalone-debug -O0 -Wall -Wextra -pedantic obj/tests_main.o ./tests/tests.cc ./src/color.cc ./src/elevation_dataset.cc ./src/grayscale_image.cc ./src/path_image.cc ./src/path.cc -o $@

obj/tests_main.o: ./tests/tests_main.cc
	$(CXX) -std=c++20 -Iincludes -gdwarf-4 -fstandalone-debug -O0 -Wall -Wextra -pedantic -c ./tests/tests_main.cc -o $@

.DEFAULT_GOAL := tests
.PHONY: clean exec tests

clean:
	rm -fr bin/* obj/*
