# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# For each target create a dummy rule so the target does not have to exist


# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.tree.Debug:
/Users/user/Developer/sphere/movies/tomatoes/test/Debug/tree:
	/bin/rm -f /Users/user/Developer/sphere/movies/tomatoes/test/Debug/tree


PostBuild.treeEx.Debug:
/Users/user/Developer/sphere/movies/tomatoes/test/Debug/treeEx:
	/bin/rm -f /Users/user/Developer/sphere/movies/tomatoes/test/Debug/treeEx


PostBuild.tree.Release:
/Users/user/Developer/sphere/movies/tomatoes/test/Release/tree:
	/bin/rm -f /Users/user/Developer/sphere/movies/tomatoes/test/Release/tree


PostBuild.treeEx.Release:
/Users/user/Developer/sphere/movies/tomatoes/test/Release/treeEx:
	/bin/rm -f /Users/user/Developer/sphere/movies/tomatoes/test/Release/treeEx


PostBuild.tree.MinSizeRel:
/Users/user/Developer/sphere/movies/tomatoes/test/MinSizeRel/tree:
	/bin/rm -f /Users/user/Developer/sphere/movies/tomatoes/test/MinSizeRel/tree


PostBuild.treeEx.MinSizeRel:
/Users/user/Developer/sphere/movies/tomatoes/test/MinSizeRel/treeEx:
	/bin/rm -f /Users/user/Developer/sphere/movies/tomatoes/test/MinSizeRel/treeEx


PostBuild.tree.RelWithDebInfo:
/Users/user/Developer/sphere/movies/tomatoes/test/RelWithDebInfo/tree:
	/bin/rm -f /Users/user/Developer/sphere/movies/tomatoes/test/RelWithDebInfo/tree


PostBuild.treeEx.RelWithDebInfo:
/Users/user/Developer/sphere/movies/tomatoes/test/RelWithDebInfo/treeEx:
	/bin/rm -f /Users/user/Developer/sphere/movies/tomatoes/test/RelWithDebInfo/treeEx


