//---------------------------------------------

char* ReadFile(const char *filename, int extra_buffer=0)
{
    char *buffer = NULL;
    int string_size, read_size;
    FILE *handler = fopen(filename, "r");

    if (handler) {
        fseek(handler, 0, SEEK_END);
        string_size = ftell(handler);
        rewind(handler);
        buffer = (char*) malloc(extra_buffer + sizeof(char) * (string_size + 1) );
        read_size = fread(buffer, sizeof(char), string_size, handler);
        buffer[string_size] = '\0';
    
        if (string_size != read_size) {
            free(buffer);
            buffer = NULL;
        }
    
        fclose(handler);
    }

    return buffer;
}

//---------------------------------------------

void Replace(char *input, const char *pattern, const char *new_pattern, char *output)
{
	char *found;
        long pos;

        found = strstr(input, pattern);
        if (found != 0) {
                pos = found - input;
                memcpy(output, input, pos);
                memcpy(output + pos, new_pattern, strlen(new_pattern) + 1); //copy the zero
                input += pos;
                input += strlen(pattern);
                strcat(output, input);
        }
}


//---------------------------------------------

