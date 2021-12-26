package br.com.hbobenicio.jlox;

public class Token {
    private final TokenType type;
    private final String lexeme;
    private final Object literal;
    private final int line;

    Token(TokenType type, String lexeme, Object literal, int line) {
        this.type = type;
        this.lexeme = lexeme;
        this.literal = literal;
        this.line = line;
    }

    public int getLength() {
        return this.lexeme.length();
    }

    public String toString() {
        return String.format("Token{type='%s', lexeme='%s', literal='%s'}", type, lexeme, literal);
    }

    public TokenType getType() {
        return type;
    }

    public String getLexeme() {
        return lexeme;
    }

    public Object getLiteral() {
        return literal;
    }

    public int getLine() {
        return line;
    }
}
