#include <stdio.h>
#include <string.h>
#include <ctype.h>

// --- Predicate Functions (return 1 for true, 0 for false) ---

// Checks if a character is a C operator.
int is_operator(char c) {
    char operators[] = "+-*/=";
    for (int i = 0; i < strlen(operators); i++) {
        if (c == operators[i]) return 1;
    }
    return 0;
}

// Checks if a character is a C delimiter.
int is_delimiter(char c) {
    char delimiters[] = "{}()[],;";
    for (int i = 0; i < strlen(delimiters); i++) {
        if (c == delimiters[i]) return 1;
    }
    return 0;
}

// Checks if a string is a C keyword.
int is_keyword(const char* buffer) {
    const char* keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(keywords[i], buffer) == 0) {
            return 1;
        }
    }
    return 0;
}

// Checks if a string represents a valid integer.
int is_integer(const char* buffer) {
    if (buffer[0] == '\0') return 0; // Empty string is not an integer
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (!isdigit(buffer[i])) {
            return 0;
        }
    }
    return 1;
}

// Checks if a string represents a simple float (e.g., "12.34").
int is_float(const char* buffer) {
    int dot_count = 0;
    if (buffer[0] == '\0') return 0; // Empty string is not a float
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '.') {
            dot_count++;
        } else if (!isdigit(buffer[i])) {
            return 0;
        }
    }
    // Must have exactly one dot and be longer than just "."
    return dot_count == 1 && strlen(buffer) > 1;
}

// Analyzes a buffered token and prints its classification.
void analyze_token(const char* token) {
    if (is_keyword(token)) {
        printf("%s - Keyword\n", token);
    } else if (is_integer(token)) {
        printf("%s - Integer\n", token);
    } else if (is_float(token)) {
        printf("%s - Float\n", token);
    } else {
        // A valid identifier must start with a letter or underscore
        if (strlen(token) > 0 && (isalpha(token[0]) || token[0] == '_')) {
            printf("%s - Identifier\n", token);
        } else if (strlen(token) > 0) {
            printf("%s - Invalid Identifier\n", token);
        }
    }
}

// --- Main Function ---

int main() {
    FILE *f = fopen("input.txt", "r");
    if (f == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[256];
    char buffer[100] = "";
    int buffer_idx = 0;
    int in_multiline_comment = 0;

    while (fgets(line, sizeof(line), f)) {
        printf("\n--- Analyzing Line: %s", line);

        for (int i = 0; line[i] != '\0'; i++) {
            // --- 1. Handle Comments ---
            if (line[i] == '/' && line[i + 1] == '*') {
                in_multiline_comment = 1;
                i++;
                continue;
            }
            if (line[i] == '*' && line[i + 1] == '/') {
                in_multiline_comment = 0;
                i++;
                continue;
            }
            if (in_multiline_comment) {
                continue;
            }
            if (line[i] == '/' && line[i + 1] == '/') {
                break; // Ignore the rest of the line
            }

            // --- 2. Tokenize ---
            char c = line[i];
            
            // If character is a separator (operator, delimiter, or whitespace)
            if (is_operator(c) || is_delimiter(c) || isspace(c)) {
                // First, process any token currently in the buffer
                if (buffer_idx > 0) {
                    buffer[buffer_idx] = '\0';
                    analyze_token(buffer);
                    buffer_idx = 0; // Reset buffer
                    buffer[0] = '\0';
                }
                // Then, process the separator character itself
                if (is_operator(c)) {
                    printf("%c - Operator\n", c);
                }
                if (is_delimiter(c)) {
                    printf("%c - Delimiter\n", c);
                }
            } else {
                // If it's not a separator, add it to the buffer
                buffer[buffer_idx++] = c;
            }
        }
    }

    // After the file ends, process any final token left in the buffer
    if (buffer_idx > 0) {
        buffer[buffer_idx] = '\0';
        analyze_token(buffer);
    }

    fclose(f);
    return 0;
}