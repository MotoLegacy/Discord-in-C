NAME            := dic

#------------------------------------------------

BUILD          := build
SOURCE         := src/
CLI_SOURCE 	   := src/clients/cli

#------------------------------------------------

SOURCES        := $(shell find $(SOURCE) -maxdepth 2 -name '*.c')

# Command Line Interface Client
ifeq ($(CLIENT_CLI),1)
SOURCES        += $(shell find $(CLI_SOURCE) -name '*.c')
CFLAGS         := -DCLIENT_CLI
endif

OBJECTS        := $(addprefix $(BUILD)/,$(SOURCES:%.c=%.o))

#-------------------------------------------------

CFLAGS         += -Wall -g -fPIC 
LDFLAGS        := -lm -lc -lssl -lcurl

#-------------------------------------------------

OUTPUT_BOLD    := `tput bold`
OUTPUT_GREEN   := `tput setaf 2`
OUTPUT_BLUE    := `tput setaf 6`
OUTPUT_NORMAL  := `tput sgr0`

#-------------------------------------------------
all: $(NAME)
#-------------------------------------------------

#-------------------------------------------------
$(NAME): $(OBJECTS)
#-------------------------------------------------
	@echo Linking...
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LDFLAGS)
	@echo $(OUTPUT_BOLD)$(OUTPUT_GREEN)Build done.$(OUTPUT_NORMAL)

#-------------------------------------------------
$(BUILD)/%.o: %.c
#-------------------------------------------------
	@echo $(OUTPUT_BOLD)- $(subst $(SOURCE)/,,$<)$(OUTPUT_NORMAL)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(dir $<) -c $< -o $@

#-------------------------------------------------
clean:
#-------------------------------------------------
	@echo $(OUTPUT_BLUE)$(OUTPUT_BOLD)Cleaning...$(OUTPUT_NORMAL)
	@rm -rf $(BUILD) $(NAME)