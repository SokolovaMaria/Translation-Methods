import org.antlr.v4.runtime.*;

import java.io.IOException;
import java.io.PrintWriter;

class Main {
    public static void main(String[] args) {
        String input = "src/main/resources/example.txt";
        try {
            translate(input);
        } catch (IOException e) {
            System.err.println("Failed to translate program from file " + input);
            e.printStackTrace();
        }
    }

    private static void translate(String input) throws IOException {
        SimpleOCamlLexer lexer = new SimpleOCamlLexer(new ANTLRFileStream(input));
        SimpleOCamlParser parser = new SimpleOCamlParser(new CommonTokenStream(lexer));
        parser.addErrorListener(new BaseErrorListener() {
            @Override
            public void syntaxError (Recognizer<?,?> recognizer,
                                     Object offendingSymbol,
                                     int line,
                                     int charPositionInLine,
                                     String msg,
                                     RecognitionException e) {
                throw new IllegalStateException ("Failed to parse input at line " + line + "\nERROR: " + msg, e);
            }
        });
        try(PrintWriter out = new PrintWriter(System.out)) {
            Translator translator = new Translator();
            parser.addParseListener(translator);
            parser.parse();
            translator.print(out);
        }
    }
}