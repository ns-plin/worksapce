TARGETS= test_str_view test_folder_reader
all:

test_str_view: ExtractStatusCodeFuncTest.cpp utils/log.h
	g++ -std=c++2a ExtractStatusCodeFuncTest.cpp -lgtest -lpthread -I. -o $@

test_folder_reader: FolderReaderTest.cpp utils/FolderReader.h
	g++ -std=c++2a FolderReaderTest.cpp -lgmock -lgtest -pthread -I. -o $@

clean:
	rm -rf $(TARGTS) *.o
