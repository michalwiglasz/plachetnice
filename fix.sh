git filter-branch --commit-filter '
        if [ "$GIT_COMMITTER_NAME" = "Your Name" ];
        then
                GIT_COMMITTER_NAME="Michal Wiglasz";
                GIT_AUTHOR_NAME="Michal Wiglasz";
                GIT_COMMITTER_EMAIL="michal.wiglasz@gmail.com";
                GIT_AUTHOR_EMAIL="michal.wiglasz@gmail.com";
                git commit-tree "$@";
        else
                git commit-tree "$@";
        fi' HEAD