package br.com.hbobenicio.jlox;

import java.util.Optional;

import static java.lang.String.format;

public class AstPrinter implements Expr.Visitor<String> {

    // NOTE This is temporary just for testing purposes. You can remove this later on...
    public static void main(String[] args) {
        Expr expression = new Expr.Binary(
                new Expr.Unary(
                        new Token(TokenType.MINUS, "-", null, 1),
                        new Expr.Literal(123)),
                new Token(TokenType.STAR, "*", null, 1),
                new Expr.Grouping(
                        new Expr.Literal(45.67)));

        var printer = new AstPrinter();
        System.out.println(printer.print(expression));
    }


    public String print(Expr expr) {
        return expr.accept(this);
    }

    @Override
    public String visitBinaryExpr(Expr.Binary expr) {
        return format("(%s %s %s)",
                expr.getOperator().lexeme(),
                expr.getLeft().accept(this),
                expr.getRight().accept(this)
        );
    }

    @Override
    public String visitGroupingExpr(Expr.Grouping expr) {
        return format("(group %s)", expr.getExpression().accept(this));
    }

    @Override
    public String visitLiteralExpr(Expr.Literal expr) {
        return Optional.ofNullable(expr.getValue())
                .map(Object::toString)
                .orElse("nil");
    }

    @Override
    public String visitUnaryExpr(Expr.Unary expr) {
        return format("(%s %s)",
                expr.getOperator().lexeme(),
                expr.getRight().accept(this)
        );
    }
}
