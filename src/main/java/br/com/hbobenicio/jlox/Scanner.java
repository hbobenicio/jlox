package br.com.hbobenicio.jlox;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static br.com.hbobenicio.jlox.TokenType.*;
import static java.lang.Character.isDigit;

class Scanner {

    /**
     * Input source program.
     */
    private final String source;

    private final List<Token> tokens = new ArrayList<>();

    /**
     * Start position (offset index from source) where the current token being scanned starts.
     */
    private int start = 0;

    /**
     * Current scanning position (offset index from source). Alongside with start, this identifies the lexeme at the end
     * of a scanning step.
     */
    private int current = 0;

    private int line = 1;

    Scanner(String source) {
        this.source = source;
    }

    List<Token> scanTokens() {
        while (!isAtEnd()) {
            // We are at the beginning of the next lexeme.
            start = current;
            scanToken();
        }

        tokens.add(new Token(EOF, "", null, line));
        return tokens;
    }

    private void scanToken() {
        char c = advance();
        switch (c) {
            case '(' -> addToken(LEFT_PAREN);
            case ')' -> addToken(RIGHT_PAREN);
            case '{' -> addToken(LEFT_BRACE);
            case '}' -> addToken(RIGHT_BRACE);
            case ',' -> addToken(COMMA);
            case '.' -> addToken(DOT);
            case '-' -> addToken(MINUS);
            case '+' -> addToken(PLUS);
            case ';' -> addToken(SEMICOLON);
            case '*' -> addToken(STAR);
            case '!' -> addToken(match('=') ? BANG_EQUAL : BANG);
            case '=' -> addToken(match('=') ? EQUAL_EQUAL : EQUAL);
            case '<' -> addToken(match('=') ? LESS_EQUAL : LESS);
            case '>' -> addToken(match('=') ? GREATER_EQUAL : GREATER);
            case '/' -> {
                // double slashes `//` starts a line comment.
                if (match('/')) {
                    while (peek() != '\n' && !isAtEnd()) {
                        advance();
                    }
                } else {
                    addToken(SLASH);
                }
            }
            case ' ', '\r', '\t' -> {
                // noop. skip/ignore them
            }
            case '\n' -> line++;
            case '"' -> string();
            case '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' -> number();
            // the erroneous character is still consumed by the earlier call to advance().
            // That’s important so that we don’t get stuck in an infinite loop.
            default  -> Lox.error(line, "Unexpected character");
        }
    }

    private void number() {
        while (isDigit(peek())) {
            advance();
        }

        // Look for a fractional part.
        // Looking past the decimal point requires a second character of lookahead since
        // we don’t want to consume the `.` until we’re sure there is a digit after it
        if (peek() == '.' && isDigit(peekNext())) {
            // Consume the "."
            advance();

            while (isDigit(peek())) {
                advance();
            }
        }

        Double literalValue = Double.parseDouble(source.substring(start, current));
        addToken(NUMBER, literalValue);
    }

    private void string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') {
                line++;
            }
            advance();
        }

        if (isAtEnd()) {
            Lox.error(line, "Unterminated string.");
            return;
        }

        // The closing ".
        advance();

        // Trim the surrounding quotes.
        var literalValue = source.substring(start + 1, current - 1);
        addToken(STRING, literalValue);
    }

    /**
     * Conditionally advances the current position (consumes) by a character only if the current char equals the
     * expected one.
     */
    private boolean match(char expected) {
        if (isAtEnd()) {
            return false;
        }
        if (source.charAt(current) != expected) {
            return false;
        }
        current++;
        return true;
    }

    private char peek() {
        if (isAtEnd()) {
            return '\0';
        }
        return source.charAt(current);
    }

    private char peekNext() {
        if (current + 1 >= source.length()) {
            return '\0';
        }
        return source.charAt(current + 1);
    }

    private boolean isAtEnd() {
        return current >= source.length();
    }

    private char advance() {
        return source.charAt(current++);
    }

    private void addToken(TokenType type) {
        addToken(type, null);
    }

    private void addToken(TokenType type, Object literal) {
        var lexeme = source.substring(start, current);
        var token = new Token(type, lexeme, literal, line);
        tokens.add(token);
    }
}
