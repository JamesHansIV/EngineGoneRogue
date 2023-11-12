# run clang tidy on all project files
cd src
run-clang-tidy $PWD -p ../build -config-file ../.clang-tidy -fix -clang-apply-replacements-binary /usr/local/bin/clang-apply-repcements
