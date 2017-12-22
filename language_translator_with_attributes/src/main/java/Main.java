import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;

import java.io.IOException;

class Main {
    public static void main(String[] args) {
        String input = "src/main/resources/example";
        NaiveFuncLexer lexer;
        try {
            lexer = new NaiveFuncLexer(CharStreams.fromFileName(input));
            NaiveFuncParser parser = new NaiveFuncParser(new CommonTokenStream(lexer));
            parser.parse();
            System.out.println(parser.program.toString());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}