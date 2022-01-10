{-
   The Visitor pattern lets you emulate the functional style in an object-oriented language.
   Devise a complementary pattern for a functional language.
   It should let you bundle all of the operations on one type together and let you define new types easily.
-}
module Expr where

import Text.Printf

data Expr
    = Binary
        { left     :: Expr
        , operator :: Char
        , right    :: Expr
        }
    | Grouping Expr
    | NumLiteral Double
    | StrLiteral String
    | Unary
        { operator :: Char
        , right    :: Expr
        }
    deriving Show

showPrefixNotation :: Expr -> String
showPrefixNotation rootExpr = case rootExpr of
    Binary{ left=lhs, operator=op, right=rhs } ->
        printf "(%c %s %s)" op (showPrefixNotation lhs) (showPrefixNotation rhs)

    Grouping expr ->
        printf "(group %s)" $ showPrefixNotation expr

    Unary{ operator=op, right=rhs } ->
        printf "(%c %s)" op (showPrefixNotation rhs)

    NumLiteral x -> show x
    StrLiteral s -> s

showRPN :: Expr -> String
showRPN rootExpr = case rootExpr of
    Binary{ left=lhs, operator=op, right=rhs } ->
        printf "%s %s %c" (showRPN lhs) (showRPN rhs) op

    Grouping expr ->
        printf "%s group" $ showRPN expr

    Unary{ operator=op, right=rhs } ->
        printf "%s %c" (showRPN rhs) op

    NumLiteral x -> show x
    StrLiteral s -> s

testExpr :: Expr
testExpr =
    Binary
        { left = Unary{operator = '-' , right = NumLiteral 123}
        , operator = '*'
        , right = Grouping (NumLiteral 45.67)
        }

