#include <lexer.h>
#include <memwrapper.h>
#include <parser.h>
#include <parsergen.h>
#include <stdio.h>
#include <stringstack.h>
#include <switches.h>
#include <parsergraph.h>
#include <parserutil.h>

void init() {
	spInit();
	parserInit();
	parserInitTables();
	parserInitRules();
	parserGenerateFirstSet();
	parserGenerateFollowSet();
	parserGeneratePredictTable();
}

// Deallocate all dynamically memory
void cleanup() {
	parseTreeDestroy();
	deinitLexer();
	ssCleanup();
	spCleanup();
}

int main(int argc, const char *argv[]) {
	memstat();

	if (MAIN_BREAK == 1) {
		printf("[DEBUG] Ready. [press enter]");
		getchar();
	}

	init();

	if(PRINT_PARSERGEN_DEBUG_MSGS){
		parserDisplayFirstSet();
		parserDisplayFollowSet();
		parserDisplayPredictTable();
	}

	memstat();
	if (MAIN_BREAK == 1) {
		printf("[DEBUG] Initialized. [press enter]");
		getchar();
	}

	// I'm lazy and I'm limiting the user's input to 999 characters. :P
	//char input[1000];
	//int ret = rdLine("> ", input, sizeof(input));
	int ret = 0;
	char input[] =
			"1+2*(sin(0x1,532,bcf + 0.3)-[34+{12-0xbbc,3.163,b}-23]/21)-34,000";
	//char input[] = "1+2";

	if (ret == 0) {
		// Input not truncated and is not empty
		initLexer(input);

		if (parserGenOk()) {
			parseTreeNode *root = parserExec();

			if (parserOk()) {
				parserGenerateDotFile(root, "parseTree.dot", true);
				parserGenerateDotFile(root, "parseTree.uncompressed.dot", false);
				root = parseTreeDeallocateEpsilon(root);
				parserGenerateDotFile(root, "prunedTree.dot", true);
				parserGenerateDotFile(root, "prunedTree.uncompressed.dot", false);
			}
		}
		/*token t;
		 printf("        %-17s %-20s\n", "Token Type", "Token Value");
		 do{
		 t = lexerNext();

		 // Print info
		 printf("[TOKEN] %-17s ", tokenTypeName[t.type + 1]);

		 if(t.value == NULL){
		 printf("%-20s ", "NULL");
		 } else {
		 printf("%-20s ", t.value);
		 }

		 // Wait
		 printf("[press enter]");
		 //printf("\n");
		 getchar();
		 } while(t.type != ERROR && t.type != CHAR_NUL);*/
	}

	memstat();
	if (MAIN_BREAK == 1) {
		printf("[DEBUG] Ready to cleanup. [press enter]");
		getchar();
	}

	cleanup();

	memstat();
	if (MAIN_BREAK == 1) {
		printf("[DEBUG] All done. [press enter]");
		getchar();
	}

	return 0;
}
