package br.com.hbobenicio.jlox;

public record Token(TokenType type, String lexeme, Object value, int line) {

    public int getLength() {
        return this.lexeme.length();
    }

    public String toString() {
        if (value == null) {
            return String.format("Token{type='%s', lexeme='%s'}", type, lexeme);
        } else {
            return String.format("Token{type='%s', lexeme='%s', value=%s}", type, lexeme, value);
        }
    }
}
