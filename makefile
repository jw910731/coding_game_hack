all:
	yapf -ri .
	clang-format -i $(shell find . -type f -name "*.cpp") \
					$(shell find . -type f -name "*.h") \
					$(shell find . -type f -name "*.c")
	
	cd code && make
	python3 make_c.py
	python3 auto_report.py
clean:
	cd code && make clean
	rm -rf test.docx Answer.cpp