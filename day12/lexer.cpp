#include "lexer.h"

std::string to_string(lexme_type type) {
    switch (type) {
        case lexme_type::start_object:
            return "start_object";
        case lexme_type::end_object:
            return "end_object";
        case lexme_type::start_array:
            return "start_array";
        case lexme_type::end_array:
            return "end_array";
        case lexme_type::comma:
            return "comma";
        case lexme_type::colon:
            return "colon";
        case lexme_type::string:
            return "string";
        case lexme_type::number:
            return "number";
        default:
            return "unknown";
    }
}

std::ostream& operator<<(std::ostream& out, const lexme& l) {
    out << "{type: " << to_string(l.type) << ", value: " << l.value << "}";
    return out;
}

state_action* start_object_state::operator()(lexer* lexer) {
    lexer->next();
    lexer->emit(lexme_type::start_object);

    auto next_state = in_object_state::instance();
    lexer->state_stack.push(next_state);
    return next_state;
}

start_object_state* start_object_state::instance() {
    static start_object_state instance;
    return &instance;
}

//

state_action* start_array_state::operator()(lexer* lexer) {
    lexer->next();
    lexer->emit(lexme_type::start_array);

    auto next_state = in_array_state::instance();
    lexer->state_stack.push(next_state);
    return next_state;
}

start_array_state* start_array_state::instance() {
    static start_array_state instance;
    return &instance;
}

//
state_action* end_array_state::operator()(lexer* lexer) {
    lexer->next();
    lexer->emit(lexme_type::end_array);
    lexer->state_stack.pop();
    return lexer->state_stack.empty() ? nullptr : lexer->state_stack.top();
}

end_array_state* end_array_state::instance() {
    static end_array_state instance;
    return &instance;
}

//

state_action* in_array_state::operator()(lexer* lexer) {
    while (true) {
        switch (lexer->peek()) {
            case '{':
                return start_object_state::instance();
            case '}':
                return object_end_state::instance();
            case '[':
                return start_array_state::instance();
            case ']':
                return end_array_state::instance();
        }
        switch (lexer->next()) {
            case '"':
                lexer->ignore();
                lexer->state_stack.push(this);
                return read_string_state::instance();
            case ',':
                lexer->emit(lexme_type::comma);
                lexer->ignore();
                break;
            case '+':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                lexer->backup();
                lexer->state_stack.push(this);
                return read_number_state::instance();
        }
    }
}

in_array_state* in_array_state::instance() {
    static in_array_state instance;
    return &instance;
}


//
start_state* start_state::instance() {
    static start_state instance;
    return &instance;
}

state_action* start_state::operator()(lexer* l) {
    switch (l->peek()) {
        case '{':
            return start_object_state::instance();
        case '[':
            return start_array_state::instance();
        case '\0':
        default:
            return nullptr;
    }
}

//
state_action* object_end_state::operator()(lexer* lexer) {
    lexer->next();
    lexer->emit(lexme_type::end_object);
    lexer->state_stack.pop();
    return lexer->state_stack.empty() ? nullptr : lexer->state_stack.top();
}

object_end_state* object_end_state::instance() {
    static object_end_state instance;
    return &instance;
}

state_action* read_string_state::operator()(lexer* lexer) {
    char previous_char = '\0';
    for (;;) {
        auto c = lexer->next();
        if (c == '"' && previous_char != '\\') {
            lexer->backup();
            lexer->emit(lexme_type::string);
            lexer->next();
            lexer->ignore();
            break;
        }
        previous_char = c;
    }
    auto next_state = lexer->state_stack.top();
    lexer->state_stack.pop();
    return next_state;
}

read_string_state* read_string_state::instance() {
    static read_string_state instance;
    return &instance;
}

//

state_action* read_number_state::operator()(lexer* lexer) {
    lexer->accept("+-");
    std::string digits = "0123456789";
    lexer->accept_run(digits);
    lexer->emit(lexme_type::number);

    auto next_state = lexer->state_stack.top();
    lexer->state_stack.pop();
    return next_state;
}

read_number_state* read_number_state::instance() {
    static read_number_state instance;
    return &instance;
}

//
state_action* in_object_state::operator()(lexer* lexer) {
    while (true) {
        switch (lexer->peek()) {
            case '{':
                return start_object_state::instance();
            case '}':
                return object_end_state::instance();
            case '[':
                return start_array_state::instance();
            case ']':
                return end_array_state::instance();
        }
        switch (lexer->next()) {
            case '"':
                lexer->ignore();
                lexer->state_stack.push(this);
                return read_string_state::instance();
            case ':':
                lexer->emit(lexme_type::colon);
                lexer->ignore();
                break;
            case ',':
                lexer->emit(lexme_type::comma);
                lexer->ignore();
                break;
            case '+':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                lexer->backup();
                lexer->state_stack.push(this);
                return read_number_state::instance();
        }

    }
}

in_object_state* in_object_state::instance() {
    static in_object_state instance;
    return &instance;
}

// -----------------------------------------------------------------------------------

void lexer::run() {
    // Thanks, Rob Pike, nice talk
    for (state_action* state = start_state::instance();
         state != nullptr;
         state = (*state)(this)) {
    }
}

void lexer::emit(lexme_type type) {
    listener->on_lexme(new lexme{type, source.substr(start, pos - start)});
    start = pos;
}

char lexer::peek() {
    auto result = next();
    backup();
    return result;
}

char lexer::next() {
    if (pos >= source.size()) {
        return '\0';
    }
    return source[pos++];
}

void lexer::ignore() {
    start = pos;
}

void lexer::backup() {
    pos--;
}

// consumes the next character
// if it's from the valid set
bool lexer::accept(const std::string& valid_character_set) {
    if (valid_character_set.find(next()) != std::string::npos) {
        return true;
    }
    backup();
    return false;
}

// consumes a run of characters from the valid set
void lexer::accept_run(const std::string& valid_character_set) {
    for (; accept(valid_character_set););
}
