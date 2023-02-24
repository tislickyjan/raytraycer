CXX = g++
TARGET = rt
CFLAGS = -pedantic -Wall -O2 -std=c++14
LFLAGS = -pthread
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)$*.d

OBJDIR := obj/
DEPDIR := $(OBJDIR)deps/

SRCS := $(wildcard *.cpp)
SRCC := $(wildcard *.cc)
OBJS := $(SRCS:%.cpp=$(OBJDIR)%.o) $(SRCC:%.cc=$(OBJDIR)%.o)
DEPS := $(SRCS:%.cpp=$(DEPDIR)%.d) $(SRCC:%.cc=$(DEPDIR)%.d)

.PHONY: all
all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) $(LFLAGS)  $^ -o $@

$(OBJDIR)%.o : %.cpp $(DEPDIR)%.d | $(DEPDIR)
	$(CXX) $(DEPFLAGS) $(CFLAGS) -o $@ -c $<

$(OBJDIR)%.o : %.cc $(DEPDIR)%.d | $(DEPDIR)
	$(CXX) $(DEPFLAGS) $(CFLAGS) -o $@ -c $<

$(DEPS):

$(DEPDIR): ; @mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r $(OBJDIR) $(TARGET)

include $(wildcard $(DEPS))
