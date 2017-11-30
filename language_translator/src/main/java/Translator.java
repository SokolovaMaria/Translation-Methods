import org.antlr.v4.runtime.tree.TerminalNode;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class Translator extends SimpleOCamlBaseListener {
    private static Map<String, ArrayList<String>> declarations = new HashMap<>();
    private static Map<String, String> definitions = new HashMap<>();
    private static ArrayList<String> main = new ArrayList<>();
    private static String ARG = "arg";
    private static String AUTO = "auto";
    private static int i = 0;

    private String getNewName() {
        return ARG + (i++);
    }

    private static class Header {
        String str;
        Map<String, String> argMap;

        Header(String str, Map<String, String> argMap) {
            this.str = str;
            this.argMap = argMap;
        }
    }

    @Override
    public void exitDeclaration(SimpleOCamlParser.DeclarationContext ctx) {
        String funcID = ctx.ID().getText();
        if (declarations.containsKey(funcID)) {
            throw new IllegalStateException("Multiple function declarations: " + funcID);
        }
        ArrayList<String> types = new ArrayList<>();
        for (SimpleOCamlParser.TypeContext argType : ctx.argTypes().type()) {
            types.add(translateType(argType));
        }
        types.add(translateType(ctx.resType().type()));
        declarations.put(funcID, types);
    }


    @Override
    public void exitDefinition(SimpleOCamlParser.DefinitionContext ctx) {
        StringBuilder def = new StringBuilder();
        String funcID = ctx.ID().getText();
        i = 0;
        Header header = getHeader(ctx.ID().toString(), ctx.args(), null);
        def.append(header.str);
        def.append(translateBody(ctx.body(), header.argMap));
        definitions.put(funcID, def.toString());
    }

    @Override
    public void exitMain(SimpleOCamlParser.MainContext ctx) {
        for (SimpleOCamlParser.FuncCallContext funcCallContext : ctx.funcCall()) {
            main.add(translateFunctionCall(funcCallContext, new HashMap<String, String>()));
        }
    }

    private Header getHeader(String funcID, SimpleOCamlParser.ArgsContext args, Map<String, String> parentMap) {
        StringBuilder str = new StringBuilder();
        Map<String, String> argMap = new HashMap<>();
        if (parentMap != null) argMap = parentMap;
        if (declarations.containsKey(funcID)) {
            ArrayList<String> types = declarations.get(funcID);
            str.append(types.get(types.size() - 1)).append(' ').append(funcID).append(" (");
            int j = 0;
            for (Iterator<TerminalNode> it = args.ID().iterator(); it.hasNext(); ) {
                String argName = it.next().toString();
                String newName = getNewName();
                str.append(types.get(j)).append(' ').append(newName);
                if (it.hasNext()) {
                    str.append(", ");
                }
                argMap.put(argName, newName);
                j++;
            }
            str.append(")");
        } else {
            //since C++14 we can use auto in lieu of an actual type declaring a lambda function parameter
            str.append(AUTO).append(' ').append(funcID).append(" = ").append("[]").append(" (");
            for (Iterator<TerminalNode> it = args.ID().iterator(); it.hasNext(); ) {
                String argName = it.next().toString();
                String newName = getNewName();
                str.append(AUTO).append(' ').append(newName);
                if (it.hasNext()) {
                    str.append(", ");
                }
                argMap.put(argName, newName);
            }
            str.append(")");
        }
        return new Header(str.toString(), argMap);
    }

    private String translateBody(SimpleOCamlParser.BodyContext ctx, Map<String, String> argMap) {
        StringBuilder body = new StringBuilder();
        body.append(" {\n");
        for (SimpleOCamlParser.Local_definitionContext local_definitionContext : ctx.local_definition()) {
            body.append(translateLocalDefinitions(local_definitionContext, argMap));
        }
        body.append("return ").append(translateExpr(ctx.expr(), argMap)).append(';');
        body.append("\n}\n");
        return body.toString();
    }

    private String translateLocalDefinitions(SimpleOCamlParser.Local_definitionContext ctx, Map<String, String> argMap) {
        StringBuilder def = new StringBuilder();
        Header header = getHeader(ctx.ID().toString(), ctx.args(), argMap);
        def.append(header.str);
        def.append(translateBody(ctx.body(), header.argMap));
        return def.toString();
    }

    private String translateType(SimpleOCamlParser.TypeContext type) {
        if (type.BOOL_T() != null) {
            return "bool";
        }
        if (type.INT_T() != null) {
            return "int";
        }
        if (type.STRING_T() != null) {
            return "string";
        }
        throw new IllegalStateException("Undefined type " + type.getText());
    }


    private String translateExpr(SimpleOCamlParser.ExprContext exprCtx, Map<String, String> argMap) {
        if (exprCtx.binOp() != null) {
            return translateExpr(exprCtx.expr(0), argMap) +
                    translateBinOp(exprCtx.binOp()) + translateExpr(exprCtx.expr(1), argMap);
        }
        if (exprCtx.unaryOp() != null) {
            return translateUnaryOp(exprCtx.unaryOp()) + translateExpr(exprCtx.expr(0), argMap);
        }
        if (exprCtx.variable() != null) {
            return translateVariable(exprCtx.variable(), argMap);
        }
        if (exprCtx.constant() != null) {
            return translateConstant(exprCtx.constant());
        }
        if (exprCtx.funcCall() != null) {
            return translateFunctionCall(exprCtx.funcCall(), argMap);
        }
        if (exprCtx.parenExpr() != null) {
            return "(" + translateExpr(exprCtx.parenExpr().expr(), argMap) + ")";
        }
        throw new IllegalStateException("Failed to parse expression " + exprCtx.getText());
    }

    private String translateBinOp(SimpleOCamlParser.BinOpContext ctx) {
        if (ctx.EQUAL() != null) {
            return "==";
        } else if (ctx.NOT_EQUAL() != null) {
            return "==";
        } else if (ctx.GT() != null) {
            return ">";
        } else if (ctx.LT() != null) {
            return "<";
        } else if (ctx.GE() != null) {
            return ">=";
        } else if (ctx.LE() != null) {
            return "<=";
        } else if (ctx.OR() != null) {
            return "||";
        } else if (ctx.AND() != null) {
            return "&&";
        } else if (ctx.MUL() != null) {
            return "*";
        } else if (ctx.DIV() != null) {
            return "/";
        } else if (ctx.ADD() != null) {
            return "+";
        } else if (ctx.SUB() != null) {
            return "-";
        }
        throw new IllegalStateException("Undefined binary operator " + ctx.getText());
    }

    private String translateUnaryOp(SimpleOCamlParser.UnaryOpContext ctx) {
        if (ctx.NEG() != null) {
            return "!";
        }
        throw new IllegalStateException("Undefined unary operator " + ctx.getText());
    }

    private String translateVariable(SimpleOCamlParser.VariableContext varCtx, Map<String, String> argMap) {
        String var = argMap.get(varCtx.getText());
        return var != null ? var : varCtx.getText();
    }

    private String translateConstant(SimpleOCamlParser.ConstantContext constCtx) {
        return constCtx.getText();
    }

    private String translateFunctionCall(SimpleOCamlParser.FuncCallContext funcCtx, Map<String, String> argMap) {
        StringBuilder funcCall = new StringBuilder();
        funcCall.append(funcCtx.ID().getText());
        funcCall.append('(');
        for (Iterator<SimpleOCamlParser.ExprContext> it = funcCtx.expr().iterator(); it.hasNext(); ) {
            String argName = translateExpr(it.next(), argMap);
            funcCall.append(argName);
            if (it.hasNext()) {
                funcCall.append(", ");
            }
        }
        funcCall.append(')');
        return funcCall.toString();
    }

    public void print(PrintWriter out) {
        for (Map.Entry<String, String> def : definitions.entrySet()) {
            out.println(def.getValue());
        }
        StringBuilder m = new StringBuilder();
        m.append("int main() {\n");
        for (String func_call: main) {
            m.append(func_call).append(";\n");
        }
        m.append("}\n");
        out.print(m.toString());
    }
}
