package br.com.hbobenicio.jlox;

public abstract class Expr {

    public interface Visitor<R> {
        R visitBinaryExpr(Binary expr);
        R visitGroupingExpr(Grouping expr);
        R visitLiteralExpr(Literal expr);
        R visitUnaryExpr(Unary expr);
    }

    public abstract <R> R accept(Visitor<R> visitor);

    public static class Binary extends Expr {
        private Expr left;
        private Token operator;
        private Expr right;

        public Binary(Expr left, Token operator, Expr right) {
            this.left = left;
            this.operator = operator;
            this.right = right;
        }

        @Override
        public <R> R accept(Visitor<R> visitor) {
            return visitor.visitBinaryExpr(this);
        }

        public Expr getLeft() {
            return left;
        }

        public void setLeft(Expr left) {
            this.left = left;
        }

        public Token getOperator() {
            return operator;
        }

        public void setOperator(Token operator) {
            this.operator = operator;
        }

        public Expr getRight() {
            return right;
        }

        public void setRight(Expr right) {
            this.right = right;
        }
    }
    public static class Grouping extends Expr {
        private Expr expression;

        public Grouping(Expr expression) {
            this.expression = expression;
        }

        @Override
        public <R> R accept(Visitor<R> visitor) {
            return visitor.visitGroupingExpr(this);
        }

        public Expr getExpression() {
            return expression;
        }

        public void setExpression(Expr expression) {
            this.expression = expression;
        }
    }
    public static class Literal extends Expr {
        private Object value;

        public Literal(Object value) {
            this.value = value;
        }

        @Override
        public <R> R accept(Visitor<R> visitor) {
            return visitor.visitLiteralExpr(this);
        }

        public Object getValue() {
            return value;
        }

        public void setValue(Object value) {
            this.value = value;
        }
    }
    public static class Unary extends Expr {
        private Token operator;
        private Expr right;

        public Unary(Token operator, Expr right) {
            this.operator = operator;
            this.right = right;
        }

        @Override
        public <R> R accept(Visitor<R> visitor) {
            return visitor.visitUnaryExpr(this);
        }

        public Token getOperator() {
            return operator;
        }

        public void setOperator(Token operator) {
            this.operator = operator;
        }

        public Expr getRight() {
            return right;
        }

        public void setRight(Expr right) {
            this.right = right;
        }
    }
}
