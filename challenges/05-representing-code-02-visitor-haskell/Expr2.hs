{-
   The Visitor pattern lets you emulate the functional style in an object-oriented language.
   Devise a complementary pattern for a functional language.
   It should let you bundle all of the operations on one type together and let you define new types easily.
-}
module Expr where

import Text.Printf

data LiteralValue
    = LiteralValueStr String
    | LiteralValueNum Double

instance Show LiteralValue where
    show (LiteralValueNum x) = show x
    show (LiteralValueStr s) = s

data Expr
    = Binary { left :: Expr, operator :: Char, right :: Expr }
    | Grouping { expr :: Expr }
    | Literal { value :: LiteralValue }
    | Unary { operator :: Char, right :: Expr }
    deriving Show

showLisp :: Expr -> String
showLisp rootExpr = case rootExpr of
    Literal val -> show val
    Binary lhs op rhs -> printf "(%c %s %s)" op (showLisp lhs) (showLisp rhs)
    Grouping expr -> printf "(group %s)" $ showLisp expr
    Unary op rhs -> printf "(%c %s)" op $ showLisp rhs

showRPN :: Expr -> String
showRPN rootExpr = case rootExpr of
    Literal val -> show val
    Binary lhs op rhs -> printf "%s %s %c" (showRPN lhs) (showRPN rhs) op
    Grouping expr -> printf "%s group" $ showRPN expr
    Unary op rhs -> printf "%s %c" (showRPN rhs) op

testExpr :: Expr
testExpr =
    let lhs = Unary '-' (Literal (LiteralValueNum 123))
        rhs = Grouping (Literal (LiteralValueNum 45.67))
    in
        Binary lhs '*' rhs
