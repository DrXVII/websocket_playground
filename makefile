CXX_FLAGS = -Wall -Wextra
CXX_LIBS = \
	-lboost_system \
	-lboost_thread \
	-lboost_coroutine \
	-lboost_context \
	-lssl \
	-lcrypto \
	-lboost_chrono \
	-lboost_date_time \
	-lboost_atomic \
	-lpthread
CXX_INCLUDE_DIRS = -I include/3rd_party -I include
SERVER_SRC = src/server.cpp
CLIENT_SRC = src/client.cpp
SERVER_NAME = server
CLIENT_NAME = client

all: $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER_SRC)
	@echo "building $@"
	@g++ -o $@ $(CXX_INCLUDE_DIRS) $(CXX_FLAGS) $(SERVER_SRC) $(CXX_LIBS)

$(CLIENT_NAME): $(CLIENT_SRC)
	@echo "building $@"
	@g++ -o $@ $(CXX_INCLUDE_DIRS) $(CXX_FLAGS) $(CLIENT_SRC) $(CXX_LIBS)

clean:
	@rm -vf $(SERVER_NAME) $(CLIENT_NAME)
