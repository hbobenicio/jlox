import Expr

main :: IO ()
main = do
    putStrLn $ showPrefixNotation testExpr
    putStrLn $ showRPN testExpr

