#pragma once

#include <string>
#include <stack>
#include <ostream>

enum class lexme_type {
    start_object,
    end_object,
    start_array,
    end_array,
    comma,
    colon,
    string,
    number
};

struct lexme {
    lexme_type type;
    std::string value;
};

std::string to_string(lexme_type type);
std::ostream& operator<<(std::ostream& out, const lexme& l);

struct lexer;

struct state_action {
    virtual state_action* operator()(lexer*) = 0;
};

struct lexer_listener {
    virtual void on_lexme(lexme* l) = 0;
};

struct lexer {
    std::string source;
    lexer_listener* listener;
    size_t start;
    size_t pos;
    std::stack<state_action*> state_stack;

    lexer(std::string source_, lexer_listener* listener_)
            : source(std::move(source_)),
              listener(listener_),
              start(0), pos(0) {
    }

    void run();
    void emit(lexme_type type);
    char peek();
    char next();
    void ignore();
    void backup();

    // consumes the next character
    // if it's from the valid set
    bool accept(const std::string& valid_character_set);

    // consumes a run of characters from the valid set
    void accept_run(const std::string& valid_character_set);
};

struct start_state : public state_action {
    state_action* operator()(lexer* lexer) override;
    static start_state* instance();
};

struct start_object_state : public state_action {
    state_action* operator()(lexer* lexer) override;
    static start_object_state* instance();
};

struct object_end_state : public state_action {
    state_action* operator()(lexer* lexer) override;
    static object_end_state* instance();
};

struct in_object_state : public state_action {
    state_action* operator()(lexer* lexer) override;
    static in_object_state* instance();
};

struct start_array_state : public state_action {
    state_action* operator()(lexer* lexer) override;
    static start_array_state* instance();
};

struct end_array_state : public state_action {
    state_action* operator()(lexer* lexer) override;
    static end_array_state* instance();
};

struct in_array_state : public state_action {
    state_action* operator()(lexer* lexer) override;
    static in_array_state* instance();
};

struct read_string_state : public state_action {
    state_action* operator()(lexer* lexer) override;
    static read_string_state* instance();
};

struct read_number_state : public state_action {
    state_action* operator()(lexer* lexer) override;
    static read_number_state* instance();
};
