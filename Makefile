TARGETS= test_str_view test_folder_reader app_counter
all:

test_extract_status_code: ExtractStatusCodeFuncTest.cpp utils/log.h ./utils/Counter.h
	g++ -std=c++2a ExtractStatusCodeFuncTest.cpp -lgtest -lpthread -I. -o $@

test_folder_reader: FolderReaderTest.cpp utils/FolderReader.h
	g++ -std=c++2a FolderReaderTest.cpp -lgmock -lgtest -pthread -I. -o $@

app_counter: ./utils/Counter.h CounterApp.cpp
	g++ -std=c++2a ./CounterApp.cpp -I. -o $@

app_stringview: StringViewApp.cpp
	g++ -std=c++2a StringViewApp.cpp -o $@

clean:
	rm -rf $(TARGTS) *.o
