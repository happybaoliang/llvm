#include "clang/Frontend/FrontedActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatcherFinder.h"

using namespace clang::tooling;
using namespace llvm;

static llvm::cl::OptionCategory MyToolCategory("ast-matcher options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp("\nMore help text...");

static int funcParameterCount=0;
DeclarationMatcher FuncMatcher = functionDecl(isDefinition(),parameterCountIs(0)).bind("parameterCount");

static int forStmtCount=0;
StatementMatcher ForMatcher = forStmt().bind("forStmt");

static int whileStmtCount=0;
StatementMatcher WhileMatcher = whileStmt().bind("whileStmt");

static int forStmtWithIf=0;
StatementMatcher ForMatcherWithIf1 = forStmt(hasBody(ifStmt())).bind("forStmtWithIf1");
StatementMatcher ForMatcherWithIf2 = forStmt(hasBody(compoundStmt(forEach(ifStmt())))).bind("forStmtWithIf2");

static int whileStmtWithIf=0;
StatementMatcher WhileMatcherWithIf1 = whileStmt(hasBody(ifStmt())).bind("whileStmtWithIf1");
StatementMatcher WhileMatcherWithIf2 = whileStmt(hasBody(compoundStmt(forEach(ifStmt())))).bind("whileStmtWithIf2");

class LoopPrinter: public MatchFinder::MatchCallBack{
public:
	virtual void run(const MatchFinder::MatchResult & result){
		if (const FunctionDecl *fd=Result.Nodes.getNodeAs<clang::FunctionDecl>("parameterCount")){
			funcParameterCount++;
		}
		
		if (const ForStmt *fs=Result.Nodes.getNodeAs<clang::ForStmt>("forStmt")){
			forStmtCount++;
		}

		if (const WhileStmt *ws=Result.Nodes.getNodeAs<clang::WhileStmt>("whileStmt")){
			whileStmtCount++;
		}

		if (const ForStmt* fs=Result.Nodes.getNodeAs<clang::ForStmt>("forStmtWithIf1")){
			forStmtWithIf++;
		}

		if (const ForStmt* fs=Result.Nodes.getNodeAs<clang::FoStmt>("forStmtWithIf2")){
			forStmtWithIf++;
		}

		if (const WhileStmt* ws=Result.Nodes.getNodeAs<clang::WhileStmt>("whileStmtWithIf1")){
			whileStmtWithIf++;
		}

		if (const WhileStmt* ws=Result.Nodes.getNodeAs<clang::WhileStmt>("whileStmtWithIf2")){
			whileStmtWithIf++;
		}
	}
};

int main(int argc, const char* argv){
	CommonOptionsParser OptionsParser(argc,argv,MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(),OptionsParser.getSourcePathList());

	MatchFinder Finder;
	LoopPrinter Printer;
	Finder.addMatcher(FuncMatcher,&Printer);
	Finder.addMatcher(ForMatcher,&Printer);
	Finder.addMatcher(WhileMatcher,&Printer);
	Finder.addMatcher(ForMatcherWithIf1,&Printer);
	Finder.addMatcher(ForMatcherWithIf2,&Printer);
	Finder.addMatcher(WhileMatcherWithIf1,&Printer);
	Finder.addMatcher(WhileMatcherWithIf2,&Printer);

	Tool.run(newFrontedActionFactory(&Finder).get());
	
	llvm::outs()<<"function parameter count:"<<funcParameterCount<<"\n";
	llvm::outs()<<"forStmt Count:"<<forStmtCount<<"\n";
	llvm::outs()<<"forStmt with If Count:"<<forStmtWithIf<<"\n";
	llvm::outs()<<"whileStmt Count:"<<whileStmtCount<<"\n";
	llvm::outs()<<"whileStmt with If Count:"<<whileStmtWithIf<<"\n";

	return 0;
}
