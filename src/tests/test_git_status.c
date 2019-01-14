#define DEBUG
#define TEST

#include "../common.h"
#include "../utils/git_status.h"
#include "test.h"

int main(int argc, char const *argv[])
{
    git_status_t *stat = git_status_create();

    char test[][256] = {
        "## develop...origin/develop [ahead 5]\n",
        "## develop...origin/develop [behind 3]\n",
        "M  updated in index 1\n",
        "MM updated in index 2\n",
        "MD updated in index 3\n",
        "A  Add to index 1\n",
        "AM Add to index 2\n",
        "AD Add to index 3\n",
        "D  deleted from index\n",
        "R  renamed in index 1\n",
        "RM renamed in index 2\n",
        "RD renamed in index 3\n",
        "C  copied in index 1\n",
        "CM copied in index 2\n",
        "CD copied in index 3\n",
        " M work tree changed since index 1\n",
        "AM work tree changed since index 2\n",
        "RM work tree changed since index 3\n",
        "CM work tree changed since index 4\n",
        " D deleted in work tree 1\n",
        "MD deleted in work tree 2\n",
        "AD deleted in work tree 3\n",
        "RD deleted in work tree 4\n",
        "CD deleted in work tree 5\n",
        " R renamed in work tree 1\n",
        "DR renamed in work tree 2\n",
        " C copied in work tree 1\n",
        "DC copied in work tree 2\n",
        "DD unmerged, both deleted\n",
        "AU unmerged, added by us\n",
        "UD unmerged, deleted by them\n",
        "UA unmerged, added by them\n",
        "DU unmerged, deleted by us\n",
        "AA unmerged, both added\n",
        "UU unmerged, both modified\n",
        "?? untracked\n"
    };

    unsigned int i;
    for (i = 0; i < 36; i++)
        git_status_row_prase(stat, test[i]);

    test(strcmp(stat->branch, "develop") == 0, "Checking for git_status (branch) [E: develop, A: %s]", stat->branch);
    test(stat->ahead == 5, "Checking for git_status (ahead) [E: 5, A: %d]", stat->ahead);
    test(stat->behind == 3, "Checking for git_status (behind) [E: 3, A: %d]", stat->behind);
    test(stat->untracked == 1, "Checking for git_status (untracked) [E: 1, A: %d]", stat->untracked);
    test(stat->added == 1, "Checking for git_status (added) [E: 1, A: %d]", stat->added);
    test(stat->modified == 9, "Checking for git_status (modified) [E: 9, A: %d]", stat->modified);
    test(stat->renamed == 3, "Checking for git_status (renamed) [E: 3, A: %d]", stat->renamed);
    test(stat->copied == 3, "Checking for git_status (copied) [E: 3, A: %d]", stat->copied);
    test(stat->deleted == 10, "Checking for git_status (deleted) [E: 10, A: %d]", stat->deleted);
    test(stat->staged == 22, "Checking for git_status (staged) [E: 22, A: %d]", stat->staged);
    test(stat->changed == 21, "Checking for git_status (changed) [E: 21, A: %d]", stat->changed);
    test(stat->conflicted == 7, "Checking for git_status (conflicted) [E: 7, A: %d]", stat->conflicted);

    return report_results();
}
