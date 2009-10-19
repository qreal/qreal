#include "projectTypeFixer.h"
#include "solutionFileFixer.h"

int main()
{
	ProjectTypeFixer projectTypeFixer;
	projectTypeFixer.fix();
	SolutionFileFixer solutionFileFixer;
	solutionFileFixer.fix();
	return 0;
}
