default: counter

run:
	./counter -c cppcms.js

counter: counter.cpp
	c++ counter.cpp -o counter -Lcppcms/lib -Icppcms/include -lcppcms -lbooster -lz

clean:
	rm -f counter
