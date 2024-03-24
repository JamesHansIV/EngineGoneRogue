# run clang tidy on all project files
cd src
run-clang-tidy $PWD -p ../build -config-file ../.clang-tidy -fix -clang-apply-replacements-binary /usr/local/bin/clang-apply-replacements -header-filter src/ 

# Todo maybe?
# Integrate with Cmake: https://www.labri.fr/perso/fleury/posts/programming/using-clang-tidy-and-clang-format.html
