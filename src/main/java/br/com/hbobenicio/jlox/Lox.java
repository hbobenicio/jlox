package br.com.hbobenicio.jlox;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class Lox {
    private static boolean hadError = false;

    public static void main(String[] args) throws IOException {
        if (args.length > 1) {
            System.err.println("usage: jlox [script]");
            System.exit(64);
        } else if (args.length == 1) {
            runFile(args[0]);
        } else {
            runPrompt();
        }
    }

    private static void runFile(String inputFilePath) throws IOException {
        byte[] bytes = Files.readAllBytes(Paths.get(inputFilePath));
        var fileContents = new String(bytes, Charset.defaultCharset());

        run(fileContents);

        if (hadError) {
            System.exit(65);
        }
    }

    private static void runPrompt() throws IOException {
        try (var reader = new BufferedReader(new InputStreamReader(System.in))) {
            while (true) {
                System.out.print("\u001B[36mjlox>\u001B[0m ");
                var line = reader.readLine();
                if (line == null) {
                    break;
                }

                run(line);
                hadError = false;
            }
        }
    }

    private static void run(String source) {
        Scanner scanner = new Scanner(source);
        List<Token> tokens = scanner.scanTokens();

        // For now, just print the tokens.
        for (Token token : tokens) {
            System.out.println(token);
        }
    }

    public static void error(int line, String message) {
        report(line, "", message);
    }

    private static void report(int line, String where, String message) {
        var s = String.format("[line %d] Error %s: %s", line, where, message);
        System.err.println(s);
        hadError = true;
    }
}
