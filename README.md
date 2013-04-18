This was a quick and dirty modification of Michael Kerrisk's nftw_dir_tree.c from his examples.
I needed something that could walk through a directory with probably several million files, and delete them.
Other utilities that I tried (rm -rf, find ...) were using several gigabytes of memory, which I didn't like (nor the server that was trying to delete them).

Things to do:

1. ~~Clean it up (remove commented out stuff, add Makefile).~~
2. Add options for verbosity.
3. Add option for quiet mode.
4. Integrate some ionice functionality to make it run with idle priority.
5. Maybe let it fork off and run in the background (usually the time you'll need this, you don't want to sit around for hours watching it).
6. Make this my own, Michael Kerrisk might have taught me how to do this, but it has strayed a long ways from his example.
7. Have it log somewhere - verbose or just total files deleted.
8. Remove directories as well as files.
9. Maybe this will save me some time so I can go on a date... (maybe)
