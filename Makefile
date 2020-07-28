cc = g++
prom = simulator
deps = $(shell ls | find *.h)
src = $(shell ls | find *.cpp)
obj = $(src:%.cpp=%.o)

%.o: %.cpp $(deps)
	$(cc) -c $< -o $@
IRDecoder.o: IRDecoder.hpp $(dpes)
	$(cc) -c $< -o $@
$(prom): $(obj)
	$(cc) -o $(prom) $(obj)

clean:
	rm -rf $(obj) $(prom)

