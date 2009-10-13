#include "ProjectTypeFixer.h"
#include "SolutionFileFixer.h"

int main()
{
	ProjectTypeFixer projectTypeFixer;
	projectTypeFixer.fix();
	SolutionFileFixer solutionFileFixer;
	solutionFileFixer.fix();
	return 0;
}
